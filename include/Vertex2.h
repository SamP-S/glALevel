#pragma once

#include <math.h>
#include <stdio.h>
#include <iostream>

/* Note
With OpenGL and GLSL Matricies are column major ordered >> [x][y]
*/

/* 1 x 3 Vector */
struct vec2_t
{
	float x;
	float y;
};

vec2_t vec2(float x, float y)
{
	vec2_t v;
	v.x = x;
	v.y = y;
	return v;
}

/* <type><operator><"" = same type, "s" = scalar>*/

vec2_t v2_Add(vec2_t a, vec2_t b)
{
	vec2_t v;
	v.x = a.x + b.x;
	v.y = a.y + b.y;
	return v;
}

vec2_t v2_AddS(vec2_t a, float s)
{
	vec2_t v;
	v.x = a.x + s;
	v.y = a.y + s;
	return v;
}

vec2_t v2_Sub(vec2_t a, vec2_t b)
{
	vec2_t v;
	v.x = a.x - b.x;
	v.y = a.y - b.y;
	return v;
}

vec2_t v2_SubS(vec2_t a, float s)
{
	vec2_t v;
	v.x = a.x - s;
	v.y = a.y - s;
	return v;
}

vec2_t v2_Mul(vec2_t a, vec2_t b)
{
	vec2_t v;
	v.x = a.x * b.x;
	v.y = a.y * b.y;
	return v;
}

vec2_t v2_MulS(vec2_t a, float s)
{
	vec2_t v;
	v.x = a.x * s;
	v.y = a.y * s;
	return v;
}

vec2_t v2_Div(vec2_t a, vec2_t b)
{
	vec2_t v;
	v.x = a.x / b.x;
	v.y = a.y / b.y;
	return v;
}

vec2_t v2_DivS(vec2_t a, float s)
{
	vec2_t v;
	v.x = a.x / s;
	v.y = a.y / s;
	return v;
}

float v2_Length(vec2_t v)
{
	return sqrtf(v.x*v.x + v.y*v.y);
}

float v2_Dot(vec2_t a, vec2_t b)
{
	return a.x * b.x + a.y * b.y;
}

vec2_t v2_Normalise(vec2_t v)
{
	float len = v2_Length(v);

	if (len > 0)
		return vec2_t{ v.x / len, v.y / len };
	else
		return vec2_t{ 0, 0};
}

vec2_t v2_Project(vec2_t v, vec2_t onto)
{
	return v2_MulS(onto, v2_Dot(v, onto) / v2_Dot(onto, onto));
}

float v2_Angle_Between(vec2_t a, vec2_t b)
{
	return acosf(v2_Dot(a, b) / (v2_Length(a)* v2_Length(b)));
}

void v2_print(vec2_t a)
{
	std::cout << "x: " << a.x << " y: " << a.y << std::endl;
}