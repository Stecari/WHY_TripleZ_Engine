#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model_trans;

void main()
{
	v_TexCoord = texCoord;
	gl_Position = u_ViewProjection * u_Model_trans * vec4(position, 1.0);
}