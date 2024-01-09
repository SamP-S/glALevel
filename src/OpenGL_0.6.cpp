/* Definitions */
#define GLEW_STATIC
#define WINDOW_WIDTH 1280									// xxxx
#define WINDOW_HEIGHT 720									// yyyy
#define WINDOW_FULLSCREEN NULL								// NULL || glfwGetPrimaryMonitor()7
#define WINDOW_POSX 50
#define WINDOW_POSY 50	

#define COLOUR_RED 1.0f, 0.0f, 0.0f, 1.0f
#define COLOUR_GREEN 0.0f, 1.0f, 0.0f, 1.0f
#define COLOUR_BLUE 0.0f, 0.0f, 1.0f, 1.0f
#define COLOUR_WHITE 1.0f, 1.0f, 1.0f, 1.0f
#define COLOUR_BLACK 0.0f, 0.0f, 0.0f, 1.0f

/* Include */
#include <iostream>
#include <chrono>
/* Must Define GLEW > GLFW */
#include <GLEW/include/GL/glew.h>
#include <GLFW/include/GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

/* Shaders */
// Vertex Shader
const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    in vec3 color;
    in vec2 texcoord;
    out vec3 Color;
    out vec2 Texcoord;
    uniform mat4 trans;
    void main()
    {
        Color = color;
        Texcoord = texcoord;
        gl_Position = trans * vec4(position, 0.0, 1.0);
    }
)glsl";
//Fragment Shader
const char* fragmentSource = R"glsl(
    #version 150 core
    in vec3 Color;
    in vec2 Texcoord;
    out vec4 outColor;
    uniform sampler2D tex;
    void main()
    {
        outColor = texture(tex, Texcoord) * vec4(Color, 1.0);
    }
)glsl";



int main(void)
{
	GLFWwindow* window;

	/* Initialize GLFW */
	std::cout << "ATTEMPTING: Initialization" << std::endl;
	if (!glfwInit())
	{
		std::cout << "FAILED: Initialization" << std::endl;
		return -1;
	}
	std::cout << "SUCCESS: Initialization" << std::endl;

	/* Gets video mode to set window dynamically */
	std::cout << "ATTEMPTING: Get Video Mode" << std::endl;
	std::cout << "SUCCESS: Video Mode" << std::endl;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", WINDOW_FULLSCREEN, NULL);
	std::cout << "ATTEMPTING: Window Creation" << std::endl;
	if (!window)
	{
		std::cout << "FAILED: Window Creation" << std::endl;
		glfwTerminate();
		return -1;
	}
	std::cout << "SUCCESS: Window Created" << std::endl;

	/* Set Window Position */
	std::cout << "ATTEMPTING: Set Window Position" << std::endl;
	glfwSetWindowPos(window, WINDOW_POSX, WINDOW_POSY);
	std::cout << "SUCCESS: Set Window Position" << std::endl;

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Initialize GLEW */
	glewExperimental = GL_TRUE;
	glewInit();

	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);

	GLfloat vertices[] = {
		//  Position			Color					Texcoords
			-0.5f,  0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f, // Top-left
			 0.5f,  0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 1.0f, // Top-right
			 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, // Bottom-right
			-0.5f, -0.5f,		1.0f, 1.0f, 1.0f,		0.0f, 0.0f  // Bottom-left
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create an element array
	GLuint ebo;
	glGenBuffers(1, &ebo);

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

	// Load texture
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	//// White image
	//int width = 200;
	//int height = 100;
	//const int total = width * height * 3;
	//float pixels[200 * 100 * 3];
	//for (int i = 0; i < total; i++)
	//{
	//	pixels[i] = 1.0f;
	//}
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	//// Black/white checkerboard
	//int width = 8;
	//int height = 8;
	//float pixels[] = {
	//	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f,
	//	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f,
	//	1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	//	1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f,
	//	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f,
	//	1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,
	//	1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f
	//};
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);


	int width, height;
	unsigned char* image = SOIL_load_image("img/Space3.jpeg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

#define WRAPPING GL_CLAMP_TO_EDGE

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WRAPPING);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WRAPPING);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	/* Timer */
	auto t_start = std::chrono::high_resolution_clock::now();

	// Specify the Uniform Translation Matrix Location
	GLint uniTrans = glGetUniformLocation(shaderProgram, "trans");

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(COLOUR_BLACK);		// Sets Background Colour to Black
		glClear(GL_COLOR_BUFFER_BIT);	// Clears Screen to Black

		/* Time */
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

		/* Translation Matrix */
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(
			trans,
			time * glm::radians(180.0f),
			glm::vec3(1.0f, 1.0f, 1.0f)
		);
		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));

		/* "MODE", "NUM_VERTICIES", "Type", "Indicies" */
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//std::cout << "EVENT: Rendering" << std::endl;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		//std::cout << "EVENT: Swapping Buffers" << std::endl;

		/* Poll ofr and process events */
		glfwPollEvents();
		//std::cout << "EVENT: Polling Events" << std::endl;
	}
	glDeleteTextures(1, &tex);
	glfwTerminate();
	return 0;
}