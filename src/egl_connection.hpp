#pragma once
#include "x_connection.hpp"
#define EGL_NO_X11
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglmesaext.h>

inline static PFNEGLGETPLATFORMDISPLAYEXTPROC          eglGetPlatformDisplayEXT;
inline static PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC eglCreatePlatformWindowSurfaceEXT;
inline static PFNEGLCREATEIMAGEKHRPROC                 eglCreateImageKHR;
inline static PFNEGLDESTROYIMAGEKHRPROC                eglDestroyImageKHR;
inline static PFNEGLQUERYWAYLANDBUFFERWL               eglQueryWaylandBufferWL;
inline static PFNEGLBINDWAYLANDDISPLAYWL               eglBindWaylandDisplayWL;
inline static PFNEGLUNBINDWAYLANDDISPLAYWL             eglUnbindWaylandDisplayWL;
inline static PFNEGLQUERYDMABUFFORMATSEXTPROC          eglQueryDmaBufFormatsEXT;
inline static PFNEGLQUERYDMABUFMODIFIERSEXTPROC        eglQueryDmaBufModifiersEXT;
inline static PFNEGLEXPORTDMABUFIMAGEQUERYMESAPROC     eglExportDMABUFImageQueryMESA;
inline static PFNEGLEXPORTDMABUFIMAGEMESAPROC          eglExportDMABUFImageMESA;
inline static PFNEGLDEBUGMESSAGECONTROLKHRPROC         eglDebugMessageControlKHR;

namespace egl
{
    class connection
    {
        EGLDisplay egl_display;

        void loadEGLProc(void* proc_ptr, const char* name)
        {
            void* proc = (void*)eglGetProcAddress(name);
            if (proc == NULL)
            {
                // spdlog::error("eglGetProcAddress({}) failed", name);
                abort();
            }
            *(void**)proc_ptr = proc;
        }

        connection(const char* display)
        {
            EGLint major, minor;
            egl_display = eglGetDisplay((EGLNativeDisplayType)x::connection::get());
            eglInitialize(egl_display, &major, &minor);
            printf("EGL major: %d, minor %d\n", major, minor);
            // clang-format off
            loadEGLProc(&eglGetPlatformDisplayEXT     , "eglGetPlatformDisplayEXT"     );
            loadEGLProc(&eglBindWaylandDisplayWL      , "eglBindWaylandDisplayWL"      );
            loadEGLProc(&eglUnbindWaylandDisplayWL    , "eglUnbindWaylandDisplayWL"    );
            loadEGLProc(&eglQueryWaylandBufferWL      , "eglQueryWaylandBufferWL"      );
            loadEGLProc(&eglCreateImageKHR            , "eglCreateImageKHR"            );
            loadEGLProc(&eglDestroyImageKHR           , "eglDestroyImageKHR"           );
            //loadEGLProc(&glEGLImageTargetTexture2DOES , "glEGLImageTargetTexture2DOES" );
            loadEGLProc(&eglDebugMessageControlKHR    , "eglDebugMessageControlKHR"    );
            loadEGLProc(&eglQueryDmaBufFormatsEXT     , "eglQueryDmaBufFormatsEXT"     );
            loadEGLProc(&eglQueryDmaBufModifiersEXT   , "eglQueryDmaBufModifiersEXT"   );
            loadEGLProc(&eglExportDMABUFImageQueryMESA, "eglExportDMABUFImageQueryMESA");
            loadEGLProc(&eglExportDMABUFImageMESA     , "eglExportDMABUFImageMESA"     );
            // clang-format on
        }

        ~connection()
        {}

    public:
        connection(const connection&) = delete;
        connection(connection&&)      = delete;

        static connection& get()
        {
            static connection connection(nullptr);
            return connection;
        }

        operator EGLDisplay() const noexcept
        {
            return egl_display;
        }
    };
} // namespace egl