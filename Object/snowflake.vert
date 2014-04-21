#version 150

in vec3 in_position;
in vec3 in_normal;
in vec4 in_colour;

out vec4 ex_colour;
out vec3 ex_normal;
out vec3 ex_position;

uniform mat4 proj_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

void main()
{
	ex_colour = vec4(in_normal,1.0);
	mat3 norm_matrix = mat3(view_matrix*model_matrix);
	ex_normal = norm_matrix*in_normal;
	gl_Position = proj_matrix*view_matrix*model_matrix*vec4(in_position,1.0);
	ex_position = vec3(proj_matrix*view_matrix*model_matrix*vec4(in_position,1.0));
}
