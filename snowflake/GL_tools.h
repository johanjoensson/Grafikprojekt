#ifndef GL_TOOLS_H
#define GL_TOOLS_H

#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

typedef enum SHADER_TYPE_t{
	VERTEX,
	GEOMETRY,
	FRAGMENT
}SHADER_TYPE;

GLuint init_shaders(char* vertex_shader_name, char* geometry_shader_name,
char* fragment_shader_name);

#endif /* GL_TOOLS_H */
