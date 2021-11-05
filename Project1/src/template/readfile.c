#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <stdio.h>
#include <math.h>

#include "../mylib/initShader.h"
#include "../mylib/matrixlib.h"


#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
int length;
vec4 vertices[7200000];
vec4 colors[7200000];
double rad = M_PI / 180;
int num_vertices = 7200000;

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

void readfile()
{
    FILE * myfile;
    //myfile = fopen("menger_sponge.txt", "r");
    myfile = fopen("falcon.txt", "r");
    //int length;
    if(myfile != NULL)
    {
        fscanf(myfile, "%d\n", &length);
    }
    printf("length %d\n", length);
    //length * sizeof(vec4);
    //vec4 *vertices_read =(vec4 *)malloc(sizeof(vec4) * length);
    //vertices = vertices_read;

    for(int i = 0; i < length; i++)
    {
        fscanf(myfile, "%f,%f,%f,%f\n", &vertices[i].x, &vertices[i].y, &vertices[i].z, &vertices[i].w);
        //printf("test %d\n", i);
        vec_print(vertices[i]);
        if((i + 1) % 3 == 0)
        {
            vec4 color_temp = colors_generator();
            colors[i] = color_temp;
            colors[i-1] = color_temp;
            colors[i-2] = color_temp;
        }
    }

    /*for(int i = 0; i < length; i++)
    {
        vec_print(vertices[i]);
        vec_print(colors[i]);
    }*/
    fclose(myfile);
}

void transform_matrix()
{
    float right, left, top, bottom, near, far;
    int array_length = sizeof(vertices) / sizeof(vertices[0]);
    for(int i = 0; i < array_length; i++)
    {
        if(vertices[i].x > right)
            right = vertices[i].x;
        if(vertices[i].x < left)
            left = vertices[i].x;
        if(vertices[i].y > top)
            top = vertices[i].y;
        if(vertices[i].y < bottom)
            bottom = vertices[i].y;
        if(vertices[i].z > near)
            near = vertices[i].z;
        if(vertices[i].z < far)
            far = vertices[i].z;
    }

    printf("right %.2f left %.2f top %.2f bottom %.2f near %.2f far %.2f\n", right, left, top, bottom, near, far);
    GLfloat mid_x = (right + left) / 2;
    GLfloat mid_y = (top + bottom) / 2;
    GLfloat mid_z = (near  + far) / 2;
    printf("mid x %.2f mid y %.2f mid z %.2f\n", mid_x, mid_y, mid_z);
    vec4 center = {mid_x, mid_y, mid_z, 1};
    vec4 origin = {0, 0, 0, 1};
    vec4 shift_vector = vec_vec_sub(origin, center);
    shift_vector.w = 1;

    /*vec4 smx = {2 / (right - left), 0, 0, 0};
    vec4 smy = {0, 2 / (top - bottom), 0, 0};
    vec4 smz = {0, 0, 2 / (near - far), 0};
    vec4 smw = {0, 0, 0, 1};
    mat4 scale = {smx, smy, smz, smw};*/
    mat4 scale = scaling_mat(0.01, 0.01, 0.01);

    vec4 tmx = {1, 0, 0, 0};
    vec4 tmy = {0, 1, 0, 0};
    vec4 tmz = {0, 0, 1, 0};
    //vec4 tmw = {-mid_x, -mid_y, -mid_z, 1};
    vec4 tmw = shift_vector;
    mat4 shift = {tmx, tmy, tmz, tmw};
    printf("translation matrix\n");
    mat_print(shift);

    mat4 ortho = mat_mat_mul(scale, shift);
    mat_print(scale);
    mat_print(shift);
    mat_print(ortho);
    vec4 ox = {0.04, 0, 0, 0};
    vec4 oy = {0, 0.03, 0, 0};
    vec4 oz = {0, 0, 0.1, 0};
    vec4 ow = {1.1,-0.9,-1,1};

    mat4 ortho_test = {ox, oy, oz, ow};

    for(int i = 0; i < length; i++)
    {
        vertices[i] = mat_vec_mul(shift, vertices[i]);
        vertices[i] = mat_vec_mul(scale, vertices[i]);
        //vertices[i] = mat_vec_mul(shift, vertices[i]);
        //vertices[i] = mat_vec_mul(scale, vertices[i]);
        vec_print(vertices[i]);
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
    vec_print(v1);
    vec_print(v2);
    vec_print(r_v);
    printf("\n");
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
        /*if(x < 255.5)
            x_2 = -(GLfloat)((x / 255.5) - 1);
        else
            x_2 = -(GLfloat)((x - 255.5) / 255.5);

        if(y < 255.5)
            y_2 = -(GLfloat)((y / 255.5) - 1);
        else
            y_2 = -(GLfloat)((y - 255.5) / 255.5);*/
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
    readfile();
    transform_matrix();
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