#ifndef OBJECT_H
#define OBJECT_H

#include <GL/gl.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/* Simple struct for storing models loaded */
typedef struct model {
	GLfloat* vertices;

	GLfloat* normals;
	GLfloat* texture_coords;
	GLint* indices;
	
	GLfloat* colours;

	uint n_vertices;
	uint n_indices;

	GLuint vertex_array_object;
	GLuint buffer_objects[5];

} model;

model** load_model(char* model_file);

void draw_model(model* m, GLuint shaders, char* gpu_vertex, char* gpu_normal,
		char* gpu_texcoord, char* gpu_colour);


#endif /* OBJECT_H */
