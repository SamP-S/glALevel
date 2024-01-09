#pragma once

#include "glfw3.h"
#include <iostream>

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
		std::cout << "OK: No Error!" << std::endl;
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