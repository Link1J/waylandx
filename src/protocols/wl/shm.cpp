#include "shm.hpp"
#include <wl_connection.hpp>
#include <x_connection.hpp>
#include <wayland-server.h>
#include <unistd.h>

struct shm_pool_create_data
{
    int32_t fd;
    int32_t size;
};

struct shm_buffer_create_data
{
    ::protocol::wl::shm_pool* pool;
    int32_t                   offset;
    int32_t                   width;
    int32_t                   height;
    int32_t                   stride;
    uint32_t                  format;
};

const struct wl_interface* ::protocol::wl::shm::interface() const noexcept
{
    return &wl_shm_interface;
}

int ::protocol::wl::shm::version() const noexcept
{
    return 1;
}

void* ::protocol::wl::shm::functions() const noexcept
{
    static struct wl_shm_interface interface = {
        .create_pool = &::protocol::wl::shm::create_pool,
    };
    return &interface;
}

void ::protocol::wl::shm::bind(struct wl_resource* resource, struct wl_client* client, void* data, uint32_t version,
                               uint32_t id) noexcept
{
    wl_shm_send_format(resource, WL_SHM_FORMAT_ARGB8888);
    wl_shm_send_format(resource, WL_SHM_FORMAT_XRGB8888);
}

void ::protocol::wl::shm::destory(struct wl_resource* resource) noexcept
{}

void ::protocol::wl::shm::create_pool(struct wl_client* client, struct wl_resource* resource, uint32_t id, int32_t fd,
                                      int32_t size)
{
    shm_pool_create_data data{fd, size};
    ::wl::connection::get().create_resource(new wl::shm_pool, client, wl_resource_get_version(resource), id, &data);
}

const struct wl_interface* ::protocol::wl::shm_pool::interface() const noexcept
{
    return &wl_shm_pool_interface;
}

int ::protocol::wl::shm_pool::version() const noexcept
{
    return 4;
}

void* ::protocol::wl::shm_pool::functions() const noexcept
{
    static struct wl_shm_pool_interface interface = {
        .create_buffer = &::protocol::wl::shm_pool::create_buffer,
        .destroy       = &::protocol::wl::shm_pool::destroy,
        .resize        = &::protocol::wl::shm_pool::resize,
    };
    return &interface;
}

void ::protocol::wl::shm_pool::bind(struct wl_resource* resource, struct wl_client* client, void* data,
                                    uint32_t version, uint32_t id) noexcept
{}

void ::protocol::wl::shm_pool::destory(struct wl_resource* resource) noexcept
{}

void ::protocol::wl::shm_pool::create(struct wl_resource* resource, void* data) noexcept
{
    auto  info       = (shm_pool_create_data*)data;
    auto& connection = x::connection::get();
    shmseg           = xcb_generate_id(connection);
    fd               = info->fd;
    size             = info->size;

    x::print_error(xcb_request_check(connection, xcb_shm_attach_fd_checked(connection, shmseg, fd, false)));
}

void ::protocol::wl::shm_pool::create_buffer(struct wl_client* client, struct wl_resource* resource, uint32_t id,
                                             int32_t offset, int32_t width, int32_t height, int32_t stride,
                                             uint32_t format) noexcept
{
    shm_buffer_create_data data{
        (::protocol::wl::shm_pool*)wl_resource_get_user_data(resource), offset, width, height, stride, format,
    };
    ::wl::connection::get().create_resource(new wl::buffer, client, wl_resource_get_version(resource), id, &data);
}

void ::protocol::wl::shm_pool::destroy(struct wl_client* client, struct wl_resource* resource) noexcept
{}

void ::protocol::wl::shm_pool::resize(struct wl_client* client, struct wl_resource* resource, int32_t size) noexcept
{}

const struct wl_interface* ::protocol::wl::buffer::interface() const noexcept
{
    return &wl_buffer_interface;
}

int ::protocol::wl::buffer::version() const noexcept
{
    return 1;
}

void* ::protocol::wl::buffer::functions() const noexcept
{
    static struct wl_buffer_interface interface = {
        .destroy = &::protocol::wl::buffer::destroy,
    };
    return &interface;
}

void ::protocol::wl::buffer::bind(struct wl_resource* resource, struct wl_client* client, void* data, uint32_t version,
                                  uint32_t id) noexcept
{}

void ::protocol::wl::buffer::destory(struct wl_resource* resource) noexcept
{}

void ::protocol::wl::buffer::create(struct wl_resource* resource, void* data) noexcept
{
    auto  info       = (shm_buffer_create_data*)data;
    auto& connection = x::connection::get();

    pool   = info->pool;
    offset = info->offset;
    width  = info->width;
    height = info->height;
    stride = info->stride;
    format = info->format;

    WL_SHM_FORMAT_ABGR8888;

    
}

void ::protocol::wl::buffer::destroy(struct wl_client* client, struct wl_resource* resource) noexcept
{}