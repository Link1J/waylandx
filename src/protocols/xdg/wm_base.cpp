#include "wm_base.hpp"
#include <xdg-shell.h>
#include <wl_connection.hpp>

const struct wl_interface* ::protocol::xdg::wm_base::interface() const noexcept
{
    return &xdg_wm_base_interface;
}

int ::protocol::xdg::wm_base::version() const noexcept
{
    return 1;
}

void* ::protocol::xdg::wm_base::functions() const noexcept
{
    static struct xdg_wm_base_interface interface = {
        .destroy           = &::protocol::xdg::wm_base::destroy,
        .create_positioner = &::protocol::xdg::wm_base::create_positioner,
        .get_xdg_surface   = &::protocol::xdg::wm_base::get_xdg_surface,
        .pong              = &::protocol::xdg::wm_base::pong,
    };
    return &interface;
}

void ::protocol::xdg::wm_base::bind(struct wl_resource* resource, struct wl_client* client, void* data,
                                    uint32_t version, uint32_t id) noexcept
{}

void ::protocol::xdg::wm_base::destory(struct wl_resource* resource) noexcept
{}

void ::protocol::xdg::wm_base::destroy(struct wl_client* client, struct wl_resource* resource)
{}

void ::protocol::xdg::wm_base::create_positioner(struct wl_client* client, struct wl_resource* resource, uint32_t id)
{}

void ::protocol::xdg::wm_base::get_xdg_surface(struct wl_client* client, struct wl_resource* resource, uint32_t id,
                                               struct wl_resource* surface)
{
    ::wl::connection::get().create_resource(new xdg::surface, client, wl_resource_get_version(resource), id, surface);
}

void ::protocol::xdg::wm_base::pong(struct wl_client* client, struct wl_resource* resource, uint32_t serial)
{}

const struct wl_interface* ::protocol::xdg::surface::interface() const noexcept
{
    return &xdg_surface_interface;
}

int ::protocol::xdg::surface::version() const noexcept
{
    return 1;
}

void* ::protocol::xdg::surface::functions() const noexcept
{
    static struct xdg_surface_interface interface = {
        .destroy             = &::protocol::xdg::surface::destroy,
        .get_toplevel        = &::protocol::xdg::surface::get_toplevel,
        .get_popup           = &::protocol::xdg::surface::get_popup,
        .set_window_geometry = &::protocol::xdg::surface::set_window_geometry,
        .ack_configure       = &::protocol::xdg::surface::ack_configure,
    };
    return &interface;
}

void ::protocol::xdg::surface::bind(struct wl_resource* resource, struct wl_client* client, void* data,
                                    uint32_t version, uint32_t id) noexcept
{}

void ::protocol::xdg::surface::destory(struct wl_resource* resource) noexcept
{}

void ::protocol::xdg::surface::create(struct wl_resource* resource, void* data) noexcept
{}

void ::protocol::xdg::surface::destroy(struct wl_client* client, struct wl_resource* resource)
{}

void ::protocol::xdg::surface::get_toplevel(struct wl_client* client, struct wl_resource* resource, uint32_t id)
{
    ::wl::connection::get().create_resource(new xdg::toplevel, client, wl_resource_get_version(resource), id,
                                            wl_resource_get_user_data(resource));
}

void ::protocol::xdg::surface::get_popup(struct wl_client* client, struct wl_resource* resource, uint32_t id,
                                         struct wl_resource* parent, struct wl_resource* positioner)
{}

void ::protocol::xdg::surface::set_window_geometry(struct wl_client* client, struct wl_resource* resource, int32_t x,
                                                   int32_t y, int32_t width, int32_t height)
{}

void ::protocol::xdg::surface::ack_configure(struct wl_client* client, struct wl_resource* resource, uint32_t serial)
{}

const struct wl_interface* ::protocol::xdg::toplevel::interface() const noexcept
{
    return &xdg_toplevel_interface;
}

int ::protocol::xdg::toplevel::version() const noexcept
{
    return 1;
}

void* ::protocol::xdg::toplevel::functions() const noexcept
{
    static struct xdg_toplevel_interface interface = {
        .destroy          = &::protocol::xdg::toplevel::destroy,
        .set_parent       = &::protocol::xdg::toplevel::set_parent,
        .set_title        = &::protocol::xdg::toplevel::set_title,
        .set_app_id       = &::protocol::xdg::toplevel::set_app_id,
        .show_window_menu = &::protocol::xdg::toplevel::show_window_menu,
        .move             = &::protocol::xdg::toplevel::move,
        .resize           = &::protocol::xdg::toplevel::resize,
        .set_max_size     = &::protocol::xdg::toplevel::set_max_size,
        .set_min_size     = &::protocol::xdg::toplevel::set_min_size,
        .set_maximized    = &::protocol::xdg::toplevel::set_maximized,
        .unset_maximized  = &::protocol::xdg::toplevel::unset_maximized,
        .set_fullscreen   = &::protocol::xdg::toplevel::set_fullscreen,
        .unset_fullscreen = &::protocol::xdg::toplevel::unset_fullscreen,
        .set_minimized    = &::protocol::xdg::toplevel::set_minimized,
    };
    return &interface;
}

void ::protocol::xdg::toplevel::bind(struct wl_resource* resource, struct wl_client* client, void* data,
                                     uint32_t version, uint32_t id) noexcept
{
    //wl_array* states = new wl_array();
    //wl_array_init(states);
    //wl_array_add(states, sizeof(xdg_toplevel_state));
    //((xdg_toplevel_state*)states->data)[0] = XDG_TOPLEVEL_STATE_ACTIVATED;
    //xdg_toplevel_send_configure(resource, 320, 200, states);
    //wl_array_release(states);
}

void ::protocol::xdg::toplevel::destory(struct wl_resource* resource) noexcept
{}

void ::protocol::xdg::toplevel::create(struct wl_resource* resource, void* data) noexcept
{}

void ::protocol::xdg::toplevel::destroy(struct wl_client* client, struct wl_resource* resource)
{}

void ::protocol::xdg::toplevel::set_parent(struct wl_client* client, struct wl_resource* resource,
                                           struct wl_resource* parent)
{}

void ::protocol::xdg::toplevel::set_title(struct wl_client* client, struct wl_resource* resource, const char* title)
{}

void ::protocol::xdg::toplevel::set_app_id(struct wl_client* client, struct wl_resource* resource, const char* app_id)
{}

void ::protocol::xdg::toplevel::show_window_menu(struct wl_client* client, struct wl_resource* resource,
                                                 struct wl_resource* seat, uint32_t serial, int32_t x, int32_t y)
{}

void ::protocol::xdg::toplevel::move(struct wl_client* client, struct wl_resource* resource, struct wl_resource* seat,
                                     uint32_t serial)
{}

void ::protocol::xdg::toplevel::resize(struct wl_client* client, struct wl_resource* resource, struct wl_resource* seat,
                                       uint32_t serial, uint32_t edges)
{}

void ::protocol::xdg::toplevel::set_max_size(struct wl_client* client, struct wl_resource* resource, int32_t width,
                                             int32_t height)
{}

void ::protocol::xdg::toplevel::set_min_size(struct wl_client* client, struct wl_resource* resource, int32_t width,
                                             int32_t height)
{}

void ::protocol::xdg::toplevel::set_maximized(struct wl_client* client, struct wl_resource* resource)
{}

void ::protocol::xdg::toplevel::unset_maximized(struct wl_client* client, struct wl_resource* resource)
{}

void ::protocol::xdg::toplevel::set_fullscreen(struct wl_client* client, struct wl_resource* resource,
                                               struct wl_resource* output)
{}

void ::protocol::xdg::toplevel::unset_fullscreen(struct wl_client* client, struct wl_resource* resource)
{}

void ::protocol::xdg::toplevel::set_minimized(struct wl_client* client, struct wl_resource* resource)
{}
