#version 150

in vec4 in_position

out vec4 out_position

void main()
{
	gl_Position = in_position;
}
