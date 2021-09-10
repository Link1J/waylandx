#pragma once
#include <wl_protocol.hpp>

namespace protocol::wl
{
    struct seat : ::wl::protocol
    {
        const struct wl_interface* interface() const noexcept override;
        int                        version() const noexcept override;
        void*                      functions() const noexcept override;

        void bind(struct wl_resource* resource, struct wl_client* client, void* data, uint32_t version,
                  uint32_t id) noexcept override;
        void destory(struct wl_resource* resource) noexcept override;

        static void get_pointer(struct wl_client* client, struct wl_resource* resource, uint32_t id);
        static void get_keyboard(struct wl_client* client, struct wl_resource* resource, uint32_t id);
        static void get_touch(struct wl_client* client, struct wl_resource* resource, uint32_t id);
        static void release(struct wl_client* client, struct wl_resource* resource);
    };
} // namespace protocol::wl