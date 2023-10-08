#pragma once

#include "Buffer.hpp"
#include "Handle.hpp"
#include "Model.hpp"
#include "ResourceManager.hpp"
#include "Texture.hpp"
#include "common.hpp"

#include <assimp/scene.h>
#include <robin_hood.h>

#include <atomic>
#include <filesystem>
#include <string>
#include <vector>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

struct SceneDescriptor
{
    ResourceManager* mngr;
    BufferDescriptor vertexBufferDesc{.byteSize = GIB_BYTES/2, .usage = BufferUsage::VERTEX};
    BufferDescriptor indexBufferDesc{.byteSize = GIB_BYTES/2, .usage = BufferUsage::INDEX};
};

//------------------------------------------------------------------------

class Scene
{
public:
    explicit Scene(SceneDescriptor desc);
    ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = delete;
    Scene& operator=(Scene&&) = delete;

    [[nodiscard]] std::span<Handle<Model2>> getModels() const noexcept { return m_models; }

    void addModelFromFile(const fs::path& path) const noexcept;
    [[nodiscard]] const Buffer* vertexBuffer() const noexcept { return m_mngr->get(m_vertexBuffer); }
    [[nodiscard]] const Buffer* indexBuffer() const noexcept { return m_mngr->get(m_indexBuffer); }

private:
    [[nodiscard]] Handle<Mesh> loadMesh(const aiScene* scene, const aiMesh* mesh, const fs::path& path,
        Model2* model) const noexcept;
    [[nodiscard]] std::span<Vertex> loadVertices(const aiMesh* mesh) const noexcept;
    [[nodiscard]] std::span<GLuint> loadIndices(const aiMesh* mesh) const noexcept;
    [[nodiscard]] Handle<Texture> loadTexture(const aiScene* scene, const aiMesh* mesh, aiTextureType textureType,
        const fs::path& dir) const noexcept;

    static inline std::atomic_uint32_t s_meshIdCounter = 0;

    ResourceManager* m_mngr;
    Handle<Buffer> m_vertexBuffer;
    Handle<Buffer> m_indexBuffer;
    Handle<Texture> m_defaultTexture;
    mutable std::vector<Handle<Model2>> m_models;
    mutable robin_hood::unordered_map<fs::path, Handle<Model2>> m_modelCache;
    mutable robin_hood::unordered_map<fs::path, Handle<Mesh>> m_meshCache;

    friend class IndirectRenderer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
