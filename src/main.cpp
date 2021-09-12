#include "x_connection.hpp"
#include "wl_connection.hpp"
#include "client.hpp"

#include "protocols/wl/compositor.hpp"
#include "protocols/wl/shm.hpp"
#include "protocols/wl/seat.hpp"
#include "protocols/xdg/wm_base.hpp"
#include "protocols/zwp/linux_dmabuf.hpp"

#include <thread>

int main(int argc, char const** argv)
{
    x::connection::get();
    auto& connection = wl::connection::get();
    connection.add_listener(&client::create_new_client);

    /*std::thread thread([]() {
        auto&                dpy = x::connection::get();
        xcb_generic_event_t* event;

        while (event = xcb_wait_for_event(dpy))
        {
            free(event);
        }
    });*/

    connection.add_protocol(new protocol::wl::compositor);
    connection.add_protocol(new protocol::wl::shm);
    connection.add_protocol(new protocol::wl::seat);
    connection.add_protocol(new protocol::xdg::wm_base);
    // connection.add_protocol(new protocol::zwp::linux_dmabuf);

    connection.run();
}