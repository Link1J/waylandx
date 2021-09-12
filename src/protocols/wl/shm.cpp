#include "shm.hpp"
#include <wl_connection.hpp>
#include <x_connection.hpp>
#include <wayland-server.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstring>

#if __ORDER_LITTLE_ENDIAN == __BYTE_ORDER__
#define NATIVE_XCB_IMAGE_ORDER XCB_IMAGE_ORDER_LSB_FIRST
#else
#define NATIVE_XCB_IMAGE_ORDER XCB_IMAGE_ORDER_MSB_FIRST
#endif

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
    // shmseg           = xcb_generate_id(connection);
    fd   = info->fd;
    size = info->size;

    // uint8_t(*test)[256000];
    // this->data = (uint8_t*)mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, 0);
    // test       = (decltype(test))&this->data;
    //(*test)[0] = 0;

    // x::handle_cookie(xcb_shm_attach_fd_checked(connection, shmseg, fd, true));
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

static xcb_format_t const* query_xcb_format_for_depth(xcb_connection_t* const connection, unsigned depth)
{
    xcb_setup_t const* const setup = xcb_get_setup(connection);
    xcb_format_iterator_t    it;
    for (it = xcb_setup_pixmap_formats_iterator(setup); it.rem; xcb_format_next(&it))
    {
        xcb_format_t const* const format = it.data;
        if (format->depth == depth)
        {
            return format;
        }
    }
    return NULL;
}

void ::protocol::wl::buffer::create(struct wl_resource* resource, void* data) noexcept
{
    auto  info       = (shm_buffer_create_data*)data;
    auto& connection = x::connection::get();

    pool         = info->pool;
    offset       = info->offset;
    width        = info->width;
    height       = info->height;
    stride       = info->stride;
    this->format = info->format;

    xcb_format_t const* const format = query_xcb_format_for_depth(connection, 32);
    image = xcb_image_create(width, height, XCB_IMAGE_FORMAT_Z_PIXMAP, format->scanline_pad, format->depth,
                             format->bits_per_pixel, 0, NATIVE_XCB_IMAGE_ORDER, XCB_IMAGE_ORDER_MSB_FIRST, NULL, ~0, 0);

    size_t const image_segment_size = image->stride * image->height;
    image->data = (uint8_t*)mmap(NULL, image_segment_size, PROT_READ | PROT_WRITE, MAP_SHARED, pool->fd, 0);

    shmseg = xcb_generate_id(connection);
    x::handle_cookie(xcb_shm_attach_fd_checked(connection, shmseg, pool->fd, 0));

    WL_SHM_FORMAT_ABGR8888;
}

void ::protocol::wl::buffer::destroy(struct wl_client* client, struct wl_resource* resource) noexcept
{}