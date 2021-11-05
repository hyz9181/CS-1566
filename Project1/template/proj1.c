/*
 * template.c
 *
 * An OpenGL source code template.
 */


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <stdio.h>
#include <math.h>

#include "../mylib/initShader.h"
#include "../mylib/matrixlib.h"


#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
int length;
vec4 vertices[720000];
vec4 colors[720000];
double rad = M_PI / 180;
int num_vertices = 720000;

mat4 ctm = {{1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}};

GLuint ctm_location;

vec4 colors_generator()
{
    vec4 color;
    color.x = (float)rand() / (float)(RAND_MAX);
    color.y = (float)rand() / (float)(RAND_MAX);
    color.z = (float)rand() / (float)(RAND_MAX);
    color.w = 1.0;
    return color;
}

mat4 scaling_mat(float x, float y, float z)
{
    vec4 xcol = {x, 0, 0, 0};
    vec4 ycol = {0, y, 0, 0};
    vec4 zcol = {0, 0, z, 0};
    vec4 wcol = {0 ,0 ,0, 1};
    mat4 sca_mat = {xcol, ycol, zcol, wcol};
    return sca_mat;
}

void sphere()
{
    int v_index = 0;
    vec4 right = {1, 0, 0, 1};
    int vertical_angle = 10;
    vec4 prev, right_temp;
    int half_level = 90 / vertical_angle;

    for(int level = 0; level < half_level; level++) // generate the upper half sphere
    {
        for(int angle = 0; angle < 360; angle += 10) // generate a level of triangles to form a ring.
        {
            vec4 triangle1_first, triangle1_second, triangle1_third, triangle2_first, triangle2_second, triangle2_third;
            triangle1_first = right;

            float rad_degree = 10 * rad;
            mat4 rotate_y = rotate_y_m(rad_degree);
            mat4 rotate_z = rotate_z_m(vertical_angle * rad);

            triangle1_second = mat_vec_mul(rotate_y, triangle1_first);
            if(angle == 0)
            {
                triangle1_third = mat_vec_mul(rotate_z, triangle1_first);
                prev = triangle1_third;
                right_temp = triangle1_third;
            }
            else
            {
                triangle1_third = mat_vec_mul(rotate_y, prev);
                prev = triangle1_third;
            }
            //printf("first triangle first vertex %.2f, %.2f, %.2f, %.2f\n", triangle1_first.x, triangle1_first.y, triangle1_first.z, triangle1_first.w);
            //printf("first triangle second vertex %.2f, %.2f, %.2f, %.2f\n", triangle1_second.x, triangle1_second.y, triangle1_second.z, triangle1_second.w);
            //printf("first triangle third vertex %.2f, %.2f, %.2f, %.2f\n", triangle1_third.x, triangle1_third.y, triangle1_third.z, triangle1_third.w);

            vec4 curr = colors_generator();
            colors[v_index] = curr;
            vertices[v_index++] = triangle1_first;
            colors[v_index] = curr;
            vertices[v_index++] = triangle1_second;
            colors[v_index] = curr;
            vertices[v_index++] = triangle1_third;
            
            //printf("first triangle color %.2f, %.2f, %.2f, %.2f\n\n", curr.x, curr.y, curr.z, curr.w);

            triangle2_first = triangle1_second;
            triangle2_second = triangle1_third;
            triangle2_third = mat_vec_mul(rotate_y, triangle2_second);
            vec4 curr2 = colors_generator(); 
            colors[v_index] = curr2;
            vertices[v_index] = triangle2_first;
            colors[v_index+1] = curr2;
            vertices[v_index+1] = triangle2_third;
            colors[v_index+2] = curr2;
            vertices[v_index+2] = triangle2_second;
            
            v_index += 3;

            //printf("second triangle first vertex %.2f, %.2f, %.2f, %.2f\n", triangle2_first.x, triangle2_first.y, triangle2_first.z, triangle2_first.w);
            //printf("second triangle second vertex %.2f, %.2f, %.2f, %.2f\n", triangle2_second.x, triangle2_second.y, triangle2_second.z, triangle2_second.w);
            //printf("second triangle third vertex %.2f, %.2f, %.2f, %.2f\n\n", triangle2_third.x, triangle2_third.y, triangle2_third.z, triangle2_third.w);

            right.x = triangle1_second.x;
            right.y = triangle1_second.y;
            right.z = triangle1_second.z;
            right.w = 1;
        }
        
        //mat4 rotate_y_level = rotate_z_m(vertical_angle * rad);
        right = right_temp;
        //printf("new iteration level %d ", level);
        //vec_print(right);
        //printf("\n");
    }

    right.x = 1;
    right.y = 0;
    right.z = 0;
    right.w = 1;
    vertical_angle = -10;

    for(int level = 0; level < half_level; level++)
    {
        for(int angle = 0; angle < 360; angle += 10) // generate a level of triangles to form a ring.
        {
            vec4 triangle1_first, triangle1_second, triangle1_third, triangle2_first, triangle2_second, triangle2_third;
            triangle1_first = right;

            float rad_degree = 10 * rad;
            mat4 rotate_y = rotate_y_m(rad_degree);
            mat4 rotate_z = rotate_z_m(vertical_angle * rad);

            triangle1_second = mat_vec_mul(rotate_y, triangle1_first);
            if(angle == 0)
            {
                triangle1_third = mat_vec_mul(rotate_z, triangle1_first);
                prev = triangle1_third;
                right_temp = triangle1_third;
            }
            else
            {
                triangle1_third = mat_vec_mul(rotate_y, prev);
                prev = triangle1_third;
            }
            //printf("first triangle first vertex %.2f, %.2f, %.2f, %.2f\n", triangle1_first.x, triangle1_first.y, triangle1_first.z, triangle1_first.w);
            //printf("first triangle second vertex %.2f, %.2f, %.2f, %.2f\n", triangle1_second.x, triangle1_second.y, triangle1_second.z, triangle1_second.w);
            //printf("first triangle third vertex %.2f, %.2f, %.2f, %.2f\n", triangle1_third.x, triangle1_third.y, triangle1_third.z, triangle1_third.w);

            vec4 curr = colors_generator();
            colors[v_index] = curr;
            vertices[v_index++] = triangle1_first;
            colors[v_index] = curr;
            vertices[v_index++] = triangle1_third;
            colors[v_index] = curr;
            vertices[v_index++] = triangle1_second;
            
            //printf("first triangle color %.2f, %.2f, %.2f, %.2f\n\n", curr.x, curr.y, curr.z, curr.w);

            triangle2_first = triangle1_second;
            triangle2_second = triangle1_third;
            triangle2_third = mat_vec_mul(rotate_y, triangle2_second);
            vec4 curr2 = colors_generator(); 
            colors[v_index] = curr2;
            vertices[v_index] = triangle2_first;
            colors[v_index+1] = curr2;
            vertices[v_index+1] = triangle2_second;
            colors[v_index+2] = curr2;
            vertices[v_index+2] = triangle2_third;
            
            v_index += 3;

            //printf("second triangle first vertex %.2f, %.2f, %.2f, %.2f\n", triangle2_first.x, triangle2_first.y, triangle2_first.z, triangle2_first.w);
            //printf("second triangle second vertex %.2f, %.2f, %.2f, %.2f\n", triangle2_second.x, triangle2_second.y, triangle2_second.z, triangle2_second.w);
            //printf("second triangle third vertex %.2f, %.2f, %.2f, %.2f\n\n", triangle2_third.x, triangle2_third.y, triangle2_third.z, triangle2_third.w);

            right.x = triangle1_second.x;
            right.y = triangle1_second.y;
            right.z = triangle1_second.z;
            right.w = 1;
        }
        right = right_temp;
    }

    for(int i = 0; i < num_vertices; i += 3)
    {
        vec4 color_temp =  colors_generator();
        colors[i] = color_temp;
        colors[i+1] = color_temp;
        colors[i+2] = color_temp;
    }
}

float z_pos(float x, float y){
    return sqrt(256*256- x*x - y*y);
}

float x_1, y_1, z_1, x_2, y_2, z_2;
GLboolean if_rotate;

void mouse(int button, int state, int x, int y)
{
    mat4 scale;
    if(button == 3 || button == 4){
        if (button == 3){
            scale = scaling_mat(1.02, 1.02, 1.02);
            //printf("button 3, zoom in\n");
        }

        if(button == 4)
        {
            scale = scaling_mat(1/1.02, 1/1.02, 1/1.02);
            //printf("button 4, zoom out\n");
        }
        ctm = mat_mat_mul(scale, ctm);
        
        glutPostRedisplay();
    }

    else if(button == GLUT_LEFT_BUTTON){
        if(state == GLUT_DOWN){
            x_1 = x - 255; 
            y_1 = 255 - y;
            
            if(pow(x_1, 2) + pow(y_1, 2) > pow(256, 2)){
                if_rotate = GL_FALSE;
            }else{
                if_rotate = GL_TRUE;
                z_1 = z_pos(x_1, y_1);
            }
        }
        else if(state == GLUT_UP){
            
        }
    }
}

void calculate_rotation_m(){
    vec4 v1 = {x_1, y_1, z_1, 0.0};
    vec4 v2 = {x_2, y_2, z_2, 0.0};
    vec4 r_v = vec_cross_product(v1, v2);

    //rotation vector
    r_v = vec_normalize(r_v);
    // rotation angle
    float r_angle = vec_dot_product(v1, v2) / (vec_magnitude(v1) * vec_magnitude(v2));
    r_angle = acos(r_angle);
    //printf("r_angle %f\n", r_angle);
    float d = sqrt(pow(r_v.y, 2) + pow(r_v.z, 2));
    mat4 R_x = {{1, 0, 0, 0},
                {0, r_v.z / d, r_v.y / d, 0},
                {0, -r_v.y / d, r_v.z / d, 0},
                {0, 0, 0, 1}};
    mat4 R_y = {{d, 0, r_v.x, 0},
                {0, 1, 0, 0},
                {-r_v.x, 0, d, 0},
                {0, 0, 0, 1}};
    mat4 R_z = rotate_z_m(r_angle);
    mat4 M = mat_mat_mul(R_y, R_x);
    M = mat_mat_mul(R_z, M);
    M = mat_mat_mul(mat_transpose(R_y), M);
    M = mat_mat_mul(mat_transpose(R_x), M);

    ctm = mat_mat_mul(M, ctm);
}

void calculate_scale_test_mat()
{
    mat4 scale = rotate_x_m(2 * rad);
    //scale = mat_mat_mul(scale, rotate_x_m(10 * rad));
    ctm = mat_mat_mul(scale, ctm);
}

void motion(int x, int y){
    //printf("x is %i, y is %i\n", x, y);
    if(if_rotate){
        x_2 = x - 255;
        y_2 = 255 - y;
    
        z_2 = z_pos(x_2, y_2);
        if(pow(x_2, 2) + pow(y_2, 2) > pow(256, 2)){
            if_rotate = GL_FALSE;
        }
        if(if_rotate){
            calculate_rotation_m();
            //calculate_scale_test_mat();
            x_1 = x_2;
            y_1 = y_2;
            z_1 = z_2;
            //printf("after: x is %f, y is %f\n", x_1, y_1);
            glutPostRedisplay();
        }
    }
}

void init(void)
{
    sphere();
    //cone_generator();
    //readfile();
    //transform_matrix();
    GLuint program = initShader("vshader_ctm.glsl", "fshader_ctm.glsl");
    glUseProgram(program);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

    ctm_location = glGetUniformLocation(program, "ctm");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat*)&ctm);

    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	glutLeaveMainLoop();

    //glutPostRedisplay();
}

void reshape(int width, int height)
{
    glViewport(0, 0, 512, 512);
}

int main(int argc, char **argv)
{
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 1");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMotionFunc(motion); 
    glutMouseFunc(mouse);
    glutMainLoop();

    return 0;
}
