#include "Renderer.h"

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}
 

ReturnStatus Renderer::RenderInit()
{
	if (!glfwInit())
	{
		return ReturnStatus::INIT_BAD;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(m_InitialWindowWidth, m_InitialWindowHeight, "OpenGL Square", NULL, 0);

	if (!window)
	{
		glfwTerminate();
		std::cerr << "glfwCreateWindow failed\n";
		return ReturnStatus::RENDER_INIT_FAIL;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		std::cerr << "glfwMakeContextCurrent failed\n";
		return ReturnStatus::RENDER_INIT_FAIL;
	}
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	m_ShaderProgram = glCreateProgram();
	
	
	std::vector<ShaderInfo> shaders;
	shaders.push_back({ "Shaders/VertexShader.glsl", GL_VERTEX_SHADER });
	shaders.push_back({ "Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER });
	if (!(LoadShadersFromFile(shaders) == ReturnStatus::LOAD_SHADERS_FROM_FILE_SUCCESS))
	{
			std::cerr << "Failed to Load Shaders from File\n";
			return ReturnStatus::LOAD_SHADERS_FROM_FILE_FAIL;
	}
	glLinkProgram(m_ShaderProgram);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	ObjectInfo object{ "Objects/Basic_shapes/triangle.txt", ObjectInfo:: TXT};

	/*
	if (!(LoadObjectFromFile(object) == ReturnStatus::LOAD_OBJECT_FROM_FILE_SUCCESS))
	{
		std::cerr << "Failed to Load Object from File " << object.m_Path << std::endl;
		return ReturnStatus::LOAD_OBJECT_FROM_FILE_FAIL;
	}*/
	GLfloat positions[]
	{
		0.5f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,
		-0.5f, 0.0f, 0.0f
	};
	GLfloat colors[]
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
	GLuint indices[]
	{
		0,1,2
	};

	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(1);

		glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(positions), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(positions), colors);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

	}

	glUseProgram(m_ShaderProgram);

	GLuint model_loc = glGetUniformLocation(m_ShaderProgram, "ModelMatrix");
	GLuint perspective_loc = glGetUniformLocation(m_ShaderProgram, "PerspectiveMatrix");
	GLuint view_loc = glGetUniformLocation(m_ShaderProgram, "ViewMatrix");
	glm::mat4 dummy = glm::identity<glm::mat4>();
	glm::mat4 translate = glm::translate(dummy, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 scale = glm::scale(dummy, glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 rotate = glm::rotate(dummy, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(4, 3, -3), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 perspective = glm::perspective(glm::radians(45.0f), static_cast<GLfloat>(m_InitialWindowWidth / m_InitialWindowHeight), 0.0f, 10000.0f);

	GLint error_code = glGetError();
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(perspective_loc, 1, GL_FALSE, glm::value_ptr(perspective));
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));

	glViewport(0, 0, m_InitialWindowWidth, m_InitialWindowHeight);

	glClearColor(0, 0, 0, 1.0);	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	

	error_code = glGetError();
	if(error_code != GL_NO_ERROR)
	std::cerr << "Error Occured\n";
	return ReturnStatus::RENDER_INIT_PASS;
}

ReturnStatus Renderer::LoadObjectFromFile(ObjectInfo& t_ObjectInfo)
{
	std::string content = "", line = "";
	std::vector<glm::vec3 > Positions;
	std::vector<glm::vec3 > Colors;
	std::vector<GLuint> indices;
	GLuint count = 0;

	try
	{
		std::ifstream FileReader(t_ObjectInfo.m_Path.c_str(), std::ios::binary | std::ios::in);
		if (!(FileReader.is_open()))
		{
			std::cerr << "Failed to Load File " << t_ObjectInfo.m_Path.c_str() << std::endl;
			return ReturnStatus::LOAD_OBJECT_FROM_FILE_FAIL;
		}

		while (!FileReader.eof())
		{
			std::getline(FileReader, line);

			std::string temp = "";
			std::istringstream Stream(line);
			GLfloat x, y, z;
			if (line.substr(0,2) == "v ")
			{
				while (Stream >> temp)
				{
						if (temp != "t")
						{
							Stream >> temp;
							x = atof(temp.c_str());
							Stream >> temp;
							y = atof(temp.c_str());
							Stream >> temp;
							z = atof(temp.c_str());
							Positions.push_back({ x,y,z });
							Colors.push_back({ 0.5f, 0.5f, 0.5f });
							indices.push_back(count++);
						}
				}
			}
		}

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(1);

		glBufferData(GL_ARRAY_BUFFER, sizeof(Positions.data()) + sizeof(Colors.data()), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Positions.data()), Positions.data());
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(Positions.data()), sizeof(Positions.data()), Colors.data());
	
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices.data()), indices.data(), GL_STATIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices.data()), indices.data());
		FileReader.close();
	}
	catch (std::exception e)
	{
		std::cerr << e.what() << std::endl;
		return ReturnStatus::LOAD_OBJECT_FROM_FILE_FAIL;
	};
	return ReturnStatus::LOAD_OBJECT_FROM_FILE_SUCCESS;
}

ReturnStatus Renderer::InitTextures()
{
	auto Size = sizeof(m_TextureBuffers);
	glGenBuffers(Size, m_TextureBuffers);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_TextureBuffers[0]);

	//glBufferData(GL_PIXEL_UNPACK_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);'
	return ReturnStatus::INIT_TEXTURES_FAIL;
}

void Renderer::Run()
{
		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Just Wanna enable  Depth Testing and Color...

			glDrawElements(GL_TRIANGLES,3, GL_UNSIGNED_INT, NULL);
			glfwSwapBuffers(window);

			glfwPollEvents();
		}
		glfwTerminate();
}

