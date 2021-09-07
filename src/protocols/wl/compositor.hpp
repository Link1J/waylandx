#pragma once
#include <wl_protocol.hpp>

namespace protocol::wl
{
    struct compositor : ::wl::protocol
    {
        const struct wl_interface* interface() const noexcept override;
        int                        version() const noexcept override;
        void*                      functions() const noexcept override;

        void bind(struct wl_client* client, void* data, uint32_t version, uint32_t id) noexcept override;
        void destory(struct wl_resource* resource) noexcept override;

        static void create_region(struct wl_client* client, struct wl_resource* resource, uint32_t id);
        static void create_surface(struct wl_client* client, struct wl_resource* resource, uint32_t id);
    };

    struct surface : ::wl::resource
    {
        const struct wl_interface* interface() const noexcept override;
        int                        version() const noexcept override;
        void*                      functions() const noexcept override;

        void bind(struct wl_client* client, void* data, uint32_t version, uint32_t id) noexcept override;
        void destory(struct wl_resource* resource) noexcept override;

        void create(struct wl_resource* resource, void* data) noexcept override;

        static void destroy(struct wl_client* client, struct wl_resource* resource);
        static void attach(struct wl_client* client, struct wl_resource* resource, struct wl_resource* buffer,
                           int32_t x, int32_t y);
        static void damage(struct wl_client* client, struct wl_resource* resource, int32_t x, int32_t y, int32_t width,
                           int32_t height);
        static void frame(struct wl_client* client, struct wl_resource* resource, uint32_t callback);
        static void set_opaque_region(struct wl_client* client, struct wl_resource* resource,
                                      struct wl_resource* region);
        static void set_input_region(struct wl_client* client, struct wl_resource* resource,
                                     struct wl_resource* region);
        static void commit(struct wl_client* client, struct wl_resource* resource);
        static void set_buffer_transform(struct wl_client* client, struct wl_resource* resource, int32_t transform);
        static void set_buffer_scale(struct wl_client* client, struct wl_resource* resource, int32_t scale);
        static void damage_buffer(struct wl_client* client, struct wl_resource* resource, int32_t x, int32_t y,
                                  int32_t width, int32_t height);
    };
} // namespace protocol::wl