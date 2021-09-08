#pragma once
#include <wayland-server.h>
#include <string>

namespace wl
{
    struct protocol
    {
        virtual ~protocol() = default;

        virtual const struct wl_interface* interface() const noexcept = 0;
        virtual int                        version() const noexcept   = 0;
        virtual void*                      functions() const noexcept = 0;

        virtual void bind(struct wl_resource* resource, struct wl_client* client, void* data, uint32_t version,
                          uint32_t id) noexcept                     = 0;
        virtual void destory(struct wl_resource* resource) noexcept = 0;
    };

    struct resource : protocol
    {
        virtual void create(struct wl_resource* resource, void* data) noexcept = 0;
    };
} // namespace wl