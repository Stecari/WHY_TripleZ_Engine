#version 330 core
out vec4 color;

in vec4 v_Color;
in vec3 v_Position;

void main()
{
	color = vec4(0.1, 0.5, 0.7, 1.0);
	color = vec4(v_Position + 0.2, 1.0);
	color = v_Color;
}