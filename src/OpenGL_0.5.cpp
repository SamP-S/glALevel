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
#include <stb_image.h>

/* Shaders */
// Vertex Shader
const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    in vec3 color;
    in vec2 texcoord;
    out vec3 Color;
    out vec2 Texcoord;
    void main()
    {
        Color = color;
        Texcoord = texcoord;
        gl_Position = vec4(position, 0.0, 1.0);
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
	/* OPENGL CONTEXT */
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



	/* vertices */
	GLfloat vertices[] = {
		//  Position			Color					Texcoords
			-0.5f,  0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f, // Top-left
			 0.5f,  0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 1.0f, // Top-right
			 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, // Bottom-right
			-0.5f, -0.5f,		1.0f, 1.0f, 1.0f,		0.0f, 0.0f  // Bottom-left
	};

	/* ELEMENTS */
	GLuint elements[] =
	{
	0, 1, 2,	// Triangle 1
	2, 3, 0		// Triangle 2
	};



	/* VERTEX ARRAY OBJECTS (VAO) */

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



	/* VERTEX BUFFER OBJECTS (VBO) */

	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 buffer

	/* Create Vertex Buffer Object -> Load Array to Buffer */
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // Binds new buffer to being the new "Active Array Buffer"
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copies "Active Array Buffer" to GPU Memor

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

	/* Create shader object -> Load shader into object */
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);

	/* Compile Shader */
	glCompileShader(vertexShader);

	/* Check Shader Compilation Status */
	GLint status;
	char buffer[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	/* Get Shader Compilation Log IF FAILED */
	if (status != GL_TRUE)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		std::cout << buffer << std::endl;
	}



	/* FRAGMENT SHADERS */

	/* Create shader object -> Load shader into object */
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

	/* Compile Shader */
	glCompileShader(fragmentShader);

	/* Check Shader Compilation Status */
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	/* Get Shader Compilation Log IF FAILED */
	if (status != GL_TRUE)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		std::cout << buffer << std::endl;
	}



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

	/* ATTRIBUTES ("COLOUR") */

	/*  Retrieves a reference to the position input in the fragment shader */
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");

	/* References the input and detemines how the data is retrieved from the array*/
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

	/* Enables vertex Attribute */
	glEnableVertexAttribArray(colAttrib);

	/* ATTRIBUTES ("TEXTURE POSITION") */

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

	/* Notes
	The first parameter references the input. The second parameter specifies the number of values for that input, which is the same as the number of components of the vec.
	The third parameter specifies the type of each component and the fourth parameter specifies whether the input values should be normalized between -1.0 and 1.0
	(or 0.0 and 1.0 depending on the format) if they aren't floating point numbers.
	The last two parameters are arguably the most important here as they specify how the attribute is laid out in the vertex array.
	The first number specifies the stride, or how many bytes are between each position attribute in the array. The value 0 means that there is no data in between.
	This is currently the case as the position of each vertex is immediately followed by the position of the next vertex.
	The last parameter specifies the offset, or how many bytes from the start of the array the attribute occurs. Since there are no other attributes, this is 0 as well.
	*/



	///* TEXTURE OBJECT */

	///* Initialise Texture Object */
	//GLuint tex;

	///* Generate Texture Object */
	//glGenTextures(1, &tex);

	///* Bind Texture to Object */
	//glBindTexture(GL_TEXTURE_2D, tex);

	///* LOADING TEXTURE */
	//// White board
	//int width = 400;
	//int height = 200;
	//float pixels[80000];
	//for (int i = 0; i < 80000; i++)
	//{
	//	pixels[i] = 1.0f;
	//}
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	///* TEXTURE PARAMETERS */
	///* TEXTURE -> WRAPPING */
	//float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };		// Float Array for a colour (R,G,B,A)
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);	//Binds Colour Array to Texture

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// Set Wrapping Mode Per Dimension
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// Set Wrapping Mode Per Dimension

	///* Wrapping
	//GL_REPEAT: The integer part of the coordinate will be ignored and a repeating pattern is formed.
	//GL_MIRRORED_REPEAT: The texture will also be repeated, but it will be mirrored when the integer part of the coordinate is odd.
	//GL_CLAMP_TO_EDGE: The coordinate will simply be clamped between 0 and 1.
	//GL_CLAMP_TO_BORDER: The coordinates that fall outside the range will be given a specified border color.
	//*/

	///* Notes
	//The clamping can be set per coordinate, where the equivalent of (x,y,z) in texture coordinates is called (s,t,r).
	//Texture parameter are changed with the glTexParameter* functions as demonstrated here.
	//*/

	///* TEXTURE -> FILTERING */

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);		// Scaling Down
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		// Scaling Up

	///* Generate Mipmap */
	//glGenerateMipmap(GL_TEXTURE_2D);

	///* Filtering (Interpolation)
	//GL_NEAREST: Returns the pixel that is closest to the coordinates.
	//GL_LINEAR: Returns the weighted average of the 4 pixels surrounding the given coordinates.
	//GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR: Sample from mipmaps instead (Fastest and Best Quality)
	//*/

	///* Filtering -> Scaling
	//You can specify which kind of interpolation should be used for two separate cases:
	//-scaling the image down	-> GL_TEXTURE_MIN_FILTER
	//-scaling the image up	-> GL_TEXTURE_MAG_FILTER
	//*/


	/* TEXTURE OBJECT 2 */
		// Load texture
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	// White board
	int width = 400;
	int height = 200;
	float pixels[80000];
	for (int i = 0; i < 80000; i++)
	{
		pixels[i] = 1.0f;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


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

		//glDrawArrays(GL_TRIANGLES, 0, 3); // Triangles
		//glDrawArrays(GL_POLYGON, 0, 3);	// Polygons

		/* "MODE", "NUM_vertices", "Type", "Indicies" */
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

