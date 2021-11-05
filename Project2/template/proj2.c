
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "../mylib/initShader.h"
#include "../mylib/matrixlib.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

typedef struct
{
    GLfloat x;
    GLfloat y;
} vec2;

vec4 vertices[7200000];
vec4 colors[7200000];
vec2 map_vertex[7200000];
vec2 map_coords[7200000];
double rad = M_PI / 180;
//int num_vertices = 72000000;
int num_vertices;
int num_vertices_count;
int num_face;
int num_texture_vertices;
int num_texture_face;

int flag_global;
int first_choices;
int has_colors;
char input_file[100];
char filename_path[100];

FILE *myfile;
FILE *fp;

mat4 ctm = {{1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}};

GLuint ctm_location;

mat4 scaling_mat(float x, float y, float z)
{
    vec4 xcol = {x, 0, 0, 0};
    vec4 ycol = {0, y, 0, 0};
    vec4 zcol = {0, 0, z, 0};
    vec4 wcol = {0 ,0 ,0, 1};
    mat4 sca_mat = {xcol, ycol, zcol, wcol};
    return sca_mat;
}

vec4 colors_generator()
{
    vec4 color;
    color.x = (float)rand() / (float)(RAND_MAX);
    color.y = (float)rand() / (float)(RAND_MAX);
    color.z = (float)rand() / (float)(RAND_MAX);
    color.w = 1.0;
    return color;
}

char * substring(char *string, int position, int length)
{
    char *p;
    int c;
    p = malloc(length + 1);

    for(c = 0; c < length; c++)
    {
        *(p+c) = *(string + position - 1);
        string++;
    }
    *(p+c) = '\0';
    return p;
}

void readfile()
{
    myfile = fopen(input_file, "r");
    if(myfile == NULL)
        printf("File not found, please terminate program and enter correct file again!\n");
    char target1[] = "element vertex";
    char target2[] = "element face";
    char target3[] = "element multi_texture_vertex";
    char target4[] = "element multi_texture_face";
    char target5[] = "end_header\n";
    
    int size = 10;
    char *string = (char *)malloc(size);
    int line_count = 0;
    int flag = 0;
    while(getline(&string, &size, myfile) != -1)
    {
        if(flag == 1)
            line_count++;
        char *cmp1 = substring(string, 1, 14);
        char *cmp2 = substring(string, 1, 12);
        char *cmp3 = substring(string, 1, 28);
        char *cmp4 = substring(string, 1, 26);
        if(strcmp(target1, cmp1) == 0)
        {
            char *num_ver = substring(string, 16, 21);
            num_vertices_count = atoi(num_ver);
            flag = 1;
        }
        if(strcmp(target2, cmp2) == 0)
        {
            char *num_faces = substring(string, 14, 21);
            num_face = atoi(num_faces);
            num_vertices = num_face * 3;
            flag = 0;
        }
        if(strcmp(target3, cmp3) == 0)
        {
            char *num_text_ver = substring(string, 30, 35);
            num_texture_vertices = atoi(num_text_ver);
        }
        if(strcmp(target4, cmp4) == 0)
        {
            char *num_text_face = substring(string, 28, 33);
            num_texture_face = atoi(num_text_face);
        }
        if(strcmp(target5, string) == 0)
            break;

        free(cmp1);
        free(cmp2);
        free(cmp3);
        free(cmp4);
    }
    if(line_count == 7)
        flag_global = 1;

    vec4 *vertex_tmp = malloc(sizeof(vec4) * num_vertices_count);
    vec4 *colors_tmp = malloc(sizeof(vec4) * num_vertices_count);

    float *coords_read;// = malloc(sizeof(float) * 3 * num_vertices_count);
    unsigned char *colors_read;// = malloc(sizeof(unsigned char) * 3 * num_vertices_count);
    if(line_count == 7)
    {
        has_colors = 1;
        coords_read = malloc(sizeof(float) * 3);
        colors_read = malloc(sizeof(unsigned char) * 3);
        for(int i = 0; i < num_vertices_count; i++)
        {
            fread(coords_read, sizeof(float) * 3, 1, myfile);
            vec4 coords_curr;
            coords_curr.x = coords_read[0];
            coords_curr.y = coords_read[1];
            coords_curr.z = coords_read[2];
            coords_curr.w = 1;
            vertex_tmp[i] = coords_curr;
            //printf("seg fault 2 %d\n", i);
            fread(colors_read, sizeof(unsigned char) * 3, 1, myfile);
            vec4 color_curr;
            unsigned char first, second, third;
            first = colors_read[0];
            second = colors_read[1];
            third = colors_read[2];
        
            color_curr.x = colors_read[0];
            color_curr.y = colors_read[1];
            color_curr.z = colors_read[2];
            color_curr.w = 1;
            color_curr.x = color_curr.x / 255;
            color_curr.y = color_curr.y / 255;
            color_curr.z = color_curr.z / 255;
            colors_tmp[i] = color_curr;
            //printf("seg fault 3 %d\n", i);
            
        }  
    }
    else
    {
        coords_read = malloc(sizeof(float) * 3 * num_vertices_count);
        fread(coords_read, sizeof(float) * 3, num_vertices_count, myfile);
    }

    //printf("first vertex readin x %f y %f z %f\n", coords_read[0], coords_read[1], coords_read[2]);
    //printf("first color readin x %d y %d z %d\n", colors_read[0], colors_read[1], colors_read[2]);
    if(line_count != 7)
    {
        has_colors = 0;
        int float_index = 0;
        for(int i = 0; i < num_vertices_count; i++)
        {
            vec4 v_temp;
            v_temp.x = coords_read[float_index];
            v_temp.y = coords_read[float_index+1];
            v_temp.z = coords_read[float_index+2];
            v_temp.w = 1;
            vertex_tmp[i] = v_temp;
            float_index += 3;
        }
        int index = 0;
        for(int i = 0; i < num_face; i++)
        {
            unsigned char *face_vertex_num = malloc(sizeof(unsigned char));
            fread(face_vertex_num, sizeof(unsigned char), 1, myfile);
            int *vertex_index = malloc(sizeof(int) * 3);
            fread(vertex_index, 3*sizeof(int), 1, myfile);
            free(face_vertex_num);

            vec4 color_temp = colors_generator();
            colors[index] = color_temp;
            vertices[index++] = vertex_tmp[vertex_index[0]];
            colors[index] = color_temp;
            vertices[index++] = vertex_tmp[vertex_index[1]];
            colors[index] = color_temp;
            vertices[index++] = vertex_tmp[vertex_index[2]];
            free(vertex_index);
        }
        
        /* ---------- read in the texture information ----------*/
        for(int i = 0; i < num_texture_vertices; i++)
        {
            unsigned char *tx = malloc(sizeof(unsigned char));
            fread(tx, sizeof(unsigned char), 1, myfile);
            float *t_coords = malloc(sizeof(float) * 2);
            fread(t_coords, 2 * sizeof(float), 1, myfile);
            map_vertex[i].x = t_coords[0];
            map_vertex[i].y = t_coords[1];

        }

        int t_index = 0;
        for(int i = 0; i < num_texture_face; i++)
        {
            unsigned char *tx = malloc(sizeof(unsigned char));
            unsigned int *tn = malloc(sizeof(unsigned int));
            fread(tx, sizeof(unsigned char), 1, myfile);
            fread(tn, sizeof(unsigned int), 1, myfile);
            unsigned char *utx = malloc(sizeof(unsigned char));
            fread(utx, sizeof(unsigned char), 1, myfile);
            int *texture_offset = malloc(sizeof(int) * 3);
            fread(texture_offset, sizeof(int) * 3, 1, myfile);
            map_coords[t_index] = map_vertex[texture_offset[0]];
            map_coords[t_index+1] = map_vertex[texture_offset[1]];
            map_coords[t_index+2] = map_vertex[texture_offset[2]];
            t_index += 3;
        }
    }
    else
    {
        int index = 0;
        for(int i = 0; i < num_face; i++)
        {
            unsigned char *face_vertex_num = malloc(sizeof(unsigned char));
            fread(face_vertex_num, sizeof(unsigned char), 1, myfile);
            int *vertex_index = malloc(sizeof(int) * 3);
            fread(vertex_index, 3*sizeof(int), 1, myfile);
            free(face_vertex_num);

            //vec4 color_temp = colors_generator();
            colors[index] = colors_tmp[vertex_index[0]];
            vertices[index++] = vertex_tmp[vertex_index[0]];
            colors[index] = colors_tmp[vertex_index[1]];
            vertices[index++] = vertex_tmp[vertex_index[1]];
            colors[index] = colors_tmp[vertex_index[2]];
            vertices[index++] = vertex_tmp[vertex_index[2]];
            free(vertex_index);
        }
    }
}

void sphere()
{
    num_vertices = 7200000;
    has_colors = 0;
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

void transform_matrix()
{
    float right, left, top, bottom, near, far;
    int array_length = sizeof(vertices) / sizeof(vertices[0]);

    for(int i = 0; i < num_face * 3; i++)
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

    GLfloat mid_x = (right + left) / 2;
    GLfloat mid_y = (top + bottom) / 2;
    GLfloat mid_z = (near  + far) / 2;
    
    vec4 center = {mid_x, mid_y, mid_z, 1};
    vec4 origin = {0, 0, 0, 1};
    vec4 shift_vector = vec_vec_sub(origin, center);
    shift_vector.w = 1;

    mat4 scale = scaling_mat(0.001, 0.001, 0.001);

    vec4 tmx = {1, 0, 0, 0};
    vec4 tmy = {0, 1, 0, 0};
    vec4 tmz = {0, 0, 1, 0};
    vec4 tmw = {-mid_x, -mid_y, -mid_z, 1};
    mat4 shift = {tmx, tmy, tmz, tmw};

    for(int i = 0; i < num_face*3; i++)
    {
        vertices[i] = mat_vec_mul(shift, vertices[i]);
        vertices[i] = mat_vec_mul(scale, vertices[i]);
        //vec_print(vertices[i]);
    }
}


vec2 tex_coords[6] = {{0.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}, {0.0, 1.0}, {1.0, 0.0}, {0.0, 0.0}};
//vec2 map_coords[72000];
void init(void)
{
    int width, height;
    //GLubyte *my_texels;
    if(first_choices == 1)
    {
        sphere();
        width = 320;
        height = 320;
        //my_texels = malloc(width * height * 3);
        GLubyte my_texels[width][height][3];

        FILE *fp = fopen("texture01.raw", "r");
        fread(my_texels, width * height * 3, 1, fp);
        fclose(fp);

        for(int i = 0; i < num_vertices; i++)
        {
            map_coords[i].x = vertices[i].x / 4 + 0.25;
            map_coords[i].y = vertices[i].y / -4 + 0.25;
        }

        GLuint program = initShader("vshader.glsl", "fshader.glsl");
        glUseProgram(program);

        if(has_colors)
            glUniform1i(glGetUniformLocation(program, "use_color"), 1);
        else
            glUniform1i(glGetUniformLocation(program, "use_color"), 0);
        

        GLuint mytex[1];
        glGenTextures(1, mytex);
        glBindTexture(GL_TEXTURE_2D, mytex[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, my_texels);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

        int param;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &param);

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(map_coords), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), sizeof(map_coords), map_coords);

        GLuint vPosition = glGetAttribLocation(program, "vPosition");
        glEnableVertexAttribArray(vPosition);
        glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

        GLuint vColor = glGetAttribLocation(program, "vColor");
        glEnableVertexAttribArray(vColor);
        glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

        GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
        glEnableVertexAttribArray(vTexCoord);
        glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + (sizeof(vertices) + sizeof(colors)));

        GLuint texture_location = glGetUniformLocation(program, "texture");
        glUniform1i(texture_location, 0);

        printf("texture_location: %i\n", texture_location);

        ctm_location = glGetUniformLocation(program, "ctm");

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glDepthRange(1,0);
    }
    else if(first_choices == 2)
    {
        readfile();
        transform_matrix();

        int width = 1024;
        int height = 1024;
        GLubyte my_texels[width][height][3];
        //my_texels = malloc(width * height * 3);
        if(has_colors == 0)
        {
            FILE *fp = fopen(filename_path, "r");
            fread(my_texels, width * height * 3, 1, fp);
            fclose(fp);
        }

        GLuint program = initShader("vshader.glsl", "fshader.glsl");
        glUseProgram(program);

        if(has_colors)
            glUniform1i(glGetUniformLocation(program, "use_color"), 1);
        else
            glUniform1i(glGetUniformLocation(program, "use_color"), 0);
        

        GLuint mytex[1];
        glGenTextures(1, mytex);
        glBindTexture(GL_TEXTURE_2D, mytex[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, my_texels);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

        int param;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &param);

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(map_coords), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), sizeof(map_coords), map_coords);

        GLuint vPosition = glGetAttribLocation(program, "vPosition");
        glEnableVertexAttribArray(vPosition);
        glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

        GLuint vColor = glGetAttribLocation(program, "vColor");
        glEnableVertexAttribArray(vColor);
        glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

        GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
        glEnableVertexAttribArray(vTexCoord);
        glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + (sizeof(vertices) + sizeof(colors)));

        GLuint texture_location = glGetUniformLocation(program, "texture");
        glUniform1i(texture_location, 0);

        printf("texture_location: %i\n", texture_location);

        ctm_location = glGetUniformLocation(program, "ctm");

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glDepthRange(1,0);
    }
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
	exit(0);
}

int main(int argc, char **argv)
{
    char first_choice[5];
    printf("Please Choose which one to display\n");
    printf("1. Enter 1 to generate sphere, apply texture, and display\n");
    printf("2. Enter 2 to input a specific file name and display\n");
    printf("3. Enter other number to terminate the program\n");
    gets(first_choice);
    first_choices = atoi(first_choice);

    if(first_choices == 2)
    {
        char *filename = malloc(sizeof(char) * 50);
        //char filename_path[50];
        printf("Enter the file name that you want to read and display.\n");
        gets(input_file);
        strcpy(filename, input_file);
        strcat(input_file, "/");
        //strcat(input_file, input_file);
        strcat(input_file, filename);
        strcpy(filename_path, input_file);
        strcat(input_file, ".ply");
        strcat(filename_path, ".data");
    }
    if(first_choices == 3)
        return 0;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Project 2");
    glewInit();
    init();
    printf("init test\n");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion); 
    glutMouseFunc(mouse);
    glutMainLoop();

    return 0;
}
