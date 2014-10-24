#version 150

layout(triangles_adjacency) in;
layout(triangle_strip, max_vertices = 18) out;

in vec4 ex_colour[];
in vec3 ex_normal[];
in vec3 ex_position[];

out vec4 ex_g_colour;
out vec3 ex_g_normal;
out vec3 ex_g_position;

vec3 light = vec3(10, 20, 20);


void main()
{

vec4 tmp;

vec4 orig_v[3];
orig_v[0] = gl_in[0].gl_Position;
orig_v[1] = gl_in[2].gl_Position;
orig_v[2] = gl_in[4].gl_Position;

vec3 normals[3];
normals[0] = ex_normal[0];
normals[1] = ex_normal[2];
normals[2] = ex_normal[4];

vec3 d[3];
d[0] = light - orig_v[0].xyz;
d[1] = light - orig_v[1].xyz;
d[2] = light - orig_v[2].xyz;

bool faces_light = true;
if(!(dot(d[0], normals[0]) > 0 || 
		dot(d[1], normals[1]) > 0||
		dot(d[2], normals[2]) > 0)){
	faces_light = false;
	tmp = orig_v[1];
	orig_v[1] = orig_v[2];
	orig_v[2] = tmp;
}

/* Render near cap of volume */
for(int i = 0; i < 3; i++){
	gl_Position = orig_v[i];
	ex_g_position = vec3(orig_v[i]);
	ex_g_colour = ex_colour[2*i];
	ex_g_normal = ex_normal[2*i];
	EmitVertex();
}
EndPrimitive();

/* Far cap of volume */
for(int i = 0; i < 3; i++){
	gl_Position = vec4(orig_v[i].xyz - light, 0);
	ex_g_position = vec3(orig_v[i].xyz - light);
	ex_g_colour = ex_colour[2*i];
	ex_g_normal = ex_normal[2*i];
	EmitVertex();
}
EndPrimitive();

/* Extruding edges, sides of the volume */
for(int i = 0; i < 3; i++){
	normals[0] = ex_normal[2*i];
	normals[1] = ex_normal[2*i + 1];
	normals[2] = ex_normal[2*(i + 1) % 6];

	d[0] = light - gl_in[2*i].gl_Position.xyz;
	d[1] = light - gl_in[2*i + 1].gl_Position.xyz;
	d[2] = light - gl_in[2*(i+1) % 6].gl_Position.xyz;

	if(faces_light != (dot(normals[0], d[0]) > 0 ||
			dot(normals[1], d[1]) > 0 ||
			dot(normals[2], d[2]) > 0)){
		int i0 = faces_light ? 2*i : 2*(i+1) % 6;
		int i1 = faces_light ? 2*(i+1) % 6 : 2*i;

		gl_Position = gl_in[i0].gl_Position;
		EmitVertex();
		
		gl_Position = vec4(gl_in[i0].gl_Position.xyz - light, 1.0) ;
		EmitVertex();

		gl_Position = gl_in[i1].gl_Position;
		EmitVertex();

		gl_Position = vec4(gl_in[i1].gl_Position.xyz - light, 1.0);
		EmitVertex();
		EndPrimitive();
	}
}

}
