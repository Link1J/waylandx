#include "linux_dmabuf.hpp"
#include <egl_connection.hpp>
#include <libdrm/drm_fourcc.h>

const struct wl_interface* ::protocol::zwp::linux_dmabuf::interface() const noexcept
{
    return &zwp_linux_dmabuf_v1_interface;
}

int ::protocol::zwp::linux_dmabuf::version() const noexcept
{
    return 1;
}

void* ::protocol::zwp::linux_dmabuf::functions() const noexcept
{
    static struct zwp_linux_dmabuf_v1_interface interface = {
        .destroy       = &::protocol::zwp::linux_dmabuf::destroy,
        .create_params = &::protocol::zwp::linux_dmabuf::create_params,
    };
    return &interface;
}

void ::protocol::zwp::linux_dmabuf::bind(struct wl_resource* resource, struct wl_client* client, void* data,
                                         uint32_t version, uint32_t id) noexcept
{
    EGLint formatCount = 256, formats[256];
    eglQueryDmaBufFormatsEXT(egl::connection::get(), formatCount, formats, &formatCount);

    for (int a = 0; a < std::min(formatCount, 256); a++)
    {
        EGLint       modifierCount = 256;
        EGLuint64KHR modifiers[256];
        EGLBoolean   external[256];
        eglQueryDmaBufModifiersEXT(egl::connection::get(), formats[a], modifierCount, modifiers, external,
                                   &modifierCount);

        if (modifierCount == 0)
        {
            modifierCount = 1;
            modifiers[0]  = DRM_FORMAT_MOD_INVALID;
        }

        for (int b = 0; b < std::min(modifierCount, 256); b++)
        {
            if (version >= ZWP_LINUX_DMABUF_V1_MODIFIER_SINCE_VERSION)
            {
                uint32_t modifier_lo = modifiers[b] & 0xFFFFFFFF;
                uint32_t modifier_hi = modifiers[b] >> 32;
                zwp_linux_dmabuf_v1_send_modifier(resource, formats[a], modifier_hi, modifier_lo);
            }
            else
            {
                zwp_linux_dmabuf_v1_send_format(resource, formats[a]);
            }
        }
    }
}

void ::protocol::zwp::linux_dmabuf::destory(struct wl_resource* resource) noexcept
{}

void ::protocol::zwp::linux_dmabuf::destroy(struct wl_client* client, struct wl_resource* resource)
{}

void ::protocol::zwp::linux_dmabuf::create_params(struct wl_client* client, struct wl_resource* resource,
                                                  uint32_t params_id)
{}
