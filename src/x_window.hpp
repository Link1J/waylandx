#pragma once
#include <xcb/xcb.h>
#include <x_connection.hpp>

namespace x
{
    class window
    {
        xcb_drawable_t id_;
        xcb_gcontext_t gc_;
        uint8_t        depth_;

    public:
        window(std::nullptr_t)
        {
            id_ = 0;
        }

        explicit window()
        {
            auto& connection = x::connection::get();
            id_              = xcb_generate_id(connection);
            gc_              = xcb_generate_id(connection);
            connection.create_window(id_, gc_, depth_);
        }

        void map()
        {
            auto& connection = x::connection::get();
            xcb_map_window(connection, id_);
            xcb_flush(connection);
        }

        operator xcb_drawable_t() const noexcept
        {
            return id_;
        }

        xcb_gcontext_t gc() const noexcept
        {
            return gc_;
        }

        uint8_t depth() const noexcept
        {
            return depth_;
        }
    };
} // namespace x