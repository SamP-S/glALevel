#pragma once

/* Include */
#define GL_LITE_IMPLEMENTATION
#include "OpenGL_Loader.h"
#include "glfw3.h"

class Shader
{
private:
	const GLchar *VertexSource;
	const GLchar *FragmentSource;

	GLuint VertexShader_ID;
	GLuint FragmentShader_ID;

	/* Create and compile the vertex shader */
	GLuint VertexShader()
	{
		GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertexShader, 1, &VertexSource, NULL);
		glCompileShader(VertexShader);

		std::cout << "Vertex Shader Compile: ";
		int success;
		char infoLog[512];
		glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
		if (success)
		{
			std::cout << "Success\n";
		}
		else
		{
			std::cout << "FAILED\n";
			glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}

		return VertexShader;
	}

	/* Create and compile the fragment shader */
	GLuint FragmentShader()
	{
		GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragmentShader, 1, &FragmentSource, NULL);
		glCompileShader(FragmentShader);
		
		std::cout << "Fragment Shader Compile: ";
		int success;
		char infoLog[512];
		glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
		if (success)
		{
			std::cout << "Success\n";
		}
		else
		{
			std::cout << "FAILED\n";
 			glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}

 		return FragmentShader;
	}

	/* Link the vertex and fragment shader into a shader program that is uploaded to the GPU */
	GLuint ShaderProgram()
	{
		GLuint ShaderProgram = glCreateProgram();
		std::cout << "Shader Program Create: ";
		ErrorCheck();
		glAttachShader(ShaderProgram, VertexShader_ID);
		std::cout << "Shader Program Attach Vertex Shader: ";
		ErrorCheck();
		glAttachShader(ShaderProgram, FragmentShader_ID);
		std::cout << "Shader Program Attach Fragment Shader: ";
		ErrorCheck();
		glBindFragDataLocation(ShaderProgram, 0, "o_colour");
		std::cout << "Shader Program Bind Output Data: ";
		ErrorCheck();
		glLinkProgram(ShaderProgram);
		std::cout << "Shader Program Link Program: ";
		ErrorCheck();
		glUseProgram(ShaderProgram);
		std::cout << "Shader Program Use Program:";
		ErrorCheck();
		return ShaderProgram;
	}

public:
	GLuint ID;
	
	GLuint SetupShader(const char * vs, const char * fs)
	{
		VertexSource = vs;
		FragmentSource = fs;
		VertexShader_ID = VertexShader();
		std::cout << "Vertex Shader: ";
		ErrorCheck();
		FragmentShader_ID = FragmentShader();
		std::cout << "Fragment Shader: ";
		ErrorCheck();
		ID = ShaderProgram();
		std::cout << "Program Shader: ";
		ErrorCheck();
		return ID;
	}

	void DeleteShaderResources()
	{
		glDeleteProgram(ID);
		glDeleteShader(FragmentShader_ID);
		glDeleteShader(VertexShader_ID);
		return;
	}
};

