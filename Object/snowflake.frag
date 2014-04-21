#version 150

in vec4 ex_g_colour;
in vec3 ex_g_normal;
in vec3 ex_g_position;

out vec4 frag_Color;

vec3 light = vec3(10, 0, 0);

void main()
{
	vec4 colour = ex_g_colour;
	colour.r = abs(colour.r);
	colour.g = abs(colour.g);
	colour.b = abs(colour.b);
	colour.a = abs(colour.a);
	float shade = dot(normalize(light - ex_g_position), normalize(ex_g_normal));
	shade = clamp(shade, 0, 1);

//	frag_Color = vec4(ex_normal, 1.0);
	frag_Color = shade*colour;
}
