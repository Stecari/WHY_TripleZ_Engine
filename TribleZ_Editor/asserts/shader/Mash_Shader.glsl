#type vertex_shader
#version 330 core
layout(location = 0) in vec3 position;

out vec4 v_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model_trans;

void main()
{
	gl_Position = u_ViewProjection * u_Model_trans * vec4(position, 1.0);
}

#type fragment_shader
#version 330 core
out vec4 color;

uniform vec3 u_Color;

void main()
{
	color = vec4(0.2, 0.3, 0.8, 1.0);
	color = vec4(u_Color, 1.0);
}
