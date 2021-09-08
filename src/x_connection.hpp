#pragma once
#include <xcb/xcb.h>
#include <xcb/xcb_errors.h>
#include <cstdio>

namespace x
{
    void print_error(xcb_generic_error_t* error);

    class connection
    {
        xcb_connection_t*     display        = nullptr;
        xcb_screen_t*         default_screen = nullptr;
        xcb_gcontext_t        default_gc     = {0};
        xcb_errors_context_t* error_context  = nullptr;

        connection(const char* display)
        {
            int default_screen_number = 0;
            this->display             = xcb_connect(display, &default_screen_number);
            default_screen            = screen_of_display(default_screen_number);

            uint32_t values[] = {default_screen->black_pixel, default_screen->white_pixel};
            default_gc        = xcb_generate_id(this->display);
            xcb_create_gc(this->display, default_gc, default_screen->root, XCB_GC_FOREGROUND | XCB_GC_BACKGROUND,
                          values);

            xcb_errors_context_new(this->display, &error_context);
        }

        ~connection()
        {
            xcb_errors_context_free(error_context);
            xcb_disconnect(display);
        }

        xcb_screen_t* screen_of_display(int screen)
        {
            xcb_screen_iterator_t iter;
            iter = xcb_setup_roots_iterator(xcb_get_setup(display));
            for (; iter.rem; --screen, xcb_screen_next(&iter))
                if (screen == 0)
                    return iter.data;
            return nullptr;
        }

    public:
        connection(const connection&) = delete;
        connection(connection&&)      = delete;

        static connection& get()
        {
            static connection connection(nullptr);
            return connection;
        }

        void create_window(xcb_drawable_t win)
        {
            uint32_t mask = 0;
            uint32_t values[2];

            mask      = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
            values[0] = default_screen->white_pixel;
            values[1] = XCB_EVENT_MASK_KEY_PRESS;
            xcb_create_window(display, XCB_COPY_FROM_PARENT, win, default_screen->root, 0, 0, 320, 200, 1,
                              XCB_WINDOW_CLASS_INPUT_OUTPUT, default_screen->root_visual, mask, values);
        }

        xcb_screen_t* screen() const noexcept
        {
            return default_screen;
        }

        xcb_gcontext_t gc() const noexcept
        {
            return default_gc;
        }

        operator xcb_connection_t*() const noexcept
        {
            return display;
        }

        xcb_errors_context_t* error() const noexcept
        {
            return error_context;
        }
    };

    inline void print_error(xcb_generic_error_t* error)
    {
        if (!error)
            return;

        auto context = connection::get().error();

        char const* ext;
        auto        error_code = xcb_errors_get_name_for_error(context, error->error_code, &ext);
        auto        major_code = xcb_errors_get_name_for_major_code(context, error->major_code);
        auto        minor_code = xcb_errors_get_name_for_minor_code(context, error->major_code, error->minor_code);

        fprintf(stderr, "%s: %d (%s), sequence: %d, resource id: %d, major code: %d (%s), minor code: %d (%s)", ext,
                int(error->error_code), error_code, int(error->sequence), int(error->resource_id),
                int(error->major_code), major_code, int(error->minor_code), minor_code);

        throw 1;
    }
} // namespace x