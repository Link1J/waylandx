#pragma once
#include <xcb/xcb.h>
#include <x_connection.hpp>

namespace x
{
    class window
    {
        xcb_drawable_t id;

    public:
        window(std::nullptr_t)
        {
            id = 0;
        }

        explicit window()
        {
            auto& connection = x::connection::get();
            id               = xcb_generate_id(connection);
            connection.create_window(id);
        }

        void map()
        {
            auto& connection = x::connection::get();
            xcb_map_window(connection, id);
            xcb_flush(connection);
        }

        operator xcb_drawable_t() const noexcept
        {
            return id;
        }
    };
} // namespace x