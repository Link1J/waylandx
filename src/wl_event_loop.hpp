#pragma once
#include <wayland-server.h>

namespace wl
{
    class event_loop
    {
        friend class connection;

        wl_event_loop* loop;
        event_loop() = default;

    public:
        event_loop(const event_loop&) = delete;
        event_loop(event_loop&&)      = delete;

        static event_loop& get();
    };
} // namespace wl