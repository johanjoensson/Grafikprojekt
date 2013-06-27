/*****************************************************************************
 * Generate the Koch snowflake beginnning with a single triangle.	     *
 * Upload a single triangle to the shaders, then use the geometry shader to  *
 * generate the snowflake						     *
 *									     *
 * By:			Johan Jönsson					     *
 * First created:	2013-06-27					     *
 * Latest revision:							     *
 *****************************************************************************/

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "SDL_util.h"

typedef enum SHADER_TYPE_t{
	VERTEX,
	GEOMETRY,
	FRAGMENT
}SHADER_TYPE;

char* read_file(char* filename)
{
	FILE *f;
	long length;
	char* file_contents;

	f = fopen(filename, "rb");
	if(f == NULL){
		return NULL;
	}
	fseek(f, 0, SEEK_END);
	length = ftell(f);
	file_contents = malloc(sizeof(char)*length + 1);
	fseek(f, 0, SEEK_SET);
	fread(file_contents, length, 1, f);
	fclose(f);
	file_contents[length] = '\0';

	return file_contents;
}

GLuint read_shader(char* shader_file, SHADER_TYPE type)
{
	GLuint shader;
	char* shader_contents;

	switch(type){
		case VERTEX:
			shader = glCreateShader(GL_VERTEX_SHADER);
			break;
		case GEOMETRY:
			shader = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		case FRAGMENT:
			shader = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		default:
			return 0;
	}

	shader_contents = read_file(shader_file);
	const char* tmp = shader_contents;
	glShaderSource(shader, 1, &tmp, NULL);
	free(shader_contents);

	return shader;
}

void print_shader_error(GLuint shader)
{
	int error_length = 0;
	int written_chars = 0;
	char* error;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &error_length);

	if(error_length > 0){
		error = malloc(sizeof(char)*error_length);
		glGetShaderInfoLog(shader, error_length, &written_chars, error);
		printf("%s\n", error);
		free(error);
	}
}

void print_program_error(GLuint program)
{
	int error_length = 0;
	int written_chars = 0;
	char* error;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &error_length);

	if(error_length > 0){
		error = malloc(sizeof(char)*error_length);
		glGetProgramInfoLog(program, error_length, &written_chars, error);
		printf("%s\n", error);
		free(error);
	}
}

GLuint init_shaders(char* vertex_shader_name, char* geometry_shader_name,
char* fragment_shader_name)
{
	GLuint p, v, g, f;

	p = glCreateProgram();

	v =	read_shader(vertex_shader_name, VERTEX);
	g = read_shader(geometry_shader_name, GEOMETRY);
	f = read_shader(fragment_shader_name, FRAGMENT);
	
	glCompileShader(v);
	print_shader_error(v);

	glCompileShader(g);
	print_shader_error(g);

	glCompileShader(f);
	print_shader_error(f);

	glAttachShader(p,v);
	glAttachShader(p,g);
	glAttachShader(p,f);

	glLinkProgram(p);

	print_program_error(p);

	return p;
}

float vertices[] = {	0.75, 0.0, 0.0, 1.0,
						0.0, 0.75, 0.0, 1.0,
						0.0, 0.0, 0.0, 1.0	};

float colours[] = {		1.0, 0.0, 0.0, 1.0,
						0.0, 1.0, 0.0, 1.0,
						0.0, 0.0, 1.0, 1.0	};

void display(GLuint program)
{
	glUseProgram(program);
	glClearColor(0.3, 0.0, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	GLuint vertex_array_object_id;
	GLuint vao_buffers[2];

	glGenVertexArrays(1, &vertex_array_object_id);
	glGenBuffers(2, vao_buffers);

	glBindBuffer(GL_ARRAY_BUFFER, vao_buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_position"));
	glVertexAttribPointer(glGetAttribLocation(program, "in_position"), 4, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vao_buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_colour"));
	glVertexAttribPointer(glGetAttribLocation(program, "in_colour"), 4, GL_FLOAT, 0, 0, 0);

	glBindVertexArray(vertex_array_object_id);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	SDL_GL_SwapBuffers();
}

int main(int argc, char** argv)
{
	if(argc != 4){
		fprintf(stderr, "Please supply 3 arguments!\nVertex-, Geometry- and \
Fragment-shader\n");
		return -1;
	}

	GLuint program = init_shaders(argv[1], argv[2], argv[3]);
	init_SDL();
//	glEnable(GL_DEPTH_TEST);
	display(program);
	while(1){
	}

	return program;
}
