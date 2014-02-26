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
#include <math.h>

#include "SDL_util.h"
#include "GL_tools.h"
#include "object.h"


float vertices[9] = {	-0.5, -0.5, 0.0,
			0.0, 0.366, 0.0,
			0.5, -0.5, 0.0	};

float colours[9] = {	0.5, 0.0, 0.5, 
			0.6, 0.4, 0.0,
			0.0, 0.7, 0.3	};

/* Make sure that the winding of the triangle is correct, otherwise it will get
 * culled, should wind counterclockwise */
GLuint indices[] = {	0,1,2	};

GLuint vertex_array_object_id;
GLuint vertex_buffer_object_id;
GLuint colour_buffer_object_id;
GLuint index_buffer_object_id;

GLuint buffer_object_id[];

void display(GLuint program)
{
	glUseProgram(program);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);

	SDL_GL_SwapBuffers();
}

void init(GLuint program)
{
	glClearColor(0.6, 0.6, 0.4, 0.0);

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glUseProgram(program);

	glGenVertexArrays(1, &vertex_array_object_id);
	glBindVertexArray(vertex_array_object_id);

	glGenBuffers(3, buffer_object_id);

	glBindBuffer(GL_ARRAY_BUFFER, buffer_object_id[0]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_position"));

	glBindBuffer(GL_ARRAY_BUFFER, buffer_object_id[1]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), colours, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_colour"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_colour"));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_object_id[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLuint), indices, GL_STATIC_DRAW);

}

void event_handler(SDL_Event e)
{
	switch(e.type){
		case SDL_QUIT:
			exit_prog(0);
		case SDL_KEYDOWN:
			switch(e.key.keysym.sym){
				case SDLK_q:
				case SDLK_ESCAPE:
					exit_prog(0);
				default: 
					break;
			}
			break;
		case SDL_VIDEORESIZE:
			resize_window(e);
			break;
		default:
			break;
	}
}

int main(int argc, char** argv)
{
	init_SDL();
	GLuint program = init_shaders("snowflake.vert", "snowflake.geom", "snowflake.frag");
	init(program);
	SDL_Event event;
	while(1){
		while(SDL_PollEvent(&event)){
			event_handler(event);
			display(program);
		}
	}

	return program;
}
