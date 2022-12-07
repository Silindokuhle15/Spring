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
    ob.LoadObject("Objects/Crate/Crate1.obj");
    ob.m_VertexIndices.shrink_to_fit();
    ob.m_Positions.shrink_to_fit();
    ob.m_Vertices.shrink_to_fit();

    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    
    size_t size = sizeof(struct PerVertex) * ob.m_Vertices.size();
    glBufferData(GL_ARRAY_BUFFER, size, ob.m_Vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ob.m_VertexIndices.size(), ob.m_VertexIndices.data(), GL_STATIC_DRAW);

    create_shaders();

    m_VPOSlocation = glGetAttribLocation(m_VertexShaderProgram, "Position");
    m_VTexlocation = glGetAttribLocation(m_VertexShaderProgram, "TexCoord");
    //m_VNORMlocation = glGetAttribLocation(m_Program, "Normal");
    LoadTexture("Textures/crate_1.jpg"); 

    /*
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glEnableVertexArrayAttrib(m_VAO, m_VPOSlocation);
    glVertexArrayAttribFormat(m_VAO, m_VPOSlocation, 3, GL_FLOAT, GL_FALSE, (unsigned int)0);

    glEnableVertexArrayAttrib(m_VAO, m_VTexlocation);
    glVertexArrayAttribFormat(m_VAO, m_VTexlocation, 2, GL_FLOAT, GL_FALSE, (unsigned int)sizeof(glm::vec3));

    */
    
    EnableTesselation();
    glEnableVertexAttribArray(m_VPOSlocation);
    glVertexAttribPointer(m_VPOSlocation, 3, GL_FLOAT, GL_FALSE,
        sizeof(glm::vec3), (void*)0);

    glEnableVertexAttribArray(m_VTexlocation);
    glVertexAttribPointer(m_VTexlocation, 2, GL_FLOAT, GL_FALSE, 
        sizeof(glm::vec2), (void*)(sizeof(glm::vec3)));

    int m_Width  = BaseApplication::cam_ptr->GetWidth();
    int m_Height = BaseApplication::cam_ptr->GetHeight();

    glViewport(0, 0, m_Width, m_Height);
    glEnable(GL_DEPTH_BUFFER_BIT);
    glDepthRange(.1f, 1000.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Cube::OnUpdate()
{
}

void Cube::OnRender()
{
    m_MVPlocation = glGetUniformLocation(m_VertexShaderProgram, "MVP");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_Program);
    glUniformMatrix4fv(m_MVPlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetMVP()));
    ob.m_VertexIndices.shrink_to_fit();
    //glDrawElements(GL_TRIANGLE_STRIP, ob.m_VertexIndices.size(), GL_UNSIGNED_INT, NULL);
    glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, NULL);
}

void Cube::OnDestroy(){ }

void Cube::create_shaders()
{
    std::ifstream is("Shaders/Vertexshader.glsl");
    std::string vertex_file;
    std::string fragment_file;
    std::string control_file;
    std::string evaluation_file;
    
    is.seekg(0, std::ios::end);

    vertex_file.reserve(is.tellg());

    is.seekg(0, std::ios::beg);

    vertex_file.assign(std::istreambuf_iterator<char>(is), 
    std::istreambuf_iterator<char>());

    is = std::ifstream("Shaders/FragmentShader.glsl");

    is.seekg(0, std::ios::end);

    fragment_file.reserve(is.tellg());

    is.seekg(0, std::ios::beg);

    fragment_file.assign(std::istreambuf_iterator<char>(is),
        std::istreambuf_iterator<char>());

    {
        is = std::ifstream("Shaders/TessellationControlShader.glsl");

        is.seekg(0, std::ios::end);

        control_file.reserve(is.tellg());

        is.seekg(0, std::ios::beg);

        control_file.assign(std::istreambuf_iterator<char>(is),
            std::istreambuf_iterator<char>());

        const char* control_shader_text = control_file.c_str();

        m_TessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(m_TessControlShader, 1, &control_shader_text, NULL);
        glCompileShader(m_TessControlShader);

        m_TessControlShaderProgram = glCreateProgram();
        glAttachShader(m_TessControlShaderProgram, m_TessControlShader);
        glProgramParameteri(m_TessControlShaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);

        glLinkProgram(m_TessControlShaderProgram);

    } 
    {
        is = std::ifstream("Shaders/TessellationEvaluationShader.glsl");

        is.seekg(0, std::ios::end);

        evaluation_file.reserve(is.tellg());

        is.seekg(0, std::ios::beg);

        evaluation_file.assign(std::istreambuf_iterator<char>(is),
            std::istreambuf_iterator<char>());

        const char* evaluation_shader_text = evaluation_file.c_str();

        m_TessEvaluationShader = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(m_TessEvaluationShader, 1, &evaluation_shader_text, NULL);
        glCompileShader(m_TessEvaluationShader);

        m_TessEvaluationShaderProgram = glCreateProgram();
        glAttachShader(m_TessEvaluationShaderProgram, m_TessEvaluationShader);
        glProgramParameteri(m_TessEvaluationShaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);

        glLinkProgram(m_TessEvaluationShaderProgram);

    }

    const char* vertex_shader_text = vertex_file.c_str();
    const char* fragment_shader_text = fragment_file.c_str();

    m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_VertexShader, 1, &vertex_shader_text, NULL);
    glCompileShader(m_VertexShader);

    m_VertexShaderProgram = glCreateProgram();
    glAttachShader(m_VertexShaderProgram, m_VertexShader);
    glProgramParameteri(m_VertexShaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);

    glLinkProgram(m_VertexShaderProgram);

    m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_FragmentShader, 1, &fragment_shader_text, NULL);
    glCompileShader(m_FragmentShader);

    m_FragmentShaderProgram = glCreateProgram();
    glAttachShader(m_FragmentShaderProgram, m_FragmentShader);
    glProgramParameteri(m_FragmentShaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);

    glLinkProgram(m_FragmentShaderProgram);

    m_Program = glCreateProgram();
    glAttachShader(m_Program, m_VertexShader);
    glAttachShader(m_Program, m_FragmentShader);
    //glAttachShader(m_Program, m_TessControlShader);
    //glAttachShader(m_Program, m_TessEvaluationShader);

    glGenProgramPipelines(1, &m_ProgramPipeLine);
    glUseProgramStages(m_ProgramPipeLine, GL_VERTEX_SHADER_BIT, m_VertexShaderProgram);
    glUseProgramStages(m_ProgramPipeLine, GL_FRAGMENT_SHADER_BIT, m_FragmentShaderProgram);
    glUseProgramStages(m_ProgramPipeLine, GL_TESS_CONTROL_SHADER_BIT, m_TessControlShaderProgram);
    glUseProgramStages(m_ProgramPipeLine, GL_TESS_EVALUATION_SHADER_BIT, m_TessEvaluationShaderProgram);

    glBindProgramPipeline(m_ProgramPipeLine);

    glLinkProgram(m_Program);
}

void Cube::LoadTexture(const char* file_path)
{
    int ret = LoadTextureFromFile(file_path);
}


void Cube::EnableTesselation()
{
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
}