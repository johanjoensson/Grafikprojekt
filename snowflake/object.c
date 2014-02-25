#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <assert.h>

#include "object.h"

model*  extract_model(struct aiMesh mesh)
{
	model *res = malloc(sizeof(model));

	uint32_t n_indices = 0;
	uint32_t n_vertices = mesh.mNumVertices;

	for(uint32_t i = 0; i < mesh.mNumFaces; i++){
		n_indices += mesh.mFaces[i].mNumIndices;
	}

	/* 
	 * Allocate all the indices, also make sure that the
	 * triangulation worked properly 
	 */
	res->indices = malloc(sizeof(unsigned int)*n_indices);
	assert(n_indices == 3*mesh.mNumFaces);

	res->n_vertices = n_vertices;

	res->vertices = malloc(sizeof(GLfloat)*3*mesh.mNumVertices);
	res->texture_coords = malloc(sizeof(GLfloat)*3*mesh.mNumVertices);
	res->normals = malloc(sizeof(GLfloat)*3*mesh.mNumVertices);

	res->colours = malloc(sizeof(GLfloat)*4*mesh.mNumVertices);
	
	if(mesh.mVertices != NULL){
		for(uint32_t i = 0; i < mesh.mNumVertices; i++){
			res->vertices[3*i + 0] = mesh.mVertices[i].x;
			res->vertices[3*i + 1] = mesh.mVertices[i].y;
			res->vertices[3*i + 2] = mesh.mVertices[i].z;
		}
	}
	if(mesh.mNormals != NULL){
		for(uint32_t i = 0; i < mesh.mNumVertices; i++){
			res->normals[3*i + 0] = mesh.mNormals[i].x;
			res->normals[3*i + 1] = mesh.mNormals[i].y;
			res->normals[3*i + 2] = mesh.mNormals[i].z;
		}
	}
	if(mesh.mColors[0] != NULL){
		/* RGBA colours */
		for(uint32_t i = 0; i < mesh.mNumVertices; i++){
			res->colours[4*i + 0] = mesh.mColors[0][i].r;
			res->colours[4*i + 1] = mesh.mColors[0][i].g;
			res->colours[4*i + 2] = mesh.mColors[0][i].b;
			res->colours[4*i + 3] = mesh.mColors[0][i].a;
		}
	}
	if(mesh.mTextureCoords[0] != NULL){
		for(uint32_t i = 0; i < mesh.mNumVertices; i++){
			res->texture_coords[3*i + 0] = mesh.mTextureCoords[0][i].x;
			res->texture_coords[3*i + 1] = mesh.mTextureCoords[0][i].y;
			res->texture_coords[3*i + 2] = mesh.mTextureCoords[0][i].z;
		}

	}

	if(mesh.mFaces != NULL){
		for(uint32_t i = 0; i < mesh.mNumFaces; i++){
			for(uint32_t j = 0; j < mesh.mFaces[i].mNumIndices;
					j++){
				res->indices[i + j] =
					mesh.mFaces[i].mIndices[j];
			}
		}

	}



	return res;
}

void generate_vao(model *m)
{
	glGenVertexArrays(1, &m->vertex_array_object);
	glGenBuffers(5, m->buffer_objects);

	glBindVertexArray(m->vertex_array_object);

	/* Vertex buffer data */
	glBindBuffer(GL_ARRAY_BUFFER, m->buffer_objects[0]);
	glBufferData(GL_ARRAY_BUFFER, m->n_vertices*3*sizeof(GLfloat),
			m->vertices, GL_STATIC_DRAW);

	/* Normal buffer data */
	glBindBuffer(GL_ARRAY_BUFFER, m->buffer_objects[1]);
	glBufferData(GL_ARRAY_BUFFER, m->n_vertices*3*sizeof(GLfloat),
			m->normals, GL_STATIC_DRAW);
	
	if(m->texture_coords != NULL){
		glBindBuffer(GL_ARRAY_BUFFER, m->buffer_objects[2]);
		glBufferData(GL_ARRAY_BUFFER, m->n_vertices*3*sizeof(GLfloat),
			m->texture_coords, GL_STATIC_DRAW);
	
	}
	if(m->colours != NULL){
		glBindBuffer(GL_ARRAY_BUFFER, m->buffer_objects[3]);
		glBufferData(GL_ARRAY_BUFFER, m->n_vertices*4*sizeof(GLfloat),
			m->colours, GL_STATIC_DRAW);
	
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->buffer_objects[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->n_indices*sizeof(uint),
	m->indices, GL_STATIC_DRAW);

}

void draw_model(model* m, GLuint shaders, char* gpu_vertex, char* gpu_normal,
		char* gpu_texcoord, char* gpu_colour)
{
	if(m == NULL){
		return;
	}
	
	GLint gpu_location;

	// Vertices
	glBindVertexArray(m->vertex_array_object);

	glBindBuffer(GL_ARRAY_BUFFER, m->buffer_objects[0]);
	gpu_location = glGetAttribLocation(shaders, gpu_vertex);

	if(gpu_location >= 0){
		glVertexAttribPointer(gpu_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(gpu_location);
	}else{
		fprintf(stderr, "Warning: draw_model - %s not found in shader\
		program\n", gpu_vertex);
	}

	if(gpu_normal != NULL){
		gpu_location = glGetAttribLocation(shaders, gpu_normal);
		if(gpu_location >= 0){
			glBindBuffer(GL_ARRAY_BUFFER, m->buffer_objects[1]);
			glVertexAttribPointer(gpu_location, 3, GL_FLOAT,
					GL_FALSE, 0, 0);
			glEnableVertexAttribArray(gpu_location);
		} else {
			fprintf(stderr, "Warning: draw_model - %s not found in\
			shader program\n", gpu_normal);
		}
	}

	if(gpu_texcoord != NULL && m->texture_coords != NULL){
		gpu_location = glGetAttribLocation(shaders, gpu_texcoord);
		if(gpu_location >= 0){
			glBindBuffer(GL_ARRAY_BUFFER, m->buffer_objects[2]);
			glVertexAttribPointer(gpu_location, 3, GL_FLOAT,
					GL_FALSE, 0, 0);
			glEnableVertexAttribArray(gpu_location);
		} else {
			fprintf(stderr, "Warning: draw_model - %s not found in\
					shader program\n", gpu_texcoord);
		}
	}
	if(gpu_colour != NULL){
		gpu_location = glGetAttribLocation(shaders, gpu_colour);
		if(gpu_location >= 0){
			glBindBuffer(GL_ARRAY_BUFFER, m->buffer_objects[3]);
			glVertexAttribPointer(gpu_location, 3, GL_FLOAT,
					GL_FALSE, 0, 0);
			glEnableVertexAttribArray(gpu_location);
		} else {
			fprintf(stderr, "Warning: draw_model - %s not found in\
					shader program\n", gpu_colour);
		}
	}

	glDrawElements(GL_TRIANGLES, m->n_indices, GL_UNSIGNED_INT, 0L);
}

model* load_model(char* model_file)
{
	const struct aiScene *sce = aiImportFile(model_file, 
			aiProcess_Triangulate | 
			aiProcess_JoinIdenticalVertices | 
			aiProcess_GenSmoothNormals |
			aiProcess_FixInfacingNormals);

	model *m = extract_model(*sce->mMeshes[0]);
	generate_vao(m);
	aiReleaseImport(sce);

	return m;

}
/*
int main()
{
	model *m = load_model("only_quad_sphere.obj");
	
	draw_model(m, 0, NULL, NULL, NULL, NULL);

	return 0;
}
*/
