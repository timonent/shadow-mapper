#pragma once

#include "Buffer.hpp"
#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "Handle.hpp"
#include "Pipeline.hpp"
#include "ResourceManager.hpp"
#include "Scene.hpp"

#include <optional>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

struct RendererDescriptor
{
    ResourceManager* mngr;
    SceneDescriptor sceneDesc;
    CameraDescriptor cameraDesc;
    BufferDescriptor commandBufferDesc{
        .byteSize = MAX_DRAWS * sizeof(DrawElementsIndirectCommand),
        .usage = BufferUsage::INDIRECT
    };
    BufferDescriptor drawMetadataBuffer{
        .byteSize = MAX_DRAWS * sizeof(DrawMetadata),
        .usage = BufferUsage::STORAGE
    };
    PipelineDescriptor mainPassDesc;
};

//------------------------------------------------------------------------

class Renderer
{
public:
    explicit Renderer(RendererDescriptor desc);
    ~Renderer();

    [[nodiscard]] Camera<CameraType::PERSPECTIVE>& camera() { return m_camera; }
    [[nodiscard]] Scene& scene() { return m_scene; }

    void render();

private:
    [[nodiscard]] Buffer* buffer(const Handle<Buffer>& handle) { return m_mngr->get(handle); }
    [[nodiscard]] Pipeline* pipeline() { return m_mngr->get(m_pipeline); }

    void setupVAO();
    void setupBuffers(const RendererDescriptor& desc);
    void setupCamera(CameraDescriptor cameraDesc);
    void setupPipeline(PipelineDescriptor mainPassDesc);
    void populateBuffers();
    void clearDrawCounter();

    ResourceManager* m_mngr;
    Scene m_scene;
    GLuint m_vao;
    Camera<CameraType::PERSPECTIVE> m_camera;
    Handle<Buffer> m_commandBuffer;
    Handle<Buffer> m_drawMetadataBuffer;
    Handle<Buffer> m_atomicDrawCounterBuffer;
    Handle<Buffer> m_viewProjUniformBuffer;
    Handle<Pipeline> m_pipeline;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
