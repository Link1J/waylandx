#pragma once
#include <wl_protocol.hpp>
#include <xcb/shm.h>
#include <xcb/xcb_image.h>

namespace protocol::wl
{
    struct shm : ::wl::protocol
    {
        const struct wl_interface* interface() const noexcept override;
        int                        version() const noexcept override;
        void*                      functions() const noexcept override;

        void bind(struct wl_resource* resource, struct wl_client* client, void* data, uint32_t version,
                  uint32_t id) noexcept override;
        void destory(struct wl_resource* resource) noexcept override;

        static void create_pool(struct wl_client* client, struct wl_resource* resource, uint32_t id, int32_t fd,
                                int32_t size);
    };

    struct shm_pool : ::wl::resource
    {
        xcb_shm_seg_t shmseg;
        int           fd;
        unsigned int  size;
        uint8_t*      data;

        const struct wl_interface* interface() const noexcept override;
        int                        version() const noexcept override;
        void*                      functions() const noexcept override;

        void bind(struct wl_resource* resource, struct wl_client* client, void* data, uint32_t version,
                  uint32_t id) noexcept override;
        void destory(struct wl_resource* resource) noexcept override;

        void create(struct wl_resource* resource, void* data) noexcept override;

        static void create_buffer(struct wl_client* client, struct wl_resource* resource, uint32_t id, int32_t offset,
                                  int32_t width, int32_t height, int32_t stride, uint32_t format) noexcept;
        static void destroy(struct wl_client* client, struct wl_resource* resource) noexcept;
        static void resize(struct wl_client* client, struct wl_resource* resource, int32_t size) noexcept;
    };

    struct buffer : ::wl::resource
    {
        xcb_shm_seg_t             shmseg;
        xcb_image_t*              image;
        xcb_pixmap_t              pid;
        ::protocol::wl::shm_pool* pool;
        int32_t                   offset;
        int32_t                   width;
        int32_t                   height;
        int32_t                   stride;
        uint32_t                  format;

        const struct wl_interface* interface() const noexcept override;
        int                        version() const noexcept override;
        void*                      functions() const noexcept override;

        void bind(struct wl_resource* resource, struct wl_client* client, void* data, uint32_t version,
                  uint32_t id) noexcept override;
        void destory(struct wl_resource* resource) noexcept override;

        void create(struct wl_resource* resource, void* data) noexcept override;

        static void destroy(struct wl_client* client, struct wl_resource* resource) noexcept;
    };
} // namespace protocol::wl