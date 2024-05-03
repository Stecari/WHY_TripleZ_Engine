#version 330 core
layout(location = 0) in vec3 position;

out vec4 v_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model_trans;

void main()
{
	gl_Position = u_ViewProjection * u_Model_trans * vec4(position, 1.0);
}