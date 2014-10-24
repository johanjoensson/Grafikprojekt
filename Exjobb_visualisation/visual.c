#include "object.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <assert.h>

#include "SDL_util.h"
#include "GL_tools.h"
#include "object.h"
#include "vector_lib.h"

char sigma_bool = 0, omega_bool = 0, rotation_bool = 0; 

void set_sigma()
{
	sigma_bool = !sigma_bool;
}

void set_omega()
{
	omega_bool = !omega_bool;
}

void set_rot()
{	
	rotation_bool = !rotation_bool;
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
				case SDLK_s:
					set_sigma();
					break;
				case SDLK_o:
					set_omega();
					break;
				case SDLK_r:
					set_rot();
					break;
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

mat_4D proj_matrix;
mat_4D cam_matrix;

void display(model** m, GLuint program)
{
	glUseProgram(program);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	

	mat_4D theta, sigma, omega, rot_mat;
	
	theta = scale(1); 
	
	if(sigma_bool){
		sigma = shear(1.0/3, -2.0/3, 1.0/3);
	}else{
		sigma = zero_matrix();
	}

	if(!rotation_bool){
		rot_mat = rot_x(45);
		rot_mat = mult(rot_mat, rot_y(45));
		rot_mat = mult(rot_mat, rot_z(45));
	}else{
		rot_mat = identity_matrix();
	}
	
	if(omega_bool){
		omega = rot(2.0/3, -1.0/3, 1.0/3);
	}else{
		omega = zero_matrix();
	}

	mat_4D mat;

	
	mat = matrix_add(matrix_add(theta, mult(sigma, rot_mat)),omega);
	
	mat = mult(mult(rot_x(45),rot_y(-45)), mat);

	glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_TRUE, proj_matrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_TRUE, cam_matrix.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1,GL_TRUE, mat.m);

	draw_model(m[0], program, "in_position", "in_normal", NULL, NULL);

	SDL_GL_SwapBuffers();
}

void init()
{
	glClearColor(1.0, 0.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	proj_matrix = perspective(90.0, 1.0, 0.1, 5000.0);

	vec_3D cam, point, up;
	cam.x = 0.0;
	cam.y = 0.0;
	cam.z = 2.0;

	point.x = 0.0;
	point.y = 0.0;
	point.z = 0.0;

	up.x = 0.0;
	up.y = 1.0;
	up.z = 0.0;
	
	cam_matrix = look_at(cam, point, up);
}
int main()
{

	init_SDL();
	init();

	printf("Init done\n");
	GLuint program = init_shaders("simple.vert", "simple.geom", "simple.frag");
	glUseProgram(program);

	printf("shaders loaded and ready to go\n");
	model **m = load_model("cubeplus.obj");

	SDL_Event event;

	while(1){
		while(SDL_PollEvent(&event)){
			event_handler(event);
			display(m, program);
		}
	}

	return 0;
}
