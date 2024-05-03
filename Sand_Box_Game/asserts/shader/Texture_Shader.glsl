#type vertex_shader
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

#type fragment_shader
#version 330 core
out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture1;
uniform vec4 u_Color;
uniform vec4 u_Tex_Color;
uniform float u_TilingFector;


void main()
{														/*ÈÃÎÆÀíµþ¼ÓÑÕÉ«*/
	color = texture(u_Texture1, v_TexCoord );
	//color = vec4(u_Texture1, 1.0f, 1.0f, 1.0f);
}