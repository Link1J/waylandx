#pragma once
#include <xcb/xcb.h>
#include <xcb/render.h>
#include <xcb/xcb_errors.h>
#include <cstdio>
#include <tuple>

namespace x
{
    void print_error(xcb_generic_error_t* error);

    template<typename T>
    struct _xcb_cookie_handle;

    template<>
    struct _xcb_cookie_handle<xcb_void_cookie_t>
    {
        static constexpr auto handle = [](auto...) -> void {};
    };

    template<>
    struct _xcb_cookie_handle<xcb_render_query_pict_formats_cookie_t>
    {
        static constexpr auto handle = &xcb_render_query_pict_formats_reply;
    };

    template<typename T>
    decltype(_xcb_cookie_handle<T>::handle(nullptr, std::declval<T>(), nullptr)) handle_cookie(T cookie);

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

        auto find_visual()
        {
            static auto reply = handle_cookie(xcb_render_query_pict_formats(display));

            for (auto screens = xcb_render_query_pict_formats_screens_iterator(reply); screens.rem;
                 xcb_render_pictscreen_next(&screens))
            {
                for (auto depths = xcb_render_pictscreen_depths_iterator(screens.data); depths.rem;
                     xcb_render_pictdepth_next(&depths))
                {
                    for (auto visuals = xcb_render_pictdepth_visuals_iterator(depths.data); visuals.rem;
                         xcb_render_pictvisual_next(&visuals))
                    {
                        for (auto formats = xcb_render_query_pict_formats_formats_iterator(reply); formats.rem;
                             xcb_render_pictforminfo_next(&formats))
                        {
                            if (formats.data->id != visuals.data->format)
                                continue;
                            if (formats.data->depth != 32)
                                continue;

                            auto direct = formats.data->direct;
                            if (direct.alpha_shift == 24 && direct.red_shift == 16 && direct.green_shift == 8 &&
                                direct.blue_shift == 0)
                                return std::tuple{visuals.data, depths.data, formats.data};
                        }
                    }
                }
            }
            throw 1;
        }

        void create_window(xcb_drawable_t win, xcb_gcontext_t gc, uint8_t& depth_out)
        {
            {
                auto [visual, depth, _] = find_visual();
                auto colormap           = xcb_generate_id(this->display);
                auto cookie             = xcb_create_colormap_checked(this->display, XCB_COLORMAP_ALLOC_NONE, colormap,
                                                          default_screen->root, visual->visual);

                print_error(xcb_request_check(this->display, cookie));

                uint32_t mask       = XCB_CW_BACK_PIXEL | XCB_CW_BORDER_PIXEL | XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;
                uint32_t event_mask = XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_STRUCTURE_NOTIFY;
                uint32_t values[]   = {default_screen->white_pixel, default_screen->black_pixel, event_mask, colormap};

                handle_cookie(xcb_create_window(display, depth->depth, win, default_screen->root, 0, 0, 320, 200, 1,
                                                XCB_WINDOW_CLASS_INPUT_OUTPUT, visual->visual, mask, values));

                depth_out = depth->depth;
            }
            {
                uint32_t values[] = {default_screen->black_pixel, default_screen->white_pixel, 0};
                handle_cookie(xcb_create_gc(this->display, gc, win,
                                            XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_GRAPHICS_EXPOSURES, values));
            }
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

    template<typename T>
    decltype(_xcb_cookie_handle<T>::handle(nullptr, std::declval<T>(), nullptr)) handle_cookie(T cookie)
    {
        if constexpr (std::same_as<T, xcb_void_cookie_t>)
        {
            print_error(xcb_request_check(connection::get(), cookie));
        }
        else
        {
            xcb_generic_error_t* error;
            auto                 output = _xcb_cookie_handle<T>::handle(connection::get(), cookie, &error);
            print_error(error);
            return output;
        }
    }

    inline void print_error(xcb_generic_error_t* error)
    {
        if (!error)
            return;

        auto context = connection::get().error();

        char const* ext;
        auto        error_code = xcb_errors_get_name_for_error(context, error->error_code, &ext);
        auto        major_code = xcb_errors_get_name_for_major_code(context, error->major_code);
        auto        minor_code = xcb_errors_get_name_for_minor_code(context, error->major_code, error->minor_code);

        fprintf(stderr, "%s: %d (%s), sequence: %d, resource id: %d, major code: %d (%s), minor code: %d (%s)\n", ext,
                int(error->error_code), error_code, int(error->sequence), int(error->resource_id),
                int(error->major_code), major_code, int(error->minor_code), minor_code);

        throw 1;
    }
} // namespace x

/*
auto reply = xcb_render_query_pict_formats_reply(
    this->display, xcb_render_query_pict_formats_unchecked(this->display), nullptr);

for (auto screens = xcb_render_query_pict_formats_screens_iterator(reply); screens.rem;
     xcb_render_pictscreen_next(&screens))
{
    for (auto depths = xcb_render_pictscreen_depths_iterator(screens.data); depths.rem;
         xcb_render_pictdepth_next(&depths))
    {
        for (auto visuals = xcb_render_pictdepth_visuals_iterator(depths.data); visuals.rem;
             xcb_render_pictvisual_next(&visuals))
        {
            for (auto formats = xcb_render_query_pict_formats_formats_iterator(reply); formats.rem;
                 xcb_render_pictforminfo_next(&formats))
            {
                if (formats.data->id != visuals.data->format)
                    continue;
                fprintf(stderr,
                        "\tDepth: %d\n\tRed: %d (%02X)\n\tGreen: %d (%02X)\n\tBlue: %d (%02X)\n\tAlpha: %d (%02X)\n\n",
                        formats.data->depth, formats.data->direct.red_shift, formats.data->direct.red_mask,
                        formats.data->direct.green_shift, formats.data->direct.green_mask,
                        formats.data->direct.blue_shift, formats.data->direct.blue_mask,
                        formats.data->direct.alpha_shift, formats.data->direct.alpha_mask);
            }
        }
    }
}
*/