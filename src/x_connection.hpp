#pragma once
#include <xcb/xcb.h>

namespace x
{
    class connection
    {
        int               default_screen_number = 0;
        xcb_connection_t* display               = nullptr;

        connection(const char* display)
        {
            this->display = xcb_connect(display, &default_screen_number);
        }

        ~connection()
        {
            xcb_disconnect(display);
        }

    public:
        connection(const connection&) = delete;
        connection(connection&&)      = delete;

        static connection& get()
        {
            static connection connection(nullptr);
            return connection;
        }
    };
} // namespace x