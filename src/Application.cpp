/* Definitions */
#define WINDOW_WIDTH 1280									// xxxx
#define WINDOW_HEIGHT 720									// yyyy
#define WINDOW_FULLSCREEN NULL								// NULL || glfwGetPrimaryMonitor()
#define WINDOW_POSX 50
#define WINDOW_POSY 50	

/* Include */
#define _CRT_SECURE_NO_WARNINGS
#define GL_LITE_IMPLEMENTATION
#include "OpenGL_Loader.h"
#include "glfw3.h"
#include "Camera.h"
#include "3D_Math.h"
#include "Shader.h"
#include "Timer.h"
#include "Texture_2D.h"
#include "VertexData.h"
#include "obj_Reader.h"
#include "Coloursf.h"
#include "FrameRate.h"
#include <iostream>
#include <chrono>

Camera CameraClass;
Shader ShaderClass;
Timer TimerClass;
Texture TextureClass;
Obj ObjClass;
FrameRate FrameRateClass;



// Vertex Source
const GLchar* vertexSource = R"glsl(
    #version 150 core
	
    in vec3 position;
    in vec3 color;
	in vec2 texcoord;

    out vec3 Color;
	out vec2 Texcoord;

	uniform mat4 matrix;
	uniform mat4 persp;
	uniform mat4 view;

    void main()
    {
        Color = color;
        gl_Position = matrix * persp * view * vec4(position, 1.0);
		Texcoord = texcoord;
    }
)glsl";

// Fragment Source
const GLchar* fragmentSource = R"glsl(
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

double previous_x = 0.0f;
double previous_y = 0.0f;

void ErrorCheck()
{
	GLenum error;

	error = glGetError();

	if (error != GL_NO_ERROR)
	{
		std::cout << "Not Working" << std::endl;
	}

	if (error == GL_INVALID_OPERATION)
	{
		std::cout << "INVALID OPERATION" << std::endl;
	}
	if (error == GL_NO_ERROR)
	{
		std::cout << "No Error! -> CODE CHECKING <-" << std::endl;
	}

	if (error == GL_INVALID_VALUE)
	{
		std::cout << "NO VALUE" << std::endl;
	}

	if (error == GL_OUT_OF_MEMORY)
	{
		std::cout << "NO MEMORY" << std::endl;
	}

	if (error == GL_STACK_OVERFLOW)
	{
		std::cout << "NO OVERFLOW" << std::endl;
	}

	if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
	{
		std::cout << "Invalid Framebuffer <-" << std::endl;
	}

	if (error == GL_OUT_OF_MEMORY)
	{
		std::cout << "Out of Memory" << std::endl;
	}
}

static void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			std::cout << "Mouse1 Pressed" << std::endl;
		}
		else if (action == GLFW_RELEASE)
		{
			std::cout << "Mouse1 Released" << std::endl;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			std::cout << "Mouse2 Pressed" << std::endl;
		}
		else if (action == GLFW_RELEASE)
		{
			std::cout << "Mouse2 Released" << std::endl;
		}
	}
}

static void cursorPositionCallBack(GLFWwindow* window, double x, double y)
{
	double dx = x - previous_x;
	double dy = -(y - previous_y);

	std::cout << "X: " << dx << "	Y: " << dy << std::endl;

	CameraClass.moveCameraOrientation(dx, dy);

	previous_x = x;
	previous_y = y;

}

int main()
{
	ShaderClass.VertexSource = vertexSource;
	ShaderClass.FragmentSource = fragmentSource;

	TimerClass.StartTimer();

	GLFWwindow* window;

	/* Initialize GLFW */
	if (!glfwInit())
	{
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", WINDOW_FULLSCREEN, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Set Window Position */
	glfwSetWindowPos(window, WINDOW_POSX, WINDOW_POSY);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Set Mouse Control */
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* OpenGL Function Loader */
	gl_lite_init();

	/* Obj File Loader */
	ObjClass.ReadOBJ("C:/suzanne_merge.obj");

	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERT_ARRAY), VERT_ARRAY, GL_STATIC_DRAW);

	/* Shader Prorgam */
	ShaderClass.SetupShader();
	GLuint shaderProgram = ShaderClass.ShaderProgramUint;

	/* Attributes */
	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glActiveTexture(GL_TEXTURE0);

	/* Camera Setup */
	CameraClass.setProjectionMatrix(WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 10000.0f);
	CameraClass.updateCamera();
	CameraClass.print();


	/* Uniforms */
	GLint uniMatrix = glGetUniformLocation(shaderProgram, "matrix");
	glUniformMatrix4fv(uniMatrix, 1, GL_FALSE, &CameraClass.modelTransformationMatrix.m[0][0]);
	//m4_print(CameraClass.matrix);
	std::cout << "&Matrix[0][0]: " << &CameraClass.modelTransformationMatrix.m[0][0] << std::endl;
	ErrorCheck();

	GLint uniPersp = glGetUniformLocation(shaderProgram, "persp");
	glUniformMatrix4fv(uniPersp, 1, GL_FALSE, &CameraClass.perspectiveProjectionMatrix.m[0][0]);
	//m4_print(CameraClass.matrix);
	std::cout << "&Matrix[0][0]: " << &CameraClass.perspectiveProjectionMatrix.m[0][0] << std::endl;
	ErrorCheck();

	GLint uniView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, &CameraClass.cameraViewMatrix.m[0][0]);
	//m4_print(CameraClass.matrix);
	std::cout << "&Matrix[0][0]: " << &CameraClass.cameraViewMatrix.m[0][0] << std::endl;
	ErrorCheck();

	/* Texture */
	TextureClass.GenerateEmptyTexture();
	TextureClass.SetTexture(TextureClass.whiteTexture);
	TextureClass.SetFiltering(GL_NEAREST);
	TextureClass.SetTextureWrapping(GL_REPEAT);

	ErrorCheck();

	/* OpenGL Rendering Properties */
	glEnable(GL_CULL_FACE);		// Enables culling to allows triangles to be removed at either back or front to not be rendered
	glCullFace(GL_BACK);		// Sets culling mode to remove all back triangles
	glFrontFace(GL_CCW);		// Front Triangles have their points ordered counter clock-wise

	/* Set Mouse Input */
	glfwSetCursorPos(window, 0, 0);
	glfwSetMouseButtonCallback(window, mouseButtonCallBack);
	glfwSetCursorPosCallback(window, cursorPositionCallBack);

	/* Frame Rate Counter */
	FrameRateClass.start();

	bool close = false;
	while (!glfwWindowShouldClose(window) && !close)
	{
		/* Render here */
		// Clear the screen to black
		glClearColor(RGBA_BLACK);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw (type, first pos, no verticies)
		glDrawArrays(DRAW_MODE, 0, VERT_POINTS);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll ofr and process events */
		glfwPollEvents();

		/* Input Management */
		int state;

		// W
		state = glfwGetKey(window, GLFW_KEY_W);
		if (state == GLFW_PRESS)
		{
			std::cout << "W";
			CameraClass.moveCameraPosition(1, 0, 0);
		}

		// S
		state = glfwGetKey(window, GLFW_KEY_S);
		if (state == GLFW_PRESS)
		{
			std::cout << "S";
			CameraClass.moveCameraPosition(-1, 0, 0);
		}

		// A
		state = glfwGetKey(window, GLFW_KEY_A);
		if (state == GLFW_PRESS)
		{
			std::cout << "A";
			CameraClass.moveCameraPosition(0, -1, 0);
		}

		// D
		state = glfwGetKey(window, GLFW_KEY_D);
		if (state == GLFW_PRESS)
		{
			std::cout << "D";
			CameraClass.moveCameraPosition(0, 1, 0);
		}

		// SPACE
		state = glfwGetKey(window, GLFW_KEY_SPACE);
		if (state == GLFW_PRESS)
		{
			std::cout << "_";
			CameraClass.moveCameraPosition(0, 0, 1);
		}

		// CTRL
		state = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
		if (state == GLFW_PRESS)
		{
			std::cout << "^";
			CameraClass.moveCameraPosition(0, 0, -1);
		}

		// TAB
		state = glfwGetKey(window, GLFW_KEY_TAB);
		if (state == GLFW_PRESS)
		{
			std::cout << "TAB";
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			if (state == GLFW_RELEASE)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}

		// ESC
		state = glfwGetKey(window, GLFW_KEY_ESCAPE);
		if (state == GLFW_PRESS)
		{
			std::cout << "ESC";
			close = true;
		}

		/* Move Camera */
		CameraClass.updateCamera();
		glUniformMatrix4fv(uniMatrix, 1, GL_FALSE, &CameraClass.modelTransformationMatrix.m[0][0]);
		glUniformMatrix4fv(uniPersp, 1, GL_FALSE, &CameraClass.perspectiveProjectionMatrix.m[0][0]);
		glUniformMatrix4fv(uniView, 1, GL_FALSE, &CameraClass.cameraViewMatrix.m[0][0]);

		/* Frame Rate */
		/*FrameRateClass.frame();
		if (FrameRateClass.frame_num % 60 == 0)
		{
			std::cout << FrameRateClass.avg_rate << std::endl;
		}*/
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	TextureClass.DeleteTexure();
	ShaderClass.DeleteShaderResources();

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	glfwTerminate();

	return 0;
}
