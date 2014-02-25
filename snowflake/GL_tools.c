#include "GL_tools.h"

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
		printf("%s", error);
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
		printf("%s", error);
		free(error);
	}
}

GLuint init_shaders(char* vertex_shader_name, char* geometry_shader_name,
char* fragment_shader_name)
{
	GLuint p, v, g, f;

	p = glCreateProgram();
	v = read_shader(vertex_shader_name, VERTEX);
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
