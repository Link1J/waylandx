cmake_minimum_required(VERSION 3.16)
project(waylandx VERSION 0.1.0 LANGUAGES CXX C)

#------------------------------------------------------------------------------

set(SOURCES
    src/main.cpp
    src/wl_connection.cpp
    src/wl_event_loop.cpp

    src/protocols/wl/compositor.cpp
    src/protocols/wl/shm.cpp
    src/protocols/wl/seat.cpp

    src/protocols/xdg/wm_base.cpp
)

#------------------------------------------------------------------------------

#add_subdirectory(external/fmt)
#add_subdirectory(external/spdlog)
#add_subdirectory(external/Lyra)
#add_subdirectory(external/sdbus-c++)
#add_subdirectory(external/ljh)

#------------------------------------------------------------------------------

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})
set(OpenGL_GL_PREFERENCE "GLVND")

#find_package(X11 REQUIRED)
find_package(Wayland COMPONENTS Client Server Cursor Egl REQUIRED)
#find_package(UDev REQUIRED)
#find_package(Libinput REQUIRED)
#find_package(OpenGL REQUIRED)
#find_package(EGL REQUIRED)
find_package(XCB COMPONENTS XCB SHM XKB COMPOSITE XFIXES RENDER SHAPE REQUIRED)

find_package(WaylandScanner REQUIRED)
find_program(PKG_CONFIG pkg-config)

#------------------------------------------------------------------------------

#pkg_check_modules(DBUS dbus-1)
pkg_check_modules(XCB_ERRORS xcb-errors)

#------------------------------------------------------------------------------

set(WAYLAND_PROTOCOL_GEN_DIR ${CMAKE_BINARY_DIR}/src/protocols/handler)

function(wayland_protocol_create file_name file_to_use)
    add_custom_command(
        OUTPUT  ${WAYLAND_PROTOCOL_GEN_DIR}/${file_name}.h
                ${WAYLAND_PROTOCOL_GEN_DIR}/${file_name}.c

        DEPENDS ${file_to_use}
        
        COMMAND ${WaylandScanner_EXECUTABLE} server-header
                ${file_to_use} ${WAYLAND_PROTOCOL_GEN_DIR}/${file_name}.h
        
        COMMAND ${WaylandScanner_EXECUTABLE} private-code
                ${file_to_use} ${WAYLAND_PROTOCOL_GEN_DIR}/${file_name}.c
    )

    set(SOURCE_WAYLAND ${SOURCE_WAYLAND} ${WAYLAND_PROTOCOL_GEN_DIR}/${file_name}.c PARENT_SCOPE)
endfunction(wayland_protocol_create)

#------------------------------------------------------------------------------

execute_process(COMMAND ${PKG_CONFIG} --variable=pkgdatadir wayland-protocols OUTPUT_VARIABLE protocol_dir OUTPUT_STRIP_TRAILING_WHITESPACE)
file(MAKE_DIRECTORY "${WAYLAND_PROTOCOL_GEN_DIR}")

wayland_protocol_create(xdg-shell ${protocol_dir}/stable/xdg-shell/xdg-shell.xml)
wayland_protocol_create(xdg-decoration ${protocol_dir}/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml)
wayland_protocol_create(linux-dmabuf ${protocol_dir}/unstable/linux-dmabuf/linux-dmabuf-unstable-v1.xml)
#wayland_protocol_create(wlr-output-management ${CMAKE_SOURCE_DIR}/protocols/wlr-output-management-unstable-v1.xml)
wayland_protocol_create(xdg-output ${protocol_dir}/unstable/xdg-output/xdg-output-unstable-v1.xml)
wayland_protocol_create(server_decoration ${CMAKE_SOURCE_DIR}/protocols/server_decoration.xml)
wayland_protocol_create(wlr-layer-shell ${CMAKE_SOURCE_DIR}/protocols/wlr-layer-shell-unstable-v1.xml)

#------------------------------------------------------------------------------

add_executable(waylandx ${SOURCES} ${SOURCE_WAYLAND})

#------------------------------------------------------------------------------

set_target_properties(waylandx PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

#------------------------------------------------------------------------------

target_include_directories(waylandx PRIVATE 
    "src"
    ${WAYLAND_PROTOCOL_GEN_DIR}
    ${XCB_ERRORS_INCLUDE_DIRS}
    #${DBUS_INCLUDE_DIRS}
    #${systemd_INCLUDE_DIRS}
)

#------------------------------------------------------------------------------

target_link_directories(waylandx PRIVATE 
    ${XCB_ERRORS_LIBRARY_DIRS}
    #${DBUS_LIBRARY_DIRS}
    #${systemd_LIBRARY_DIRS}
)

#------------------------------------------------------------------------------

target_link_libraries(waylandx PRIVATE
    Wayland::Server
    #X11::X11
    #OpenGL::GL
    #Libinput::Libinput
    #UDev::UDev
    #fmt::fmt
    #EGL::EGL
    Wayland::Client
    XCB::XCB
    XCB::COMPOSITE
    XCB::XKB
    XCB::SHM
    #spdlog::spdlog
    #sdbus-c++
    #ljh

    #gbm
    GLESv2
    #xkbcommon
    rt

    ${XCB_ERRORS_LIBRARIES}
    #${DBUS_LIBRARIES}
)

#------------------------------------------------------------------------------

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "/usr/local/lib64")

install(
    TARGETS waylandx
    RUNTIME
    DESTINATION bin
    COMPONENT runtime
)