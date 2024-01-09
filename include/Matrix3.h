#pragma once

#include "Vertex3.h"

/* 2 x 2 Matrix */
float m2_Determinant(float m00, float m01, float m10, float m11)
{
	return m00 * m11 - m01 * m10;
}


/* 3 x 3 Matrix */
typedef union {			// Union allows for different types to be stored in 1 memory location
	float m[3][3];
	struct {
		float m00, m01, m02;
		float m10, m11, m12;
		float m20, m21, m22;
	};
} mat3_t;

mat3_t mat3(
	float m00, float m01, float m02,
	float m10, float m11, float m12,
	float m20, float m21, float m22)
{
	mat3_t m;
	m.m[0][0] = m00; m.m[1][0] = m10; m.m[2][0] = m20;
	m.m[0][1] = m01; m.m[1][1] = m11; m.m[2][1] = m21;
	m.m[0][2] = m02; m.m[1][2] = m12; m.m[2][2] = m22;
	return m;
}

mat3_t mat3(float f)
{
	mat3_t m;
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
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

mat3_t m3_Identity()
{
	return mat3(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	);
}

mat3_t m3_Mul(mat3_t a, mat3_t b)
{
	mat3_t m;
	float total;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			total = 0;
			for (int k = 0; k < 3; k++)
			{
				total = total + a.m[i][k] * b.m[k][j];
			}
			m.m[i][j] = total;
		}
	}
	return m;
}

vec3_t m3_MulV(mat3_t a, vec3_t b)
{
	vec3_t v;
	v.x = a.m00 * b.x + a.m01 * b.y + a.m02 * b.z;
	v.y = a.m10 * b.x + a.m11 * b.y + a.m12 * b.z;
	v.z = a.m20 * b.x + a.m21 * b.y + a.m22 * b.z;
	return v;
}

mat3_t m3_MulS(mat3_t a, float f)
{
	mat3_t m;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m.m[i][j] = m.m[i][j] * f;
		}
	}
	return m;
}

mat3_t m3_Transpose(mat3_t m)
{
	return mat3
	(
		m.m00, m.m10, m.m20,
		m.m01, m.m11, m.m21,
		m.m02, m.m12, m.m22
	);
}

mat3_t m3_MatrixMinors(mat3_t a)
{
	mat3_t m;
	m.m00 = m2_Determinant(a.m11, a.m12, a.m21, a.m22);
	m.m01 = m2_Determinant(a.m10, a.m12, a.m20, a.m22);	
	m.m02 = m2_Determinant(a.m10, a.m11, a.m20, a.m21);

	m.m10 = m2_Determinant(a.m01, a.m21, a.m02, a.m22);
	m.m11 = m2_Determinant(a.m00, a.m02, a.m20, a.m22);
	m.m12 = m2_Determinant(a.m00, a.m01, a.m20, a.m21);

	m.m20 = m2_Determinant(a.m01, a.m02, a.m11, a.m12);
	m.m21 = m2_Determinant(a.m00, a.m02, a.m10, a.m12);
	m.m22 = m2_Determinant(a.m00, a.m01, a.m10, a.m11);
	return m;
}

mat3_t m3_MatrixCoFactors(mat3_t m)
{
	m = m3_MatrixMinors(m);
	m.m01 = m.m01 * -1;
	m.m10 = m.m10 * -1;
	m.m12 = m.m12 * -1;
	m.m21 = m.m21 * -1;
	return m;
}

float m3_Determinant(mat3_t a)
{
	float f0 = m2_Determinant(a.m11, a.m12, a.m21, a.m22);
	float f1 = m2_Determinant(a.m10, a.m12, a.m20, a.m22);
	float f2 = m2_Determinant(a.m10, a.m11, a.m20, a.m21);

	return a.m00 * f0 - a.m01 * f1 + a.m02 * f2;
}


mat3_t m3_Inverse(mat3_t a)
{
	return m3_MulS(m3_MatrixCoFactors(a), 1 / m3_Determinant(a));
}