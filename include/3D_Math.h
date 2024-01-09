#pragma once

#include "Vertex2.h"
#include "Vertex3.h"
#include "Matrix3.h"
#include "Matrix4.h"

float deg_to_rad(float angle_in_degrees)
{
	return angle_in_degrees / 180 * Pi;
}

float rad_to_deg(float angle_in_radians)
{
	return angle_in_radians * 180 / Pi;
}