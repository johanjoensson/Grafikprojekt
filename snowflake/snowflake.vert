#version 150

in vec3 in_position;
in vec3 in_colour;

out vec3 ex_colour;

void main()
{
	gl_Position = vec4(in_position,1.0);
	ex_colour = in_colour;
}
