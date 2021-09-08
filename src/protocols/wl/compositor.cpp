#include "compositor.hpp"
#include <wl_connection.hpp>
#include <x_connection.hpp>
#include <wayland-server.h>
#include <xcb/xcb.h>
#include "shm.hpp"

const struct wl_interface* ::protocol::wl::compositor::interface() const noexcept
{
    return &wl_compositor_interface;
}

int ::protocol::wl::compositor::version() const noexcept
{
    return 4;
}

void* ::protocol::wl::compositor::functions() const noexcept
{
    static struct wl_compositor_interface interface = {
        .create_surface = &::protocol::wl::compositor::create_surface,
        .create_region  = &::protocol::wl::compositor::create_region,
    };
    return &interface;
}

void ::protocol::wl::compositor::bind(struct wl_resource* resource, struct wl_client* client, void* data,
                                      uint32_t version, uint32_t id) noexcept
{}

void ::protocol::wl::compositor::destory(struct wl_resource* resource) noexcept
{}

void ::protocol::wl::compositor::create_region(struct wl_client* client, struct wl_resource* resource, uint32_t id)
{}

void ::protocol::wl::compositor::create_surface(struct wl_client* client, struct wl_resource* resource, uint32_t id)
{
    ::wl::connection::get().create_resource(new wl::surface, client, wl_resource_get_version(resource), id, nullptr);
}

const struct wl_interface* ::protocol::wl::surface::interface() const noexcept
{
    return &wl_surface_interface;
}

int ::protocol::wl::surface::version() const noexcept
{
    return 4;
}

void* ::protocol::wl::surface::functions() const noexcept
{
    static struct wl_surface_interface interface = {
        .destroy              = &::protocol::wl::surface::destroy,
        .attach               = &::protocol::wl::surface::attach,
        .damage               = &::protocol::wl::surface::damage,
        .frame                = &::protocol::wl::surface::frame,
        .set_opaque_region    = &::protocol::wl::surface::set_opaque_region,
        .set_input_region     = &::protocol::wl::surface::set_input_region,
        .commit               = &::protocol::wl::surface::commit,
        .set_buffer_transform = &::protocol::wl::surface::set_buffer_transform,
        .set_buffer_scale     = &::protocol::wl::surface::set_buffer_scale,
        .damage_buffer        = &::protocol::wl::surface::damage_buffer,
    };
    return &interface;
}

void ::protocol::wl::surface::bind(struct wl_resource* resource, struct wl_client* client, void* data, uint32_t version,
                                   uint32_t id) noexcept
{}

void ::protocol::wl::surface::destory(struct wl_resource* resource) noexcept
{}

void ::protocol::wl::surface::create(struct wl_resource* resource, void* data) noexcept
{
    window = x::window();
}

void ::protocol::wl::surface::destroy(struct wl_client* client, struct wl_resource* resource)
{}

void ::protocol::wl::surface::attach(struct wl_client* client, struct wl_resource* resource, struct wl_resource* buffer,
                                     int32_t x, int32_t y)
{
    auto& connection = x::connection::get();
    auto  surface    = (wl::surface*)wl_resource_get_user_data(resource);
    auto  shm_buf    = (wl::buffer*)wl_resource_get_user_data(buffer);
    surface->window.map();

    auto cookie = xcb_shm_put_image_checked(
        connection, surface->window, connection.gc(), shm_buf->width, shm_buf->height, 0, 0, shm_buf->width,
        shm_buf->height, x, y, connection.screen()->root_depth, 1, false, shm_buf->pool->shmseg, shm_buf->offset);
    x::print_error(xcb_request_check(connection, cookie));
}

void ::protocol::wl::surface::damage(struct wl_client* client, struct wl_resource* resource, int32_t x, int32_t y,
                                     int32_t width, int32_t height)
{}

void ::protocol::wl::surface::frame(struct wl_client* client, struct wl_resource* resource, uint32_t callback)
{}

void ::protocol::wl::surface::set_opaque_region(struct wl_client* client, struct wl_resource* resource,
                                                struct wl_resource* region)
{}

void ::protocol::wl::surface::set_input_region(struct wl_client* client, struct wl_resource* resource,
                                               struct wl_resource* region)
{}

void ::protocol::wl::surface::commit(struct wl_client* client, struct wl_resource* resource)
{}

void ::protocol::wl::surface::set_buffer_transform(struct wl_client* client, struct wl_resource* resource,
                                                   int32_t transform)
{}

void ::protocol::wl::surface::set_buffer_scale(struct wl_client* client, struct wl_resource* resource, int32_t scale)
{}

void ::protocol::wl::surface::damage_buffer(struct wl_client* client, struct wl_resource* resource, int32_t x,
                                            int32_t y, int32_t width, int32_t height)
{}
