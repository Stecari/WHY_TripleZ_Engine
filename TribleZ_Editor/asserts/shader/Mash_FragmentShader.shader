#version 330 core
out vec4 color;

uniform vec3 u_Color;

void main()
{
	color = vec4(0.2, 0.3, 0.8, 1.0);
	color = vec4(u_Color, 1.0);
}