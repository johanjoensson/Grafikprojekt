#version 150

in vec3 ex_g_colour;

out vec4 frag_Color;

void main()
{
	frag_Color = vec4(ex_g_colour, 1.0);
}
