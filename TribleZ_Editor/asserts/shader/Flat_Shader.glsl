#type vertex_shader
#version 330 core
layout(location = 0) in vec3 position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model_trans;

void main()
{
	gl_Position = u_ViewProjection * u_Model_trans * vec4(position, 1.0);
}


#type fragment_shader
#version 330 core
out vec4 color;

uniform vec4 u_FlatColor;

void main()
{
	color = u_FlatColor;
}