#include "wl_event_loop.hpp"

wl::event_loop& wl::event_loop::get()
{
    static wl::event_loop _z_event_loop;
    return _z_event_loop;
}