/* Definitions */
#define WINDOW_WIDTH 1600									// xxxx
#define WINDOW_HEIGHT 900									// yyyy
#define WINDOW_FULLSCREEN NULL								// NULL || glfwGetPrimaryMonitor()
#define WINDOW_POSX 50
#define WINDOW_POSY 50	

/* Include */
#define _CRT_SECURE_NO_WARNINGS
#define GL_LITE_IMPLEMENTATION
#include "OpenGL_Loader.h"
#include "glfw3.h"
#include "Error.h"
#include "Camera.h"
#include "3D_Math.h"
#include "Shader.h"
#include "Texture_2D.h"
#include "Timer.h"
#include "VertexData.h"
#include "obj_Reader.h"
#include "Coloursf.h"
#include "Sources.h"
#include "Objects.h"
#include "BMP.h"
#include <iostream>
#include <chrono>

// Intialising Class Objects
Camera CameraClass;
std::vector<Object> ObjectArr;
Texture_2D tex;

Shader ShaderWireframe;
Shader ShaderFull;

Timer timer;

double previous_x = 0.0f;
double previous_y = 0.0f;

bool cursor = false;

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

	if (!cursor)
	{
		CameraClass.moveCameraOrientation(dx, dy);
	}
	

	previous_x = x;
	previous_y = y;

}

int main()
{	
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
	std::cout << "Open GL Loader: ";
	ErrorCheck(); // Test

	/* Timer */
	timer.StartTimer();

	/* Camera Setup */
	CameraClass.setProjectionMatrix(WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 10000.0f);
	CameraClass.updateCamera();
	CameraClass.print();

	/* Shader Programs */
	GLuint shaderProgram_wireframe = ShaderWireframe.SetupShader(vs_wireframe, fs_wireframe);
	std::cout << "Wireframe Shader Program: ";
	ErrorCheck(); // Test

	GLuint shaderProgram_full = ShaderFull.SetupShader(vs_full, fs_full);
	std::cout << "Full Shader Program: ";
	ErrorCheck(); // Test

	/* Obj File Loader */
	Object EmptyObj;
	for (int i = 0; i < 4; i++)
	{
		ObjectArr.push_back(EmptyObj);
	}

	ObjectArr[0].load("C:/Users/Sam/source/repos/OpenGL_C++/OpenGL_0.6/OpenGL_0.6/meshes/cube.obj", "");
	ObjectArr[0].position(80, -50, 100);
	ObjectArr[0].scale(1);
	
	ObjectArr[1].load("C:/Users/Sam/source/repos/OpenGL_C++/OpenGL_0.6/OpenGL_0.6/meshes/dog.obj" , "");
	ObjectArr[1].scale(10);
	ObjectArr[1].position(500, 0, 500);
	ObjectArr[1].rotation(0, 90, 90);

	ObjectArr[2].load("C:/Users/Sam/source/repos/OpenGL_C++/OpenGL_0.6/OpenGL_0.6/meshes/yoda.obj", "");
	ObjectArr[2].scale(10);
	ObjectArr[2].position(100, -80, 20);

	ObjectArr[3].load("C:/Users/Sam/source/repos/OpenGL_C++/OpenGL_0.6/OpenGL_0.6/meshes/leia.obj", "");
	ObjectArr[3].scale(2);
	ObjectArr[3].position(0, -50, -200);

	for (int i = 0; i < ObjectArr.size(); i++)
	{
		ObjectArr[i].bind_shader_program(shaderProgram_full, shaderProgram_wireframe);
		std::cout << "Object " << i << ": Shader Programs Assigned\n";
	}

	for (int i = 0; i < ObjectArr.size(); i++)
	{
		ObjectArr[i].setup();
		ObjectArr[i].setupWireframe();
	}

	/* OpenGL Rendering Properties */
	glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
	glDepthFunc(GL_LESS);			// Discards fragment that are further away than currently rendered fragments (render back-to-front)
	glEnable(GL_CULL_FACE);			// Enables culling to allows triangles to be removed at either back or front to not be rendered
	glCullFace(GL_BACK);			// Sets culling mode to remove all back triangles
	glFrontFace(GL_CCW);			// Front Triangles have their points ordered counter clock-wise

	/* Set Mouse Input */
	glfwSetCursorPos(window, 0, 0);
	glfwSetMouseButtonCallback(window, mouseButtonCallBack);
	glfwSetCursorPosCallback(window, cursorPositionCallBack);

	bool tab_state = false;
	bool close = false;
	bool jump = false;

	double cursorX;
	double cursorY;

	bool mouse_l_state = false;
	bool mouse_r_state = false;

	while (!glfwWindowShouldClose(window) && !close)
	{
		/* INPUT */
		{
			/* Poll of inputs and process events */
			glfwPollEvents();

			/* Input Management */
			{
				int state;

				glfwGetCursorPos(window, &cursorX, &cursorY);

				// Mouse 1
				state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
				if (state == GLFW_PRESS)
				{
					mouse_l_state = true;
				}
				else
				{
					mouse_l_state = false;
				}

				// Mouse 2
				state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);
				if (state == GLFW_PRESS)
				{
					mouse_r_state = true;
				}
				else
				{
					mouse_r_state = false;
				}

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

				float yoda_speed = 0.1;
				// 8
				state = glfwGetKey(window, GLFW_KEY_KP_8);
				if (state == GLFW_PRESS)
				{
					std::cout << "8";
					ObjectArr[2].x_pos += yoda_speed;
				}

				// 5
				state = glfwGetKey(window, GLFW_KEY_KP_5);
				if (state == GLFW_PRESS)
				{
					std::cout << "5";
					ObjectArr[2].x_pos -= yoda_speed;
				}

				// 4
				state = glfwGetKey(window, GLFW_KEY_KP_4);
				if (state == GLFW_PRESS)
				{
					std::cout << "4";
					ObjectArr[2].z_pos -= yoda_speed;
				}

				// 6
				state = glfwGetKey(window, GLFW_KEY_KP_6);
				if (state == GLFW_PRESS)
				{
					std::cout << "6";
					ObjectArr[2].z_pos += yoda_speed;
				}

				// ENTER
				state = glfwGetKey(window, GLFW_KEY_ENTER);
				if (state == GLFW_PRESS)
				{
					std::cout << "ENTER";
					//CameraClass.moveCameraPosition(0, 0, 1);

					if (!jump)
					{
						jump = true;
					}

				}

				if (jump)
				{
					if (ObjectArr[2].z_rot >= Pi * 2 - Pi / 9)
					{
						jump = false;
						ObjectArr[2].z_rot = 0;
					}
					else
					{
						ObjectArr[2].z_rot += Pi / 9;
					}

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
					if (!tab_state)
					{
						tab_state = true;
						if (cursor)
						{
							glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
							cursor = false;
						}
						else
						{
							glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
							cursor = true;
						}

					}
					std::cout << "TAB";
				}
				else
				{
					if (state == GLFW_RELEASE)
					{
						//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
						tab_state = false;
					}
				}

				// ESC
				state = glfwGetKey(window, GLFW_KEY_ESCAPE);
				if (state == GLFW_PRESS)
				{
					std::cout << "ESC";
					close = true;
				}
			}

			/* Move Camera */
			CameraClass.updateCamera();
		}

		/* RENDER */
		{
			// Clear the screen to black & clear depth buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (int i = 0; i < ObjectArr.size(); i++)
				ObjectArr[i].render(CameraClass);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);
		}
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	ShaderWireframe.DeleteShaderResources();
	ShaderFull.DeleteShaderResources();

	for (int i = 0; i < ObjectArr.size(); i++)
	{
		ObjectArr[i].delete_buffer_resources();
	}
	
	glfwTerminate();

	return 0;
}

// DrawArrays(render type/mode, first position, number of verticies)