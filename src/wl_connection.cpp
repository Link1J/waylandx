#include "wl_connection.hpp"
#include "wl_event_loop.hpp"

wl::connection::connection()
{
    display    = wl_display_create();
    socketname = wl_display_add_socket_auto(display);

    wl::event_loop::get().loop = wl_display_get_event_loop(display);

    wl_display_add_protocol_logger(display, protocol_logger, nullptr);
}

wl::connection& wl::connection::get()
{
    static connection display;
    return display;
}

void wl::connection::add_listener(void (*callback)(struct wl_listener* listener, wl_client* data))
{
    client_listener.notify = (decltype(client_listener.notify))callback;
    wl_display_add_client_created_listener(display, &client_listener);
}

static struct wl_resource* bind(struct wl_client* client, void* data, uint32_t version, uint32_t id);
static void                destory(struct wl_resource* resource);

void wl::connection::add_protocol(wl::protocol* protocol)
{
    wl_global_create(display, protocol->interface(), protocol->version(), protocol, (wl_global_bind_func_t)bind);
}

void wl::connection::create_resource(wl::resource* resource, struct wl_client* client, int version, int id, void* data)
{
    resource->create(bind(client, resource, version, id), data);
}

static struct wl_resource* bind(struct wl_client* client, void* data, uint32_t version, uint32_t id)
{
    auto protocol = (wl::protocol*)data;

    struct wl_resource* resource = wl_resource_create(client, protocol->interface(), version, id);
    if (resource == nullptr)
    {
        wl_client_post_no_memory(client);
        return nullptr;
    }
    wl_resource_set_implementation(resource, protocol->functions(), data, &destory);
    protocol->bind(resource, client, data, version, id);
    return resource;
}

static void destory(struct wl_resource* resource)
{
    //auto protocol = (wl::protocol*)wl_resource_get_user_data(resource);
    //protocol->destory(resource);
    //wl_resource_destroy(resource);
}

void wl::connection::run()
{
    wl_display_run(display);
}

void wl::connection::protocol_logger(void* user_data, wl_protocol_logger_type direction,
                                     const wl_protocol_logger_message* message)
{}