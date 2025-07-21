#include "Renderer.h"

void Renderer::SetUpForRendering()
{
    constexpr unsigned int MAX_NUM_VERTICES = 1000000;
    unsigned int m_Width = 1920;
    unsigned int m_Height = 1080;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glCreateBuffers(1, &m_VertexBuffer);
  
    unsigned int vertex_buffer_size = sizeof(Vertex) * MAX_NUM_VERTICES;
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glNamedBufferData(m_VertexBuffer, vertex_buffer_size, nullptr, GL_DYNAMIC_DRAW);

    unsigned int index_buffer_size = sizeof(unsigned int) * MAX_NUM_VERTICES;
    glCreateBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glNamedBufferData(m_IndexBuffer, index_buffer_size, nullptr, GL_DYNAMIC_DRAW);

    GLuint m_Vao = 0;
    glGenVertexArrays(1, &m_Vao);
    glBindVertexArray(m_Vao);

    GLsizei stride = sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec1) + sizeof(glm::vec3);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec1)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    // CREATE THE MATERIALS
    basicRenderProgram = glCreateProgram();
    CreateRenderingProgram(m_ActiveScene->m_Shaders[0], basicRenderProgram);
    glLinkProgram(basicRenderProgram);
}


void Renderer::CreateRenderingProgram(Shader& shader_resource, GLuint program)
{
    shader_resource.OnInit();
    for (size_t k = 0; k < shader_resource.m_Info.size(); k++)
    {
        GLenum shader_type = shader_resource.m_Info[k].ShaderType;
        auto shader = glCreateShader(shader_type);

        auto shader_source = shader_resource.m_ShaderSource[k].c_str();
        glShaderSource(shader, 1, &shader_source, NULL);
        glCompileShader(shader);

        glAttachShader(program, shader);
    }
}

void Renderer::UploadToOpenGL()
{
    GLbitfield flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    glClear(flags);
    glUseProgram(basicRenderProgram);

    auto view = m_pActiveCamera->GetV();

    GLuint viewLocation = glGetUniformLocation(basicRenderProgram, "View");
    GLuint posLocation = glGetUniformLocation(basicRenderProgram, "pos");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
    GLuint offset = 0;
    GLuint Ioffset = 0;
    for (auto& ch : m_ActiveScene->m_Characters)
    {
        Character var{ ch, m_ActiveScene.get()};
       
        auto& phzx = var.GetComponent<physics::PhysicsState>();
        glUniform3fv(posLocation, 3, glm::value_ptr(phzx.position));
        auto& mesh = var.GetComponent<Mesh>();
        glNamedBufferSubData(m_VertexBuffer, offset, sizeof(Vertex) * mesh.m_V.size(), mesh.m_V.data());
        offset += sizeof(Vertex) * mesh.m_V.size();

        glDrawArrays(GL_TRIANGLES, Ioffset, mesh.m_V.size());
        Ioffset += mesh.m_V.size();

    }
}

void Renderer::CreateOpenGLTexture(_TextureView& view, _TextureDescription& desc, GL_Texture& tex)
{    
    glCreateTextures(GL_TEXTURE_2D, 1, &tex);

    GLenum format = GL_RGB8;

    GLuint width = 10, height = 10;
    void* data = nullptr;
  
    glTextureStorage2D(tex, 1, format, width, height);

    glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureSubImage2D(tex, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

}

void Renderer::OnRender()
{
    UploadToOpenGL();
}
void Renderer::BindScene(std::shared_ptr<Scene> scene)
{
    if (m_ActiveScene)
        m_ActiveScene.reset();
    m_ActiveScene = scene;
}

void Renderer::SetActiveCamera(std::shared_ptr<Camera> camera)
{
    m_pActiveCamera = camera;
}

void Renderer::OnUpdate(TimeStep ts)
{
}

void Renderer::CreateImage()
{
}

void Renderer::EnableTesselation()
{
     float patch_inner[2] = { 5.0, 5.0 };
     float patch_outer[4] = { 6.0, 5.0, 8.0, 5.0 };
     glPatchParameteri(GL_PATCH_VERTICES, 36);
     glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, patch_inner);
     glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, patch_outer);
     glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
}

void Renderer::BeginFrame()
{
   
}

void Renderer::EndFrame()
{
    glFlush();
}
