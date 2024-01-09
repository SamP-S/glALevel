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



	/* VERTEX ARRAY OBJECTS */

	/* Create Vertex Array Object -> Generate Object */
	GLuint vao;
	glGenVertexArrays(1, &vao); // Generate Array Object

	/* Bind Array to object */
	glBindVertexArray(vao);

	/* Notes
	As soon as you've bound a certain VAO, every time you call glVertexAttribPointer, that information will be stored in that VAO.
	This makes switching between different vertex data and vertex formats as easy as binding a different VAO.
	Just remember that a VAO doesn't store any vertex data by itself, it just references the VBOs you've created and how to retrieve the attribute values from them.
	*/



	/* VERTICIES */
	float verticies[] =
	{
	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left  Vertex 0 (X,Y,R,G,B)
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right  Vertex 1 (X,Y,R,G,B)
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right  Vertex 2 (X,Y,R,G,B)
	-0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left  Vertex 3 (X,Y,R,G,B)
	};

	/* ELEMENTS */
	GLuint elements[] = 
	{
	0, 1, 2,	// Triangle 1
	2, 3, 0		// Triangle 2
	};



	/* VERTEX BUFFER OBJECTS */

	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 buffer

	/* Create Vertex Buffer Object -> Load Array to Buffer */
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // Binds new buffer to being the new "Active Array Buffer"
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW); // Copies "Active Array Buffer" to GPU Memor

	/* Notes
	Only ONE vertexBuffer can be active at a time
	GL_STATIC_DRAW: The vertex data will be uploaded once and drawn many times (e.g. the world).
	GL_DYNAMIC_DRAW: The vertex data will be created once, changed from time to time, but drawn many times more than that.
	GL_STREAM_DRAW: The vertex data will be uploaded once and drawn once.
	*/

	/* ELEMENT BUFFER OBJECTS */

	/* Initialise Buffer*/
	GLuint ebo;
	glGenBuffers(1, &ebo);

	/* Binds buffer -> Binds Data */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


	/* VERTEX SHADERS */

	const char* vertexSource = R"glsl(
	#version 150 core

	in vec2 position;
	in vec3 color;

	out vec3 Color;

	void main()
	{
		Color = color;
	    gl_Position = vec4(position, 0.0, 1.0);
	}
	)glsl";

	/* Create shader object -> Load shader into object */
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);

	/* Compile Shader */
	glCompileShader(vertexShader);

	/* Check Shader Compilation Status */
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	/* Get Shader Compilation Log IF FAILED */
	if (status != GL_TRUE)
	{
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
	}



	/* FRAGMENT SHADERS */

	/* CONSTANT COLOUR
	const char* fragmentSource = R"glsl(
		#version 150 core

		out vec4 outColor;

		void main()
		{
			outColor = vec4(1.0, 1.0, 1.0, 1.0); // White
		}
		)glsl";
	*/
	const char* fragmentSource = R"glsl(
	#version 150 core

	in vec3 Color;

	out vec4 outColor;

	void main()
	{
	    outColor = vec4(Color, 1.0);
	}
	)glsl";


	/* Create shader object -> Load shader into object */
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

	/* Compile Shader */
	glCompileShader(fragmentShader);



	/* SHADER PROGRAM */

	/* Create program object -> Attach shaders */
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	/* Defines which buffer the fragment shader can write to */
	glBindFragDataLocation(shaderProgram, 0, "outColor");

	/* Links shader program -> Starts the program */
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	/* Notes
	Use "glDrawBuffers" when rendering to multiple buffers, because only the first output will be enabled by default.
	A shader object can be deleted with glDeleteShader, but it will not actually be removed before it has been detached from all programs with glDetachShader.
	Only ONE program can be active at a time
	*/



	/* ATTRIBUTES ("POSITION") */

	/*  Retrieves a reference to the position input in the vertex shader */
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");

	/* References the input and determines how the data for that input is retrieved from the array*/
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	/* Enables vertex Attribute */
	glEnableVertexAttribArray(posAttrib);


	/*  Retrieves a reference to the position input in the fragment shader */
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");

	/* References the input and detemines how the data is retrieved from the array*/
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

	/* Enables vertex Attribute */
	glEnableVertexAttribArray(colAttrib);


	/* Notes
	The first parameter references the input. The second parameter specifies the number of values for that input, which is the same as the number of components of the vec.
	The third parameter specifies the type of each component and the fourth parameter specifies whether the input values should be normalized between -1.0 and 1.0
	(or 0.0 and 1.0 depending on the format) if they aren't floating point numbers.
	The last two parameters are arguably the most important here as they specify how the attribute is laid out in the vertex array.
	The first number specifies the stride, or how many bytes are between each position attribute in the array. The value 0 means that there is no data in between.
	This is currently the case as the position of each vertex is immediately followed by the position of the next vertex.
	The last parameter specifies the offset, or how many bytes from the start of the array the attribute occurs. Since there are no other attributes, this is 0 as well.
	*/

	/* UNIFORMS (SIMILAR TO ATTRIBUTES BUT USED FOR CONSTANT COLOUR OVER A FRAGMENT)

	//Binds uniColour to shader colour uniform 
	GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");

	//Sets uniform to colour
	glUniform3f(uniColor, 1.0f, 0.0f, 0.0f); // Sets colour red
	*/


	/* Error Checking */
	GLint errorCode = glGetError();
	if (errorCode != 0)
	{
		std::cout << "ERROR " << errorCode << std::endl;
	}

	/* Start timer */
	auto t_start = std::chrono::high_resolution_clock::now();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(COLOUR_BLACK);		// Sets Background Colour to Black
		glClear(GL_COLOR_BUFFER_BIT);	// Clears Screen to Black

		/* TIMER */
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		std::cout << time << std::endl;

		/* Changes Colour depending on time */
		/* UNIFORM 
		//glUniform3f(uniColor, sin(time), sin(time + 90), sin(time + 180)); FOR USE WITH UNIFORMS
		*/

		/* Changes Colour depending on time */

		//glDrawArrays(GL_TRIANGLES, 0, 3); // Triangles
		//glDrawArrays(GL_POLYGON, 0, 3);	// Polygons

		/* "MODE", "NUM_VERTICIES", "Type", "Indicies" */
		glDrawElements(GL_POLYGON, 6, GL_UNSIGNED_INT, 0); // Draw Polygons using Elements

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