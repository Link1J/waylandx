#pragma once
#include <wl_protocol.hpp>
#include <linux-dmabuf.h>

namespace protocol::zwp
{
    struct linux_dmabuf : ::wl::protocol
    {
        const struct wl_interface* interface() const noexcept override;
        int                        version() const noexcept override;
        void*                      functions() const noexcept override;

        void bind(struct wl_resource* resource, struct wl_client* client, void* data, uint32_t version,
                  uint32_t id) noexcept override;
        void destory(struct wl_resource* resource) noexcept override;

        static void destroy(struct wl_client* client, struct wl_resource* resource);
        static void create_params(struct wl_client* client, struct wl_resource* resource, uint32_t params_id);
    };

    // struct surface : ::wl::resource
    //{
    //    const struct wl_interface* interface() const noexcept override;
    //    int                        version() const noexcept override;
    //    void*                      functions() const noexcept override;
    //
    //    void bind(struct wl_resource* resource, struct wl_client* client, void* data, uint32_t version,
    //              uint32_t id) noexcept override;
    //    void destory(struct wl_resource* resource) noexcept override;
    //
    //    void create(struct wl_resource* resource, void* data) noexcept override;
    //};

} // namespace protocol::zwp