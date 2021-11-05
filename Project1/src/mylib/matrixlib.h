/*
 * Yuanzhe Huang yuh94@pitt.edu
 * CS-1566 Lab 02 
 * To compile this file, please type "make matrixlib" in command line
 */

typedef struct 
{
	float x;
	float y;
	float z;
	float w;
}vec4;

typedef struct 
{
	vec4 x;
	vec4 y;
	vec4 z;
	vec4 w;
}mat4;

void vec_print(vec4 v);

vec4 sca_vec_mul(vec4 v, float a);

vec4 vec_vec_add(vec4 v1, vec4 v2);

vec4 vec_vec_sub(vec4 v1, vec4 v2);

float vec_magnitude(vec4 v);

vec4 vec_normalize(vec4 v);

float vec_dot_product(vec4 v1, vec4 v2);

vec4 vec_cross_product(vec4 v1, vec4 v2);

void mat_print(mat4 m);

mat4 sca_mat_mul(mat4 m, float a);

mat4 mat_mat_add(mat4 m1, mat4 m2);

mat4 mat_mat_sub(mat4 m1, mat4 m2);

mat4 mat_mat_mul(mat4 m1, mat4 m2);

mat4 m4_minor(mat4 m);

mat4 m4_cofactor(mat4 m);

float m4_determinant(mat4 m);

mat4 mat_inverse(mat4 m);

mat4 mat_transpose(mat4 m);

vec4 mat_vec_mul(mat4 m, vec4 v);

//mat4 scaling_mat(float x, float y, float z)

mat4 rotate_y_m(double angle);

mat4 rotate_z_m(double angle);

mat4 rotate_x_m(double angle);


