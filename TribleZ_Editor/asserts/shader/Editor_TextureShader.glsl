#type vertex_shader
#version 450 core
layout(location = 0) in vec3	position;
layout(location = 1) in	vec4	VerColor;
layout(location = 2) in vec2	texCoord;
layout(location = 3) in float	texIndex;
layout(location = 4) in float	TilingFector;

out vec4	v_Color;
out vec2	v_TexCoord;
out float	v_TexIndex;
out float	v_TilingFector;

uniform mat4 u_ViewProjection;


void main()
{
	v_Color		= VerColor;
	v_TexCoord	= texCoord;
	v_TexIndex	= texIndex;
	v_TilingFector = TilingFector;
	gl_Position = u_ViewProjection * vec4(position, 1.0);
}



#type fragment_shader
#version 450 core
//location为输出通道
layout(location = 0) out vec4 color_path1;
layout(location = 1) out int color_path2;

in vec4		v_Color;
in vec2		v_TexCoord;
in float	v_TexIndex;
in float	v_TilingFector;

uniform sampler2D	u_Texture[32];

void main()
{
															/*让纹理叠加颜色*/
	color_path1 = texture(u_Texture[int(v_TexIndex)], v_TexCoord * v_TilingFector) * v_Color;

	color_path2 = 50;
}