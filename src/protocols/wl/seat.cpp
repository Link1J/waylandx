#include "seat.hpp"

const struct wl_interface* ::protocol::wl::seat::interface() const noexcept
{
    return &wl_seat_interface;
}

int ::protocol::wl::seat::version() const noexcept
{
    return 1;
}

void* ::protocol::wl::seat::functions() const noexcept
{
    static struct wl_seat_interface interface = {
        .get_pointer  = ::protocol::wl::seat::get_pointer,
        .get_keyboard = ::protocol::wl::seat::get_keyboard,
        .get_touch    = ::protocol::wl::seat::get_touch,
        .release      = ::protocol::wl::seat::release,
    };
    return &interface;
}

void ::protocol::wl::seat::bind(struct wl_resource* resource, struct wl_client* client, void* data, uint32_t version,
                                uint32_t id) noexcept
{}

void ::protocol::wl::seat::destory(struct wl_resource* resource) noexcept
{}

void ::protocol::wl::seat::get_pointer(struct wl_client* client, struct wl_resource* resource, uint32_t id)
{}

void ::protocol::wl::seat::get_keyboard(struct wl_client* client, struct wl_resource* resource, uint32_t id)
{}

void ::protocol::wl::seat::get_touch(struct wl_client* client, struct wl_resource* resource, uint32_t id)
{}

void ::protocol::wl::seat::release(struct wl_client* client, struct wl_resource* resource)
{}
