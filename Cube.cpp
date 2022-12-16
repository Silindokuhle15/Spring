#include "Cube.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int Cube::LoadTextureFromFile(const char* file_path)
{
    int width, height, channels;

    if (!fopen(file_path, "r"))
    {
        std::cerr << "Texture File Path not valid " << std::endl;
        return -1;
    }

    unsigned char * data = stbi_load(file_path, &width, &height, &channels, 4);
    // Now work on the sampler
 
    glGenBuffers(1, &m_TexBuffer);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_TexBuffer);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_Tex1);
    //glGenTextures(1, &m_Tex1);
    glBindTexture(GL_TEXTURE_2D, m_Tex1);
    GLenum format = GL_RGB;
    
    glTextureStorage2D(m_Tex1, 1, format, width, height);
    //glTexStorage2D(GL_TEXTURE_2D, 0, format, width, height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, NULL);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //glTexParameteri(GL_TEXTURE_2D, GL_UNPACK_ALIGNMENT, 1);

    glGenSamplers(1, &m_Sampler);
    glBindSampler(1, GL_SAMPLER_2D);
    //glSamplerParameteri(m_Sampler, GL_UNPACK_ALIGNMENT, 1);

    return 0;
}

void Cube::OnInit()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    //ob.LoadObject("Objects/Crate/Crate1.obj");
    ob.LoadObject("Objects/Cube.obj");
    ob.m_VertexIndices.shrink_to_fit();

    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    
    size_t size = sizeof(struct PerVertex) * ob.m_VertexIndices.size();
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
        
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * ob.m_Positions.size(), ob.m_Positions.data());

    size = sizeof(glm::vec3) * ob.m_Positions.size();
    glBufferSubData(GL_ARRAY_BUFFER, size, sizeof(glm::vec2) * ob.m_TexCoords.size(), ob.m_TexCoords.data());

    size = sizeof(glm::vec3) * ob.m_Positions.size() + sizeof(glm::vec2) * ob.m_TexCoords.size();
    glBufferSubData(GL_ARRAY_BUFFER, size, sizeof(glm::vec3) * ob.m_Normals.size(), ob.m_Normals.data());

    EnableTesselation();
    create_shaders();

    glGenBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ob.m_VertexIndices.size(), ob.m_VertexIndices.data(), GL_STATIC_DRAW);

    m_VPOSlocation = glGetAttribLocation(m_ShaderPrograms[0], "Position");
    m_VTexlocation = glGetAttribLocation(m_ShaderPrograms[0], "TexCoord");
    m_VNORMlocation = glGetAttribLocation(m_ShaderPrograms[0], "Normal");
    //LoadTexture("Textures/crate_1.jpg"); 

    glVertexAttribPointer(m_VPOSlocation, 3, GL_FLOAT, GL_FALSE,
        sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(m_VPOSlocation);

    glVertexAttribPointer(m_VTexlocation, 2, GL_FLOAT, GL_FALSE,
        sizeof(glm::vec2), (void*)(sizeof(glm::vec3)));
    //glEnableVertexAttribArray(m_VTexlocation);

    size = sizeof(glm::vec3); //+ sizeof(glm::vec2);
    glVertexAttribPointer(m_VNORMlocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(size));
    glEnableVertexAttribArray(m_VNORMlocation);



    int m_Width  = BaseApplication::cam_ptr->GetWidth();
    int m_Height = BaseApplication::cam_ptr->GetHeight();

    glViewport(0, 0, m_Width, m_Height);
    glEnable(GL_DEPTH_BUFFER_BIT);
    glDepthRange(.010f, 1000.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Cube::OnUpdate()
{
}

void Cube::OnRender()
{
    
    glUseProgram(m_Program);
    m_MVPlocation = glGetUniformLocation(m_Program, "MVP");
    m_MVlocation = glGetUniformLocation(m_Program, "MV");

    m_PointLightLocation = glGetUniformLocation(m_Program, "LightPosition");
    m_AmbientColorLocation = glGetUniformLocation(m_Program, "AmbientColor");

    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(m_MVPlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetMVP()));
    glUniformMatrix4fv(m_MVlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetMV()));
    glUniform3fv(m_PointLightLocation, 1, glm::value_ptr(BaseApplication::m_PointLight->GetPostion()));
    glUniform4fv(m_AmbientColorLocation, 1, AmbientColor);

    ob.m_VertexIndices.shrink_to_fit();

    glDrawElements(GL_TRIANGLE_STRIP, ob.m_VertexIndices.size(), GL_UNSIGNED_INT, NULL);

    ImGui::Begin("Cube");
    ImGui::ColorEdit4("Ambient Color", AmbientColor);
    ImGui::End();

}



void Cube::OnDestroy(){ }

void Cube::create_shaders()
{
    struct ShaderInfo
    {
        const char* FilePath;
        GLenum ShaderType;
    };
    ShaderInfo baseShaders[]
    {
        {"Shaders/Vertexshader.glsl", GL_VERTEX_SHADER},
        {"Shaders/Fragmentshader.glsl", GL_FRAGMENT_SHADER},
        {"Shaders/TessellationControlShader.glsl", GL_TESS_CONTROL_SHADER},
        {"Shaders/TessellationEvaluationShader.glsl", GL_TESS_EVALUATION_SHADER},
        {"Shaders/GeometryShader.glsl", GL_GEOMETRY_SHADER}
    };
    //Tryna Refactor this entire Functuion
    std::ifstream is;
    std::string file_data;

    for(int i =0; i < 5; i++)   // Must Update this not to use the hard-coded value for the number of shaders
    {
        const char* file_path = baseShaders[i].FilePath;

        is = std::ifstream(file_path);
        is.seekg(0, std::ios::end);
        file_data.reserve(is.tellg());
        is.seekg(0, std::ios::beg);
        file_data.assign(std::istreambuf_iterator<char>(is),
            std::istreambuf_iterator<char>());

        const char * file_data_text = file_data.c_str();
        //file_data.clear();

        GLenum shader_type = baseShaders[i].ShaderType;
        m_Shaders[i] = glCreateShader(shader_type);
        glShaderSource(m_Shaders[i], 1, &file_data_text, NULL);
        glCompileShader(m_Shaders[i]);

        m_ShaderPrograms[i] = glCreateProgram();
        glAttachShader(m_ShaderPrograms[i], m_Shaders[i]);
        glProgramParameteri(m_ShaderPrograms[i], GL_PROGRAM_SEPARABLE, GL_TRUE);

        glLinkProgram(m_ShaderPrograms[i]);

        //glDeleteShader(m_Shaders[i]);
        //glDeleteProgram(m_ShaderPrograms[i]);
    }

    //glGenProgramPipelines(1, &m_ProgramPipeLine);
    //glUseProgramStages(m_ProgramPipeLine, GL_VERTEX_SHADER_BIT, m_VertexShaderProgram);
    //glUseProgramStages(m_ProgramPipeLine, GL_FRAGMENT_SHADER_BIT, m_FragmentShaderProgram);
    //glUseProgramStages(m_ProgramPipeLine, GL_TESS_CONTROL_SHADER_BIT, m_TessControlShaderProgram);
    //glUseProgramStages(m_ProgramPipeLine, GL_TESS_EVALUATION_SHADER_BIT, m_TessEvaluationShaderProgram);

    //glBindProgramPipeline(m_ProgramPipeLine);

    // Now for the Singular Rendering Program
    m_Program = glCreateProgram();
    glAttachShader(m_Program, m_Shaders[0]);
    glAttachShader(m_Program, m_Shaders[1]);
    //glAttachShader(m_Program, m_Shaders[2]);
    //glAttachShader(m_Program, m_Shaders[3]);
    //glAttachShader(m_Program, m_Shaders[4]);

    glLinkProgram(m_Program);
}

void Cube::LoadTexture(const char* file_path)
{
    int ret = LoadTextureFromFile(file_path);
}

void Cube::EnableTesselation()
{
    float patch_inner[2] = { 5.0, 5.0 };
    float patch_outer[4] = { 6.0, 5.0, 8.0, 5.0 };
    glPatchParameteri(GL_PATCH_VERTICES,36);
    glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, patch_inner);
    glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, patch_outer);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
}