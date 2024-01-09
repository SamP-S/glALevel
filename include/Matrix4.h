#pragma once

#include <math.h>
#include <iostream>
#include "Vertex3.h"

const float Pi = 3.1415926535;

/* Note
With OpenGL and GLSL Matricies are column major ordered >> [x][y]
*/

/*
Column Major vs Row Major only affects the way in which arrays are stored

COLUMN MAJORED
// | m[0][0]  m[1][0]  m[2][0]  m[3][0] |
// | m[0][1]  m[1][1]  m[2][1]  m[3][1] |
// | m[0][2]  m[1][2]  m[2][2]  m[3][2] |
// | m[0][3]  m[1][3]  m[2][3]  m[3][3] |

ROW MAJORED
// | m[0][0]  m[0][1]  m[0][2]  m[0][3] |
// | m[1][0]  m[1][1]  m[1][2]  m[1][3] |
// | m[2][0]  m[2][1]  m[2][2]  m[2][3] |
// | m[3][0]  m[3][1]  m[3][2]  m[3][3] |

The data is still always represented as:

a.00, b.01, c.02, d.03,
e.10, f.11, g.12, h.13,
i.20, j.21, k.22, l.23,
m.30, n.31, o.32, p.33,

*/

/* 4 x 4 Matrix */
typedef union {			// Union allows for different types to be stored in 1 memory location
	float m[4][4];
	struct {
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;
	};
} mat4_t;

mat4_t mat4(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	mat4_t m;
	m.m[0][0] = m00; m.m[1][0] = m10; m.m[2][0] = m20; m.m[3][0] = m30;
	m.m[0][1] = m01; m.m[1][1] = m11; m.m[2][1] = m21; m.m[3][1] = m31;
	m.m[0][2] = m02; m.m[1][2] = m12; m.m[2][2] = m22; m.m[3][2] = m32;
	m.m[0][3] = m03; m.m[1][3] = m13; m.m[2][3] = m23; m.m[3][3] = m33;
	return m;
}

mat4_t mat4(float f)
{
	mat4_t m;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (x == y)
			{
				m.m[x][y] = f;
			}
			else
			{
				m.m[x][y] = 0.0f;
			}
		}
	}
	return m;
}

mat4_t m4_Identity()
{
	return mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

mat4_t m4_Mul(mat4_t a, mat4_t b)
{
	mat4_t m;
	float total;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			total = 0;
			for (int k = 0; k < 4; k++)
			{
				total = total + a.m[i][k] * b.m[k][j];
			}
			m.m[i][j] = total;
		}
	}
	return m;
}

mat4_t m4_Translation(vec3_t offset)
{
	return mat4(
		1, 0, 0, offset.x,
		0, 1, 0, offset.y,
		0, 0, 1, offset.z,
		0, 0, 0, 1
	);
}

mat4_t m4_Scaling(vec3_t scale)
{
	float x = scale.x;
	float y = scale.y;
	float z = scale.z;
	return mat4(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	);
}

mat4_t m4_Rotation_x(float angle_in_rad)
{
	float s = sinf(angle_in_rad);
	float c = cosf(angle_in_rad);
	return mat4
	(
		1, 0, 0, 0,
		0, c, -s, 0,
		0, s, c, 0,
		0, 0, 0, 1
	);
}

mat4_t m4_Roation_y(float angle_in_rad)
{
	float s = sinf(angle_in_rad);
	float c = cosf(angle_in_rad);
	return mat4
	(
		c, 0, s, 0,
		0, 1, 0, 0,
		-s, 0, c, 0,
		0, 0, 0, 1
	);

}

mat4_t m4_Rotation_z(float angle_in_rad)
{
	float s = sinf(angle_in_rad);
	float c = cosf(angle_in_rad);
	return mat4
	(
		c, -s, 0, 0,
		s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

mat4_t m4_Transpose(mat4_t m)
{
	return mat4
	(
		m.m00, m.m10, m.m20, m.m30,
		m.m01, m.m11, m.m21, m.m31,
		m.m02, m.m12, m.m22, m.m32,
		m.m03, m.m13, m.m23, m.m33
	);
}

/* Matrix Rotation
Creates a matrix to rotate around an axis by a given angle.
The axis does not have to be normalised.
*/
mat4_t m4_Rotation(float angle_in_rad, vec3_t axis)
{
	vec3_t normalised_axis = v3_Normalise(axis);
	float x = normalised_axis.x;
	float y = normalised_axis.y;
	float z = normalised_axis.z;
	float s = sinf(angle_in_rad);
	float c = cosf(angle_in_rad);

	return mat4
	(
		c + x * x*(1 - c), x*y*(1 - c) - z * s, x*z*(1 - c) + y * s, 0,
		y*x*(1 - c) + z * s, c + y * y*(1 - c), y*z*(1 - c) - x * s, 0,
		z*x*(1 - c) - y * s, z*y*(1 - c) + x * s, c + z * z*(1 - c), 0,
		0, 0, 0, 1
	);
}

/* Orthographic Projection
The right-handed cube is mapped into normalized device
coordinates, a left-handed cube where (-1 -1) is the lower left corner,
(1, 1) the upper right corner and a z-value of -1 is the nearest point and
1 the furthest point. OpenGL takes it from there and puts it on the screen
and into the z-buffer.
*/

mat4_t m4_Orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	float tz = -(zFar + zNear) / (zFar - zNear);
	return mat4
	(
		2 / (right - left), 0, 0, tx,
		0, 2 / (top - bottom), 0, ty,
		0, 0, 2 / (zFar - zNear), tz,
		0, 0, 0, 1
	);
}

/* Matrix Projection
The camera is at the origin and looks in the direction of the negative Z axis.
"near_view_distance" and "far_view_distance" have to be positive and > 0.
They are distances from the camera eye, not values on an axis.
*/

mat4_t m4_Perspective(float vertical_FOV_in_deg, float aspect_ratio, float near_view_distance, float far_view_distance)
{
	float FOVy_in_rad = vertical_FOV_in_deg * Pi / 180;
	float f = 1.0f / tanf(FOVy_in_rad / 2.0f);
	float nvd = near_view_distance;
	float fvd = far_view_distance;
	return mat4
	(
		f / aspect_ratio, 0, 0, 0,
		0, f, 0, 0,
		0, 0, (fvd + nvd) / (nvd - fvd), (2 * fvd * nvd) / (nvd - fvd),
		0, 0, -1, 0
	);
}

/* Camera Translation Transformation Matrix
Builds a transformation matrix for a camera that looks from "from" towards
"to". "up" defines the direction that's upwards for the camera. All three
vectors are given in world space and `up` doesn't need to be normalized.
*/

mat4_t m4_Look_At(vec3_t from, vec3_t to, vec3_t up)
{
	vec3_t z = v3_MulS(v3_Normalise(v3_Sub(to, from)), -1);
	vec3_t x = v3_Normalise(v3_Cross(up, z));
	vec3_t y = v3_Cross(z, x);
	return mat4
	(
		x.x, x.y, x.z, -v3_Dot(from, x),
		y.x, y.y, y.z, -v3_Dot(from, y),
		z.x, z.y, z.z, -v3_Dot(from, z),
		0, 0, 0, 1
	);
}

/* Invert Affine Transformation Matrix
Inverts an affine transformation matrix. That are translation, scaling, mirroring,
reflection, rotation and shearing matrices or any combination of them.
inv(matrix) = inv(rotation) * inv(translation)
*/

mat4_t m4_Invert_Affine(mat4_t m)
{
	// Invert 3x3 part of the 4x4 matric that contains the rotation ( R )

		// Calculate CoFactor Matrix of R
	float c00 = m.m11 * m.m22 - m.m12 * m.m21, c10 = -(m.m01*m.m22 - m.m02 * m.m21), c20 = m.m01 * m.m12 - m.m02 * m.m11;
	float c01 = -(m.m10*m.m22 - m.m12 * m.m20), c11 = m.m00 * m.m22 - m.m02 * m.m20, c21 = -(m.m00*m.m12 - m.m02 * m.m10);
	float c02 = m.m10 * m.m21 - m.m11 * m.m20, c12 = -(m.m00*m.m21 - m.m01 * m.m20), c22 = m.m00 * m.m11 - m.m01 * m.m10;

	// Calculate determinant by using the calculated cofactor maxtrix
	// Second sign is already minus from the covfactor matrix
	float det = m.m00 * c00 + m.m10 * c10 + m.m20 * c20;
	if (fabsf(det == 0))
	{
		return m4_Identity();
	}

	// Calculate inverse of R by dividing the transposed co factor matrix by the determinant
	float i00 = c00 / det, i10 = c01 / det, i20 = c02 / det;
	float i01 = c10 / det, i11 = c11 / det, i21 = c12 / det;
	float i02 = c20 / det, i12 = c21 / det, i22 = c22 / det;

	// Combine the inverted R with the inverted translation
	return mat4
	(
		i00, i10, i20, -(i00*m.m30 + i10 * m.m31 + i20 * m.m32),
		i01, i11, i21, -(i01*m.m30 + i11 * m.m31 + i21 * m.m32),
		i02, i12, i22, -(i02*m.m30 + i12 * m.m31 + i22 * m.m32),
		0, 0, 0, 1
	);
}

/*
Multiplies a 4x4 matrix with a 3D vector representing a point in 3D space.
*/

vec3_t m4_Mul_Pos(mat4_t m, vec3_t pos)
{
	vec3_t result = vec3
	(
		m.m00 * pos.x + m.m10 * pos.y + m.m20 * pos.z + m.m30,
		m.m01 * pos.x + m.m11 * pos.y + m.m21 * pos.z + m.m31,
		m.m02 * pos.x + m.m12 * pos.y + m.m22 * pos.z + m.m32
	);

	float w = m.m03 * pos.x + m.m13 * pos.y + m.m23 * pos.z + m.m33;
	if (w != 0 && w != 1)
	{
		return vec3(result.x / w, result.y / w, result.z / w);
	}

	return result;
}

void m4_print_properties(mat4_t m)
{
	float *pointer = &m.m[0][0];
	std::cout << "Float Array:		   " << m.m << std::endl;
	std::cout << "Pointer:             " << pointer << std::endl;
	std::cout << "*Pointer Value:      " << *pointer << std::endl;
	std::cout << "&Pointer Pointer:    " << &pointer << std::endl;
	std::cout << "&Pointer[0] Pointer: " << &pointer[0] << std::endl;
}

void m4_print(mat4_t m)
{
	int i, j;
	for (j = 0; j < 4; j++)
	{
		for (i = 0; i < 4; i++)
		{
			std::cout << m.m[i][j];
		}
		std::cout << std::endl;
	}
}
