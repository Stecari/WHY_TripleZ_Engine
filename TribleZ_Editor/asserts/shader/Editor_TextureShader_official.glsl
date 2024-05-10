//这个是cherno写的样子，用的Switch

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
layout(location = 0) out vec4 color_path1;
layout(location = 1) out int color_path2;

in vec4		v_Color;
in vec2		v_TexCoord;
in float	v_TexIndex;
in float	v_TilingFector;

uniform sampler2D	u_Texture[32];

void main()
{
	vec4 texColor = v_Color;
	switch(int(v_TexIndex))
	{
		case 0 : texColor *= texture(u_Texture[0], v_TexCoord * v_TilingFector);		break;
		case 1 : texColor *= texture(u_Texture[1], v_TexCoord * v_TilingFector);		break;
		case 2 : texColor *= texture(u_Texture[2], v_TexCoord * v_TilingFector);		break;
		case 3 : texColor *= texture(u_Texture[3], v_TexCoord * v_TilingFector);		break;
		case 4 : texColor *= texture(u_Texture[4], v_TexCoord * v_TilingFector);		break;
		case 5 : texColor *= texture(u_Texture[5], v_TexCoord * v_TilingFector);		break;
		case 6 : texColor *= texture(u_Texture[6], v_TexCoord * v_TilingFector);		break;
		case 7 : texColor *= texture(u_Texture[7], v_TexCoord * v_TilingFector);		break;
		case 8 : texColor *= texture(u_Texture[8], v_TexCoord * v_TilingFector);		break;
		case 9 : texColor *= texture(u_Texture[9], v_TexCoord * v_TilingFector);		break;
		case 10 : texColor *= texture(u_Texture[10], v_TexCoord * v_TilingFector);		break;
		case 11 : texColor *= texture(u_Texture[11], v_TexCoord * v_TilingFector);		break;
		case 12 : texColor *= texture(u_Texture[12], v_TexCoord * v_TilingFector);		break;
		case 13 : texColor *= texture(u_Texture[13], v_TexCoord * v_TilingFector);		break;
		case 14 : texColor *= texture(u_Texture[14], v_TexCoord * v_TilingFector);		break;
		case 15 : texColor *= texture(u_Texture[15], v_TexCoord * v_TilingFector);		break;
		case 16 : texColor *= texture(u_Texture[16], v_TexCoord * v_TilingFector);		break;
		case 17 : texColor *= texture(u_Texture[17], v_TexCoord * v_TilingFector);		break;
		case 18 : texColor *= texture(u_Texture[18], v_TexCoord * v_TilingFector);		break;
		case 19 : texColor *= texture(u_Texture[19], v_TexCoord * v_TilingFector);		break;
		case 20 : texColor *= texture(u_Texture[20], v_TexCoord * v_TilingFector);		break;
		case 21 : texColor *= texture(u_Texture[21], v_TexCoord * v_TilingFector);		break;
		case 22 : texColor *= texture(u_Texture[22], v_TexCoord * v_TilingFector);		break;
		case 23 : texColor *= texture(u_Texture[23], v_TexCoord * v_TilingFector);		break;
		case 24 : texColor *= texture(u_Texture[24], v_TexCoord * v_TilingFector);		break;
		case 25 : texColor *= texture(u_Texture[25], v_TexCoord * v_TilingFector);		break;
		case 26 : texColor *= texture(u_Texture[26], v_TexCoord * v_TilingFector);		break;
		case 27 : texColor *= texture(u_Texture[27], v_TexCoord * v_TilingFector);		break;
		case 28 : texColor *= texture(u_Texture[28], v_TexCoord * v_TilingFector);		break;
		case 29 : texColor *= texture(u_Texture[29], v_TexCoord * v_TilingFector);		break;
		case 30 : texColor *= texture(u_Texture[30], v_TexCoord * v_TilingFector);		break;
		case 31 : texColor *= texture(u_Texture[31], v_TexCoord * v_TilingFector);		break;
	}
	color_path1 = texColor;
	color_path2 = 50;
}