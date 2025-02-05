#pragma once

#include "Handle.hpp"
#include "Texture.hpp"
#include "common.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class ResourceManager;

struct FramebufferDescriptor
{
    TextureDescriptor textureDesc;
    GLenum attachment;
    ResourceManager* mngr;
    bool managed = true;
};

//------------------------------------------------------------------------

class Framebuffer
{
public:
    Framebuffer() = default;
    explicit Framebuffer(FramebufferDescriptor desc);
    ~Framebuffer();

    Framebuffer(const Framebuffer&) = default;
    Framebuffer& operator=(const Framebuffer&) = default;
    Framebuffer(Framebuffer&&) = delete;
    Framebuffer& operator=(Framebuffer&&) = delete;

    [[nodiscard]] GLuint name() { return m_name; }
    [[nodiscard]] Texture* texture();

    void bind(GLenum target = GL_FRAMEBUFFER) { glBindFramebuffer(target, m_name); }
    void freeResources();

private:

    GLuint m_name = 0;
    Handle<Texture> m_texture{};
    ResourceManager* m_mngr = nullptr;
    bool m_managed = true;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
