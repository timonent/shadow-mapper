#include "App.hpp"
#include "Buffer.hpp"
#include "PerspectiveCamera.hpp"
#include "ResourceManager.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "TextureStorage.hpp"
#include "TextureView.hpp"
#include "common.hpp"
#include "constants.hpp"
#include "StbImageResource.hpp"
#include "util.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <array>
#include <bit>
#include <format>
#include <iostream>
#include <memory>
#include <ranges>
#include <vector>

//------------------------------------------------------------------------

struct Vertex
{
    glm::vec3 pos = glm::vec3();
    glm::vec3 nrm = glm::vec3();
    glm::vec2 tex = glm::vec2();
};

//------------------------------------------------------------------------

int main()
{
    using namespace Zhade;

    const auto app = App();

    app.init();
    {
        ResourceManager mngr;

        const auto shaderProgram = ShaderProgram(
            Shader<GL_VERTEX_SHADER>(common::shaderPath + "debug.vert"),
            Shader<GL_FRAGMENT_SHADER>(common::shaderPath + "debug.frag")
        );

        const auto camera = PerspectiveCamera(mngr, app);

        static constexpr auto numQuads = 4;

        // Basic vertex data setup.

        const Vertex quadVerts[] = {
            { glm::vec3( 0.5f,  0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
            { glm::vec3( 0.5f,  0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
            { glm::vec3(-0.5f,  0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
            { glm::vec3(-0.5f,  0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) }
        };
        const GLuint quadInds[] = {
            0, 1, 2,
            2, 3, 0
        };

        const auto vboHandle = mngr.createBuffer(GL_ARRAY_BUFFER);
        std::cout << vboHandle.m_index << "\n";
        const auto vbo = mngr.getBuffer(vboHandle);
        const auto eboHandle = mngr.createBuffer(GL_ELEMENT_ARRAY_BUFFER);
        std::cout << eboHandle.m_index << "\n";
        const auto ebo = mngr.getBuffer(eboHandle);

        GLuint vao;
        glCreateVertexArrays(1, &vao);
        glVertexArrayVertexBuffer(vao, 0, vbo->getHandle(), 0, sizeof(Vertex));
        glVertexArrayElementBuffer(vao, ebo->getHandle());

        glEnableVertexArrayAttrib(vao, 0);
        glEnableVertexArrayAttrib(vao, 1);
        glEnableVertexArrayAttrib(vao, 2);

        glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
        glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm));
        glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex));

        glVertexArrayAttribBinding(vao, 0, 0);
        glVertexArrayAttribBinding(vao, 1, 0);
        glVertexArrayAttribBinding(vao, 2, 0);

        /*auto quadVtxSpan = vbo->pushData<Vertex>(quadVerts, 4);
        auto quadIdxSpan = ebo->pushData<GLuint>(quadInds, 6);

        // Create render commands and gather model matrices.

        std::vector<glm::mat3x4> modelMatrices;
        std::vector<MultiDrawElementsIndirectCommand> cmds;

        for (auto i = 0u; i < numQuads; ++i)
        {
            modelMatrices.push_back(glm::mat3x4(glm::transpose(glm::translate(glm::vec3(0.0f, (float)i, 0.0f)))));
        }

        cmds.push_back({
            .vertexCount = 6,
            .instanceCount = numQuads,
            .firstIndex = 0,
            .baseVertex = 0,
            .baseInstance = 0
        });

        // Upload the model matrices into an SSBO.

        const auto ssboHandle = mngr.createBuffer(GL_SHADER_STORAGE_BUFFER, 1 << 10);
        auto ssbo = mngr.getBuffer(ssboHandle);
        auto x = ssbo->pushData<glm::mat3x4>(modelMatrices.data(), modelMatrices.size());
        ssbo->bindBase(constants::MODEL_BINDING);

        // Setup the indirect draw buffer and the draw IDs.

        const auto diboHandle = mngr.createBuffer(GL_DRAW_INDIRECT_BUFFER, 1 << 10);
        const auto dibo = mngr.getBuffer(diboHandle);
        auto y = dibo->pushData<MultiDrawElementsIndirectCommand>(cmds.data(), cmds.size());

        // Setup textures for the quads.

        const auto texStorage = TextureStorage();

        const auto& cataViewOpt = texStorage.pushDataFromFile(common::texturePath + "cataphract.jpg");
        const auto& berserkViewOpt = texStorage.pushDataFromFile(common::texturePath + "berserk.png");
        const auto& longbowViewOpt = texStorage.pushDataFromFile(common::texturePath + "longbowman.png");
        const auto& jagViewOpt = texStorage.pushDataFromFile(common::texturePath + "jaguarwarrior.png");

        texStorage.bindToUnit(0);
        texStorage.generateMipmap();

        shaderProgram.use();
        glBindVertexArray(vao);
        dibo->bind();

        while (!glfwWindowShouldClose(app.getGLCtx()))
        {
            app.updateInternalTimes();
            
            glfwPollEvents();

            camera.tick();

            //std::cout << 1.0f / app.getDeltaTime() << "\n";

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, cmds.size(), 0);

            glfwSwapBuffers(app.getGLCtx());
        }*/

        glBindVertexArray(0);
        glDeleteVertexArrays(1, &vao);
    }

    return 0;
}

//------------------------------------------------------------------------
