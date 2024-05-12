#type vertex_shader
#version 450 core
layout(location = 0) in vec3	position;
layout(location = 1) in	vec4	VerColor;
layout(location = 2) in vec2	texCoord;
layout(location = 3) in float	texIndex;
layout(location = 4) in float	TilingFector;
layout(location = 5) in int		EntityID;

out vec4		v_Color;
out vec2		v_TexCoord;
out flat float	v_TexIndex;
out float		v_TilingFector;
out flat int	v_entityID;				//flat,��ʾ�޲�ֵ����Ӧsmooth����ʾ�⻬��ֵ

uniform mat4 u_ViewProjection;


void main()
{
	v_Color		= VerColor;
	v_TexCoord	= texCoord;
	v_TexIndex	= texIndex;
	v_TilingFector = TilingFector;
	v_entityID	= EntityID;
	gl_Position = u_ViewProjection * vec4(position, 1.0);
}



#type fragment_shader
#version 450 core
//locationΪ���ͨ��
layout(location = 0) out vec4 color_path1;
layout(location = 1) out int color_path2;

in vec4			v_Color;
in vec2			v_TexCoord;
in flat float	v_TexIndex;
in float		v_TilingFector;
in flat int		v_entityID;				//flat,��ʾ�޲�ֵ����Ӧsmooth����ʾ�⻬��ֵ

uniform sampler2D	u_Texture[32];

void main()
{
															/*�����������ɫ*/
	color_path1 = texture(u_Texture[int(v_TexIndex)], v_TexCoord * v_TilingFector) * v_Color;

	color_path2 = v_entityID;
}