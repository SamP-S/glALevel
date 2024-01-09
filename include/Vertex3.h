#pragma once

#include <math.h>
#include <stdio.h>
#include <iostream>

/* Note
With OpenGL and GLSL Matricies are column major ordered >> [x][y]
*/

/* 1 x 3 Vector */
struct vec3_t
{
	float x;
	float y;
	float z;
};

vec3_t vec3(float x, float y, float z)
{
	vec3_t v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

/* <type><operator><"" = same type, "s" = scalar>*/

vec3_t v3_Add(vec3_t a, vec3_t b) 
{
	vec3_t v;
	v.x = a.x + b.x;
	v.y = a.y + b.y;
	v.z = a.z + b.z;
	return v;
}

vec3_t v3_Add(vec3_t a, vec3_t b, vec3_t c)
{
	vec3_t v;
	v.x = a.x + b.x + c.x;
	v.y = a.y + b.y + c.y;
	v.z = a.z + b.z + c.z;
	return v;
}

vec3_t v3_AddS(vec3_t a, float s)
{
	vec3_t v;
	v.x = a.x + s;
	v.y = a.y + s;
	v.z = a.z + s;
	return v;
}

vec3_t v3_Sub(vec3_t a, vec3_t b)
{
	vec3_t v;
	v.x = a.x - b.x;
	v.y = a.y - b.y;
	v.z = a.z - b.z;
	return v;
}

vec3_t v3_SubS(vec3_t a, float s)
{
	vec3_t v;
	v.x = a.x - s;
	v.y = a.y - s;
	v.z = a.z - s;
	return v;
}

vec3_t v3_Mul(vec3_t a, vec3_t b)
{
	vec3_t v;
	v.x = a.x * b.x;
	v.y = a.y * b.y;
	v.z = a.z * b.z;
	return v;
}

vec3_t v3_MulS(vec3_t a, float s)
{
	vec3_t v;
	v.x = a.x * s;
	v.y = a.y * s;
	v.z = a.z * s;
	return v;
}

vec3_t v3_Div(vec3_t a, vec3_t b)
{
	vec3_t v;
	v.x = a.x / b.x;
	v.y = a.y / b.y;
	v.z = a.z / b.z;
	return v;
}

vec3_t v3_DivS(vec3_t a, float s)
{
	vec3_t v;
	v.x = a.x / s;
	v.y = a.y / s;
	v.z = a.z / s;
	return v;
}

float v3_Length(vec3_t v)
{
	return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

float v3_Dot(vec3_t a, vec3_t b)
{
	return a.x * b.x + a.y * b.y + a.z *b.z;
}

vec3_t v3_Normalise(vec3_t v)
{
	float len = v3_Length(v);

	if (len > 0)
		return vec3_t { v.x / len, v.y / len, v.z / len };
	else
		return vec3_t { 0, 0, 0 };
}

vec3_t v3_Project(vec3_t v, vec3_t onto)
{
	return v3_MulS(onto, v3_Dot(v, onto) / v3_Dot(onto, onto));
}

vec3_t v3_Cross(vec3_t a, vec3_t b)
{
	return vec3_t
	{
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

float v3_Angle_Between(vec3_t a, vec3_t b)
{
	return acosf(v3_Dot(a, b) / (v3_Length(a)* v3_Length(b)));
}

void v3_print(vec3_t a)
{
	std::cout << "x: " << a.x << " y: " << a.y << " z: " << a.z << std::endl;
}

vec3_t v3_Absolute(vec3_t a)
{
	vec3_t v;

	if (a.x < 0)
		v.x = a.x * -1;
	else
		v.x = a.x;

	if (a.y < 0)
		v.y = a.y * -1;
	else
		v.y = a.y;

	if (a.z < 0)
		v.z = a.z * -1;
	else
		v.z = a.z;

	return v;
}