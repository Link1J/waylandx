#pragma once
#include <wl_protocol.hpp>

namespace protocol::xdg
{
    struct wm_base : ::wl::protocol
    {
        const struct wl_interface* interface() const noexcept override;
        int                        version() const noexcept override;
        void*                      functions() const noexcept override;

        void bind(struct wl_client* client, void* data, uint32_t version, uint32_t id) noexcept override;
        void destory(struct wl_resource* resource) noexcept override;

        static void destroy(struct wl_client* client, struct wl_resource* resource);
        static void create_positioner(struct wl_client* client, struct wl_resource* resource, uint32_t id);
        static void get_xdg_surface(struct wl_client* client, struct wl_resource* resource, uint32_t id,
                                    struct wl_resource* surface);
        static void pong(struct wl_client* client, struct wl_resource* resource, uint32_t serial);
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
        static void get_toplevel(struct wl_client* client, struct wl_resource* resource, uint32_t id);
        static void get_popup(struct wl_client* client, struct wl_resource* resource, uint32_t id,
                              struct wl_resource* parent, struct wl_resource* positioner);
        static void set_window_geometry(struct wl_client* client, struct wl_resource* resource, int32_t x, int32_t y,
                                        int32_t width, int32_t height);
        static void ack_configure(struct wl_client* client, struct wl_resource* resource, uint32_t serial);
    };

    struct toplevel : ::wl::resource
    {
        const struct wl_interface* interface() const noexcept override;
        int                        version() const noexcept override;
        void*                      functions() const noexcept override;

        void bind(struct wl_client* client, void* data, uint32_t version, uint32_t id) noexcept override;
        void destory(struct wl_resource* resource) noexcept override;

        void create(struct wl_resource* resource, void* data) noexcept override;

        static void destroy(struct wl_client* client, struct wl_resource* resource);
        static void set_parent(struct wl_client* client, struct wl_resource* resource, struct wl_resource* parent);
        static void set_title(struct wl_client* client, struct wl_resource* resource, const char* title);
        static void set_app_id(struct wl_client* client, struct wl_resource* resource, const char* app_id);
        static void show_window_menu(struct wl_client* client, struct wl_resource* resource, struct wl_resource* seat,
                                     uint32_t serial, int32_t x, int32_t y);
        static void move(struct wl_client* client, struct wl_resource* resource, struct wl_resource* seat,
                         uint32_t serial);
        static void resize(struct wl_client* client, struct wl_resource* resource, struct wl_resource* seat,
                           uint32_t serial, uint32_t edges);
        static void set_max_size(struct wl_client* client, struct wl_resource* resource, int32_t width, int32_t height);
        static void set_min_size(struct wl_client* client, struct wl_resource* resource, int32_t width, int32_t height);
        static void set_maximized(struct wl_client* client, struct wl_resource* resource);
        static void unset_maximized(struct wl_client* client, struct wl_resource* resource);
        static void set_fullscreen(struct wl_client* client, struct wl_resource* resource, struct wl_resource* output);
        static void unset_fullscreen(struct wl_client* client, struct wl_resource* resource);
        static void set_minimized(struct wl_client* client, struct wl_resource* resource);
    };
} // namespace protocol::xdg