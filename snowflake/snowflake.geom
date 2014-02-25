#version 150

layout(triangles) in;
layout(line_strip, max_vertices = 13) out;
//layout(triangle_strip, max_vertices = 3) out;

in vec3 ex_colour[3];

out vec3 ex_g_colour;

void koch_generator(vec4 s, vec4 p, vec4 m)
{
	vec4 midpoint = s+p;
	vec4 first, second;
	first = 2*s + p;
	second = s + 2*p;

	vec3 dir = vec3(p-s);
	vec3 tmp = vec3(m-s);
	vec3 up = cross(tmp,dir);
	vec3 outwards = cross(up,dir);
	outwards = normalize(outwards);
	
	vec4 lift = midpoint/midpoint.w + vec4(length(dir)/6*sqrt(3)*outwards, 1.0);
	lift.w = 1.0;

	gl_Position = first;
	ex_g_colour = ex_colour[2];
	EmitVertex();
	
	gl_Position = lift;
	ex_g_colour = ex_colour[1];
	EmitVertex();

	gl_Position = second;
	ex_g_colour = ex_colour[0];
	EmitVertex();
}

void main()
{
	for(int i = 0; i < gl_in.length(); i++){
		gl_Position = gl_in[i].gl_Position;
		ex_g_colour = ex_colour[i];
		EmitVertex();
		koch_generator(gl_in[i].gl_Position, gl_in[(i+1)%gl_in.length].gl_Position, gl_in[(i+2)%gl_in.length].gl_Position);
	}

	gl_Position = gl_in[0].gl_Position;
	ex_g_colour = ex_colour[0];
	EmitVertex();

	EndPrimitive();
}
