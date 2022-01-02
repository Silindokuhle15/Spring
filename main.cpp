#include <GL/glew.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

unsigned vao;
unsigned int shader_program;

const char* vertex_shader_source =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(position, 1.0);\n"
"    vertexColor = color;\n"
"}\n";

const char* fragment_shader_source =
"#version 330 core\n"
"out vec4 fragColor;\n"
"in vec3 vertexColor;\n"
"void main()\n"
"{\n"
"     fragColor = vec4(vertexColor, 1.0f);\n"
"}\n";

void setup_shader()
{
	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void setup_data()
{
	//float vertices[] = {
		// For visualization: each row is a vertex.
		// Each vertex has position [x, y, z] and color [r, g, b]
		//-0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0,   // red color for this vertex
		//0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.0,   // green color
		// 0.5f,  0.5f, 0.0f, 0.0, 0.0, 1.0,   // blue color for our top vertex
		//-0.5f,  0.5f, 0.0f, 1.0, 0.0, 0.0,	 // white color 
		//-0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0   // red color for this vertex

		float vertices[] = {
		-0.5f,  0.5f, 0.0f, 1.0, 0.0, 0.0,  // top left point
		 0.5f,  0.5f, 0.0f, 0.0, 1.0, 0.0,  // top right point
		 0.5f, -0.5f, 0.0f, 0.0, 0.0, 1.0,  // bottom right point
		 0.5f, -0.5f, 0.0f, 0.0, 0.0, 1.0,  // bottom right point
		-0.5f, -0.5f, 0.0f, 1.0, 1.0, 1.0,  // bottom left point
		-0.5f,  0.5f, 0.0f, 1.0, 0.0, 0.0   // top left point
	};
	//};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Square", NULL, 0);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return -1;
	}

	setup_shader();
	setup_data();

	GLuint indices[] =
	{
		0,1,2,0,1
	};
	unsigned int indexBuffer = 0;
	glGenBuffers(1, &indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), static_cast<const void*>(indices), GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);
		glBindVertexArray(vao);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);


		//glDrawElements(GL_TRIANGLE_STRIP, 2, GL_UNSIGNED_SHORT, static_cast<void*>(indices));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}