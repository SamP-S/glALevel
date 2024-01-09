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
/* Must Define GLEW > GLFW */
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

	/* Check GLEW Correct == 1 */
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(COLOUR_BLACK);
		glClear(GL_COLOR_BUFFER_BIT);

		/* Creates a Float array of a triangle*/
		float vertices[] = 
		{
		0.0f,  0.5f, // Vertex 1 (X, Y)
		0.5f, -0.5f, // Vertex 2 (X, Y)
		-0.5f, -0.5f  // Vertex 3 (X, Y)
		};

		//std::cout << "EVENT: Rendering" << std::endl;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		//std::cout << "EVENT: Swapping Buffers" << std::endl;

		/* Poll ofr and process events */
		glfwPollEvents();
		//std::cout << "EVENT: Polling Events" << std::endl;
	}

	glfwTerminate();
	return 0;
}