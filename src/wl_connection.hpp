#pragma once
#include <wayland-server.h>
#include <string>

#include "wl_protocol.hpp"

namespace wl
{
    class connection
    {
        wl_display*         display;
        wl_protocol_logger* logger;
        wl_listener         client_listener;
        std::string         socketname;

        static void protocol_logger(void* user_data, wl_protocol_logger_type direction,
                                    const wl_protocol_logger_message* message);

        connection();

    public:
        connection(const connection&) = delete;
        connection(connection&&)      = delete;

        static connection& get();

        void add_listener(void (*callback)(struct wl_listener* listener, wl_client* data));
        void add_protocol(wl::protocol* protocol);

        void create_resource(wl::resource* resource, struct wl_client* client, int version, int id, void* data);

        void run();
    };
} // namespace wl