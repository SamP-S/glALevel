#pragma once

/*

Shader ShaderClass;

// Vertex Source
const GLchar* vertexSource = R"glsl(
	#version 150 core

	in vec3 v_position;
	in vec3 v_texture_coord;
	in vec3 v_normal_coord;

	out vec3 f_colour;
	out vec3 f_texture_coord;
	out vec3 f_normal_coord;

	uniform mat4 matrix;
	uniform mat4 persp;
	uniform mat4 view;

	void main()
	{
		f_colour = v_position;
		gl_Position = matrix * persp * view * vec4(v_position, 1.0);
		f_texture_coord = v_texture_coord;
		f_normal_coord = v_normal_coord;
	}
)glsl";

// Fragment Source
const GLchar* fragmentSource = R"glsl(
	#version 150 core

	in vec3 f_colour;
	in vec3 f_texture_coord;
	in vec3 f_normal_coord;

	out vec4 o_colour;

	uniform sampler3D tex;

	void main()
	{
	  o_colour = vec4(f_texture_coord, 1.0);
	}
)glsl";	*/

/*
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
*/

/* VBOs
	// Create a Vertex Buffer Object for each Data Type
	// Verticies
	GLuint verticieBuffer;
	glGenBuffers(1, &verticieBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, verticieBuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjClass.verticies.size() * sizeof(vec3_t), &ObjClass.verticies[0], GL_STATIC_DRAW);
	std::cout << "Verticie Buffer: ";
	ErrorCheck(); // Test

	// UVs
	GLuint uvBuffer;
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, ObjClass.uvs.size() * sizeof(vec3_t), &ObjClass.uvs[0], GL_STATIC_DRAW);
	std::cout << "UV Buffer: ";
	ErrorCheck(); // Test

	GLuint normalBuffer;
	if (ObjClass.normals.size() != 0)
	{
		// Normals
		glGenBuffers(1, &normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, ObjClass.normals.size() * sizeof(vec3_t), &ObjClass.normals[0], GL_STATIC_DRAW);
		std::cout << "Normal Buffer: ";
		ErrorCheck(); // Test
	}

	Attributes
	// Specify the layout of the vertex data
	// Binds verticie buffer as active buffer then
	// Binds position attribute to active buffer

	glBindBuffer(GL_ARRAY_BUFFER, verticieBuffer);
	GLint posAttrib = glGetAttribLocation(shaderProgram, "v_position");
	//GLint posAttrib = 0;
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	std::cout << "Position Attribute: ";
	ErrorCheck();

	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	GLint texAttrib = glGetAttribLocation(shaderProgram, "v_texture_coord");
	//GLint texAttrib = 1;
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glActiveTexture(GL_TEXTURE0);
	std::cout << "Texture Attribute: ";
	ErrorCheck();

	if (ObjClass.normals.size() != 0)
	{
		//glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		//GLint norAttrib = glGetAttribLocation(shaderProgram, "v_normal_coord");
		//GLint norAttrib = 2;
		//glEnableVertexAttribArray(norAttrib);
		//glVertexAttribPointer(norAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//std::cout << "Normal Attribute: ";
		//ErrorCheck();
	}

	Element Buffer moved to create a new one per object
	*/

	/* Uniforms
	glUniformMatrix4fv(uniMatrix, 1, GL_FALSE, &CameraClass.modelTransformationMatrix.m[0][0]);
	glUniformMatrix4fv(uniPersp, 1, GL_FALSE, &CameraClass.perspectiveProjectionMatrix.m[0][0]);
	glUniformMatrix4fv(uniView, 1, GL_FALSE, &CameraClass.cameraViewMatrix.m[0][0]);*/