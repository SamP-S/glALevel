/* Definitions */
#define GLEW_STATIC
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

/* Include */
#include <iostream>
#include <GLFW/glfw3.h>

int main(void)
{
	GLFWwindow* window;

	/* Initialize GLFW */
	if (!glfwInit())
	{
		return -1;
	}

	/* Create a windowied mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll ofr and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}