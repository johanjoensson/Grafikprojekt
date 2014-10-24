#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec4 ex_colour[];
in vec3 ex_normal[];
in vec3 ex_position[];

out vec4 ex_g_colour;
out vec3 ex_g_normal;
out vec3 ex_g_position;

void main()
{
	for(int i = 0; i < gl_in.length(); i++){
		gl_Position = gl_in[i].gl_Position;
		ex_g_colour = ex_colour[i];
		ex_g_normal = ex_normal[i];
		EmitVertex();
	}

	EndPrimitive();
}
