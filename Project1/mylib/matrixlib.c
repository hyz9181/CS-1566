/*
 * Yuanzhe Huang yuh94@pitt.edu
 * CS-1566 Lab 02 
 * To compile this file, please type "make matrixlib" in command line
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrixlib.h"

void vec_print(vec4 v)
{
	printf("vector print %.2f, %.2f, %.2f, %.2f\n", v.x, v.y, v.z, v.w);
}

vec4 sca_vec_mul(vec4 v, float a)
{
	vec4 v1 = {v.x * a, v.y * a, v.z * a, v.w * a};
	return v1;
}

vec4 vec_vec_add(vec4 v1, vec4 v2)
{
	vec4 result = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
	return result;
}

vec4 vec_vec_sub(vec4 v1, vec4 v2)
{
	vec4 result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
	return result;
}

float vec_magnitude(vec4 v)
{
	float mag = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2) + pow(v.w, 2));
	return mag;
}

vec4 vec_normalize(vec4 v)
{
	vec4 result;
	float mag = vec_magnitude(v);
	result.x = v.x / mag;
	result.y = v.y / mag;
	result.z = v.z / mag;
	result.w = v.w / mag;
	//result = {v.x / mag; v.y / mag; v.z / mag; v.w / mag};
	return result;
}

float vec_dot_product(vec4 v1, vec4 v2)
{
	float dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	return dot;
}

vec4 vec_cross_product(vec4 v1, vec4 v2)
{
	vec4 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	result.w = 0;
	return result;
}

void mat_print(mat4 m)
{
	printf("[%.2f %.2f %.2f %.2f]\n", m.x.x, m.y.x, m.z.x, m.w.x);
	printf("[%.2f %.2f %.2f %.2f]\n", m.x.y, m.y.y, m.z.y, m.w.y);
	printf("[%.2f %.2f %.2f %.2f]\n", m.x.z, m.y.z, m.z.z, m.w.z);
	printf("[%.2f %.2f %.2f %.2f]\n", m.x.w, m.y.w, m.z.w, m.w.w);
	printf("\n");
}

mat4 sca_mat_mul(mat4 m, float a)
{
	vec4 v1 = {a * m.x.x, a * m.x.y, a * m.x.z, a * m.x.w};
	vec4 v2 = {a * m.y.x, a * m.y.y, a * m.y.z, a * m.y.w};
	vec4 v3 = {a * m.z.x, a * m.z.y, a * m.z.z, a * m.z.w};
	vec4 v4 = {a * m.w.x, a * m.w.y, a * m.w.z, a * m.w.w};	
	mat4 sm = {v1, v2, v3, v4};
	return sm;
}

mat4 mat_mat_add(mat4 m1, mat4 m2)
{
	vec4 v1 = {m1.x.x + m2.x.x, m1.x.y + m2.x.y, m1.x.z + m2.x.z, m1.x.w + m2.x.w};
	vec4 v2 = {m1.y.x + m2.y.x, m1.y.y + m2.y.y, m1.y.z + m2.y.z, m1.y.w + m2.y.w};
	vec4 v3 = {m1.z.x + m2.z.x, m1.z.y + m2.z.y, m1.z.z + m2.z.z, m1.z.w + m2.z.w};
	vec4 v4 = {m1.w.x + m2.w.x, m1.w.y + m2.w.y, m1.w.z + m2.w.z, m1.w.w + m2.w.w};	
	mat4 m = {v1, v2, v3, v4};
	return m;
}

mat4 mat_mat_sub(mat4 m1, mat4 m2)
{
	vec4 v1 = {m1.x.x - m2.x.x, m1.x.y - m2.x.y, m1.x.z - m2.x.z, m1.x.w - m2.x.w};
	vec4 v2 = {m1.y.x - m2.y.x, m1.y.y - m2.y.y, m1.y.z - m2.y.z, m1.y.w - m2.y.w};
	vec4 v3 = {m1.z.x - m2.z.x, m1.z.y - m2.z.y, m1.z.z - m2.z.z, m1.z.w - m2.z.w};
	vec4 v4 = {m1.w.x - m2.w.x, m1.w.y - m2.w.y, m1.w.z - m2.w.z, m1.w.w - m2.w.w};	
	mat4 m = {v1, v2, v3, v4};
	return m; 
}

mat4 mat_mat_mul(mat4 m1, mat4 m2)
{
	vec4 v1, v2, v3, v4;
	v1.x = m1.x.x * m2.x.x + m1.y.x * m2.x.y + m1.z.x * m2.x.z + m1.w.x * m2.x.w;
	v1.y = m1.x.y * m2.x.x + m1.y.y * m2.x.y + m1.z.y * m2.x.z + m1.w.y * m2.x.w;
	v1.z = m1.x.z * m2.x.x + m1.y.z * m2.x.y + m1.z.z * m2.x.z + m1.w.z * m2.x.w;
    v1.w = m1.x.w * m2.x.x + m1.y.w * m2.x.y + m1.z.w * m2.x.z + m1.w.w * m2.x.w;

    v2.x = m1.x.x * m2.y.x + m1.y.x * m2.y.y + m1.z.x * m2.y.z + m1.w.x * m2.y.w;
	v2.y = m1.x.y * m2.y.x + m1.y.y * m2.y.y + m1.z.y * m2.y.z + m1.w.y * m2.y.w;
	v2.z = m1.x.z * m2.y.x + m1.y.z * m2.y.y + m1.z.z * m2.y.z + m1.w.z * m2.y.w;
    v2.w = m1.x.w * m2.y.x + m1.y.w * m2.y.y + m1.z.w * m2.y.z + m1.w.w * m2.y.w;

    v3.x = m1.x.x * m2.z.x + m1.y.x * m2.z.y + m1.z.x * m2.z.z + m1.w.x * m2.z.w;
	v3.y = m1.x.y * m2.z.x + m1.y.y * m2.z.y + m1.z.y * m2.z.z + m1.w.y * m2.z.w;
	v3.z = m1.x.z * m2.z.x + m1.y.z * m2.z.y + m1.z.z * m2.z.z + m1.w.z * m2.z.w;
    v3.w = m1.x.w * m2.z.x + m1.y.w * m2.z.y + m1.z.w * m2.z.z + m1.w.w * m2.z.w;

    v4.x = m1.x.x * m2.w.x + m1.y.x * m2.w.y + m1.z.x * m2.w.z + m1.w.x * m2.w.w;
	v4.y = m1.x.y * m2.w.x + m1.y.y * m2.w.y + m1.z.y * m2.w.z + m1.w.y * m2.w.w;
	v4.z = m1.x.z * m2.w.x + m1.y.z * m2.w.y + m1.z.z * m2.w.z + m1.w.z * m2.w.w;
    v4.w = m1.x.w * m2.w.x + m1.y.w * m2.w.y + m1.z.w * m2.w.z + m1.w.w * m2.w.w;

    mat4 mm = {v1, v2, v3, v4};
    return mm;
}

mat4 m4_minor(mat4 m)
{
	mat4 minor;

	minor.x.x = m.y.y * m.z.z * m.w.w + m.z.y * m.w.z * m.y.w + m.w.y * m.y.z * m.z.w - m.y.w * m.z.z * m.w.y - m.z.w * m.w.z * m.y.y - m.w.w * m.y.z * m.z.y;
	minor.x.y = m.y.x * m.z.z * m.w.w + m.z.x * m.w.z * m.y.w + m.w.x * m.y.z * m.z.w - m.y.w * m.z.z * m.w.x - m.z.w * m.w.z * m.y.x - m.w.w * m.y.z * m.z.x;
	minor.x.z = m.y.x * m.z.y * m.w.w + m.z.x * m.w.y * m.y.w + m.w.x * m.y.y * m.z.w - m.y.w * m.z.y * m.w.x - m.z.w * m.w.y * m.y.x - m.w.w * m.y.y * m.z.x;
	minor.x.w = m.y.x * m.z.y * m.w.z + m.z.x * m.w.y * m.y.z + m.w.x * m.y.y * m.z.z - m.y.z * m.z.y * m.w.x - m.z.z * m.w.y * m.y.x - m.w.z * m.y.y * m.z.x;

	minor.y.x = m.x.y * m.z.z * m.w.w + m.z.y * m.w.z * m.x.w + m.w.y * m.x.z * m.z.w - m.x.w * m.z.z * m.w.y - m.z.w * m.w.z * m.x.y - m.w.w * m.x.z * m.z.y;
	minor.y.y = m.x.x * m.z.z * m.w.w + m.z.x * m.w.z * m.x.w + m.w.x * m.x.z * m.z.w - m.x.w * m.z.z * m.w.x - m.z.w * m.w.z * m.x.x - m.w.w * m.x.z * m.z.x;
	minor.y.z = m.x.x * m.z.y * m.w.w + m.z.x * m.w.y * m.x.w + m.w.x * m.x.y * m.z.w - m.x.w * m.z.y * m.w.x - m.z.w * m.w.y * m.x.x - m.w.w * m.x.y * m.z.x;
	minor.y.w = m.x.x * m.z.y * m.w.z + m.z.x * m.w.y * m.x.z + m.w.x * m.x.y * m.z.z - m.x.z * m.z.y * m.w.x - m.z.z * m.w.y * m.x.x - m.w.z * m.x.y * m.z.x;

	minor.z.x = m.x.y * m.y.z * m.w.w + m.y.y * m.w.z * m.x.w + m.w.y * m.x.z * m.y.w - m.x.w * m.y.z * m.w.y - m.y.w * m.w.z * m.x.y - m.w.w * m.x.z * m.y.y;
	minor.z.y = m.x.x * m.y.z * m.w.w + m.y.x * m.w.z * m.x.w + m.w.x * m.x.z * m.y.w - m.x.w * m.y.z * m.w.x - m.y.w * m.w.z * m.x.x - m.w.w * m.x.z * m.y.x;
	minor.z.z = m.x.x * m.y.y * m.w.w + m.y.x * m.w.y * m.x.w + m.w.x * m.x.y * m.y.w - m.x.w * m.y.y * m.w.x - m.y.w * m.w.y * m.x.x - m.w.w * m.x.y * m.y.x;
	minor.z.w = m.x.x * m.y.y * m.w.z + m.y.x * m.w.y * m.x.z + m.w.x * m.x.y * m.y.z - m.x.z * m.y.y * m.w.x - m.y.z * m.w.y * m.x.x - m.w.z * m.x.y * m.y.x;

	minor.w.x = m.x.y * m.y.z * m.z.w + m.y.y * m.z.z * m.x.w + m.z.y * m.x.z * m.y.w - m.x.w * m.y.z * m.z.y - m.y.w * m.z.z * m.x.y - m.z.w * m.x.z * m.y.y;
	minor.w.y = m.x.x * m.y.z * m.z.w + m.y.x * m.z.z * m.x.w + m.z.x * m.x.z * m.y.w - m.x.w * m.y.z * m.z.x - m.y.w * m.z.z * m.x.x - m.z.w * m.x.z * m.y.x;
	minor.w.z = m.x.x * m.y.y * m.z.w + m.y.x * m.z.y * m.x.w + m.z.x * m.x.y * m.y.w - m.x.w * m.y.y * m.z.x - m.y.w * m.z.y * m.x.x - m.z.w * m.x.y * m.y.x;
	minor.w.w = m.x.x * m.y.y * m.z.z + m.y.x * m.z.y * m.x.z + m.z.x * m.x.y * m.y.z - m.x.z * m.y.y * m.z.x - m.y.z * m.z.y * m.x.x - m.z.z * m.x.y * m.y.x;

	return minor;
}

mat4 m4_cofactor(mat4 m)
{
	mat4 minor = m4_minor(m);
	mat4 result;

	result.x.x = minor.x.x;
	result.y.x = -minor.y.x;
	result.z.x = minor.z.x;
	result.w.x = -minor.w.x;

	result.x.y = -minor.x.y;
	result.y.y = minor.y.y;
	result.z.y = -minor.z.y;
	result.w.y = minor.w.y;

	result.x.z = minor.x.z;
	result.y.z = -minor.y.z;
	result.z.z = minor.z.z;
	result.w.z = -minor.w.z;

	result.x.w = -minor.x.w;
	result.y.w = minor.y.w;
	result.z.w = -minor.z.w;
	result.w.w = minor.w.w;

	return result;
}

float m4_determinant(mat4 m)
{
	float d1, d2, d3, d4;
	mat4 minor = m4_minor(m);
	d1 = m.x.x * minor.x.x;
	d2 = m.x.y * minor.x.y;
	d3 = m.x.z * minor.x.z;
	d4 = m.x.w * minor.x.w;
	float determinant = d1 - d2 + d3 - d4;
	printf("The determinant of matrix m1: %.2f\n", determinant);
	return determinant;
}

mat4 mat_inverse(mat4 m)
{
	float det = m4_determinant(m);
	if(det == 0)
	{
		printf("The matrix has no inverse matrix");
		return m;
	}

	float det_inv = 1 / m4_determinant(m);
	mat4 cofactor_mat = m4_cofactor(m);
	mat4 cof_transpose = mat_transpose(cofactor_mat);
	mat4 inverse = sca_mat_mul(cof_transpose, det_inv);
	return inverse;
}

mat4 mat_transpose(mat4 m)
{	
	vec4 v1 = {m.x.x, m.y.x, m.z.x, m.w.x};
	vec4 v2 = {m.x.y, m.y.y, m.z.y, m.w.y};
	vec4 v3 = {m.x.z, m.y.z, m.z.z, m.w.z};
	vec4 v4 = {m.x.w, m.y.w, m.z.w, m.w.w};	
	mat4 mt = {v1, v2, v3, v4};
	return mt;
}

vec4 mat_vec_mul(mat4 m, vec4 v)
{
	vec4 vm;
	vm.x = m.x.x * v.x + m.y.x * v.y + m.z.x * v.z + m.w.x * v.w;
	vm.y = m.x.y * v.x + m.y.y * v.y + m.z.y * v.z + m.w.y * v.w;
	vm.z = m.x.z * v.x + m.y.z * v.y + m.z.z * v.z + m.w.z * v.w;
	vm.w = m.x.w * v.x + m.y.w * v.y + m.z.w * v.z + m.w.w * v.w;	
	return vm;
}

/*mat4 scaling_mat(float x, float y, float z)
{
	vec4 xcol = {x, 0, 0, 0};
	vec4 ycol = {0, y, 0, 0};
	vec4 zcol = {0, 0, z, 0};
	vec4 wcol = {0 ,0 ,0, 1};
	mat4 sca_mat = {xcol, ycol, zcol, wcol};
	return sca_mat;
}*/

mat4 rotate_y_m(double angle)
{
	float cosine = cos(angle);
	float sine = sin(angle);
	vec4 xcol = {cosine, 0.0, -sine, 0.0};
	vec4 ycol = {0.0, 1.0, 0.0, 0.0};
	vec4 zcol = {sine, 0.0, cosine, 0.0};
	vec4 wcol = {0.0, 0.0, 0.0, 1.0};
	mat4 m = {xcol, ycol, zcol, wcol};
	return m;
}

mat4 rotate_z_m(double angle){
	float cosine = cos(angle);
	float sine = sin(angle);
	vec4 xcol = {cosine, sine, 0.0, 0.0};
	vec4 ycol = {-sine, cosine, 0.0, 0.0};
	vec4 zcol = {0.0, 0.0, 1.0, 0.0};
	vec4 wcol = {0.0, 0.0, 0.0, 1.0};
	mat4 m = {xcol, ycol, zcol, wcol};
	return m;
}

mat4 rotate_x_m(double angle){
	float cosine = cos(angle);
	float sine = sin(angle);
	vec4 xcol = {1.0, 0.0, 0.0, 0.0};
	vec4 ycol = {0.0, cosine, sine, 0.0};
	vec4 zcol = {0.0, -sine, cosine, 0.0};
	vec4 wcol = {0.0, 0.0, 0.0, 1.0};
	mat4 m = {xcol, ycol, zcol, wcol};
	return m;
}


