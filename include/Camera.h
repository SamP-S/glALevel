#pragma once

#include "glfw3.h"
#include "3D_Math.h"

class Camera
{
private:
	float horizontalAngle = 0.0f;
	float verticalAngle = 0.0f;

public:
	vec3_t cameraFrom = vec3(1.2f, 1.2f, 1.2f);
	vec3_t cameraTo = vec3(0.0f, 0.0f, 0.0f);
	vec3_t cameraUp = vec3(0.0f, 1.0f, 0.0f);
	vec3_t cameraRight = vec3(1.0f, 0.0f, 0.0f);
	vec3_t cameraDirection = vec3(1.0f, 1.0f, 1.0f);

	mat4_t modelTransformationMatrix = m4_Identity();
	mat4_t cameraViewMatrix;
	mat4_t perspectiveProjectionMatrix;

	mat4_t matrix;

	float mouse_sensitivity = 0.0015f;
	float speed = 0.5f;

	float FOV = 80.0f;

	void setProjectionMatrix(float Window_Width, float Window_Height, float Near, float Far)
	{
		perspectiveProjectionMatrix = m4_Perspective(FOV, Window_Width / Window_Height, Near, Far);
		return;
	}

	void moveCameraOrientation(double dx, double dy)
	{
		horizontalAngle += mouse_sensitivity * float(-dx);
		verticalAngle += mouse_sensitivity * float(dy);

		cameraDirection = vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		cameraTo = v3_Add(cameraFrom, cameraDirection);

		cameraRight = vec3(
			sin(horizontalAngle - Pi / 2.0f),
			0,
			cos(horizontalAngle - Pi / 2.0f)
			);

		cameraUp = v3_Cross(cameraRight, cameraDirection);
	}

	void moveCameraPosition(double forward, double right, double up)
	{
		if (forward == 1)
		{
			cameraFrom = v3_Add(cameraFrom, v3_MulS(cameraDirection, speed));
		}

		if (forward == -1)
		{
			cameraFrom = v3_Sub(cameraFrom, v3_MulS(cameraDirection, speed));
		}
		
		if (right == 1)
		{
			cameraFrom = v3_Add(cameraFrom, v3_MulS(cameraRight, speed));
		}

		if (right == -1)
		{
			cameraFrom = v3_Sub(cameraFrom, v3_MulS(cameraRight, speed));
		}

		if (up == 1)
		{
			cameraFrom = v3_Add(cameraFrom, v3_MulS(cameraUp, speed));
		}

		if (up == -1)
		{
			cameraFrom = v3_Sub(cameraFrom, v3_MulS(cameraUp, speed));
		}

		return;
	}

	void updateCamera()
	{
		moveCameraOrientation(0, 0);
		cameraViewMatrix = m4_Look_At(cameraFrom, cameraTo, cameraUp);
		matrix = m4_Mul(cameraViewMatrix, perspectiveProjectionMatrix);
	}

	void print()
	{
		printMatricies();
		printVectors();
	}

	void printMatricies()
	{
		std::cout << "----------------------------------------------------" << std::endl;
		std::cout << "  Camera Matricies  " << std::endl;
		std::cout << "----------------------------------------------------" << std::endl;
		std::cout << "Model Transformation: " << "Identity Matrix Expected" << std::endl;
		m4_print(modelTransformationMatrix);
		std::cout << std::endl;
		std::cout << "Camera View Matrix: " << std::endl;
		m4_print(cameraViewMatrix);
		std::cout << std::endl;
		std::cout << "Perspective Projecton Matrix: " << std::endl;
		m4_print(perspectiveProjectionMatrix);
		std::cout << std::endl;
		std::cout << "Complete Matrix: " << std::endl;
		m4_print(matrix);
		std::cout << "----------------------------------------------------" << std::endl;
	}

	void printVectors()
	{
		std::cout << "----------------------------------------------------" << std::endl;
		std::cout << "  Camera Vectors  " << std::endl;
		std::cout << "----------------------------------------------------" << std::endl;
		std::cout << "Camera From: " << std::endl;
		v3_print(cameraFrom);
		std::cout << std::endl;
		std::cout << "Camera To: " << std::endl;
		v3_print(cameraTo);
		std::cout << std::endl;
		std::cout << "Up Vector: " << std::endl;
		v3_print(cameraUp);
		std::cout << "----------------------------------------------------" << std::endl;
	}
};