#pragma once
#include <wayland-server.h>
#include <unordered_map>

class client
{
    inline static std::unordered_map<wl_client*, client*> clients;

    wl_client* id;
    pid_t      pid;

    client(wl_client* id)
        : id(id)
    {
        wl_client_get_credentials(id, &pid, nullptr, nullptr);
    }

public:
    static void create_new_client(struct wl_listener* listener, wl_client* id)
    {
        clients[id] = new client(id);
    }

    static client& get(wl_client* id)
    {
        return *clients[id];
    }
};