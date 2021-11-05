#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../mylib/matrixlib.h"

void vec_print_helper(vec4 v)
{
	printf("vector print %.2f, %.2f, %.2f, %.2f\n", v.x, v.y, v.z, v.w);
}

int main()
{
	FILE * myfile;
	myfile = fopen("cube.txt", "r");

	int length;
	const char * s = ",";
	char * token;
	if(myfile != NULL)
	{
		fscanf(myfile, "%d", &length);
	}

	vec4 vertices_temp[length];
	float vertices_index[length * 5];
	if(myfile != NULL)
	{
		char line[39]
;		int i;
		for(i = 0; i < length * 5; i = i + 1)
		{
			while(fgets(line, 39, myfile) != NULL)
			{
				token = strtok(line, s);
				while(token != NULL)
				{
					//printf("array size 3 %d\n", length);
					float temp = atof(token);
					token = strtok(NULL, s);
					//if(temp != 0)
					//{
						vertices_index[i] = temp;
						printf("temp %f insert %f with i value %d\n", temp, vertices_index[i], i);
					//}
				
}				printf("---------\n");
			}
			//printf("the first vertex value %.2f\n", vertices_index[1]);
		}
	}

	for(int j = 0; j < length * 5; j+=5)
	{
		printf("j value %d\n", j);
		float t = vertices_index[j];
		float tx = vertices_index[j + 1];
		float ty = vertices_index[j + 2];
		float tz = vertices_index[j + 3];
		float tw = vertices_index[j + 4];
		vec4 temp_vec = {tx, ty, tz, tw};
		printf("insert print %.2f %.2f, %.2f, %.2f, %.2f\n", t, tx, ty, tz, tw);
		vertices_temp[j / 5] = temp_vec;
	}
	
	for(int k = 0; k < length; k++)
	{
		vec4 toprint = vertices_temp[k];
		vec_print_helper(toprint);
	}

    printf("array size %d\n", length);
    for(int i = 0; i < length; i++)
    {
    	vec4 temp = vertices_temp[i];
    	//printf("%f %f %f %f\n", temp.x, temp.y, temp.z, temp.w);
    }

	fclose(myfile);
	return 0;
}