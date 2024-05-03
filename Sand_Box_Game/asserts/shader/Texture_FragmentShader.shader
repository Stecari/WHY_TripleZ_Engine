#version 330 core
out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture1;

void main()
{
	color = texture(u_Texture1, v_TexCoord);
}