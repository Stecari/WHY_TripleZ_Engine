//后期的时候为了适配Vulkon去除了uniform，但又改用了uniform Buffer
//flat,表示无插值，对应smooth，表示光滑插值
#type vertex_shader
#version 450 core

layout(location = 0) in vec3	position;
layout(location = 1) in	vec4	VerColor;
layout(location = 2) in vec2	texCoord;
layout(location = 3) in float	texIndex;
layout(location = 4) in float	TilingFector;
layout(location = 5) in int		EntityID;

//	 采用140标准， 绑定在0号通道
layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4	Color;
	vec2	TexCoord;
	float	TexIndex;
	float	TilingFector;
};


layout(location = 0) out VertexOutput v_Output;
layout(location = 4) out flat int	v_entityID;				


void main()
{
	v_Output.Color		= VerColor;
	v_Output.TexCoord	= texCoord;
	v_Output.TexIndex	= texIndex;
	v_Output.TilingFector = TilingFector;

	v_entityID	= EntityID;

	gl_Position = u_ViewProjection * vec4(position, 1.0);
}



#type fragment_shader
#version 450 core
layout(location = 0) out vec4 color_path1;
layout(location = 1) out int color_path2;

struct VertexOutput
{
	vec4	Color;
	vec2	TexCoord;
	float	TexIndex;
	float	TilingFector;
};

layout(location = 0) in VertexOutput v_Input;
layout(location = 4) in flat int		v_entityID;

layout(binding = 0)uniform sampler2D	u_Texture[32];

void main()
{
	vec4 texColor = v_Input.Color;
	switch(int(v_Input.TexIndex))
	{
		case  0 : texColor *= texture(u_Texture[ 0], v_Input.TexCoord * v_Input.TilingFector);		break;
		case  1 : texColor *= texture(u_Texture[ 1], v_Input.TexCoord * v_Input.TilingFector);		break;
		case  2 : texColor *= texture(u_Texture[ 2], v_Input.TexCoord * v_Input.TilingFector);		break;
		case  3 : texColor *= texture(u_Texture[ 3], v_Input.TexCoord * v_Input.TilingFector);		break;
		case  4 : texColor *= texture(u_Texture[ 4], v_Input.TexCoord * v_Input.TilingFector);		break;
		case  5 : texColor *= texture(u_Texture[ 5], v_Input.TexCoord * v_Input.TilingFector);		break;
		case  6 : texColor *= texture(u_Texture[ 6], v_Input.TexCoord * v_Input.TilingFector);		break;
		case  7 : texColor *= texture(u_Texture[ 7], v_Input.TexCoord * v_Input.TilingFector);		break;
		case  8 : texColor *= texture(u_Texture[ 8], v_Input.TexCoord * v_Input.TilingFector);		break;
		case  9 : texColor *= texture(u_Texture[ 9], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 10 : texColor *= texture(u_Texture[10], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 11 : texColor *= texture(u_Texture[11], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 12 : texColor *= texture(u_Texture[12], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 13 : texColor *= texture(u_Texture[13], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 14 : texColor *= texture(u_Texture[14], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 15 : texColor *= texture(u_Texture[15], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 16 : texColor *= texture(u_Texture[16], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 17 : texColor *= texture(u_Texture[17], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 18 : texColor *= texture(u_Texture[18], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 19 : texColor *= texture(u_Texture[19], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 20 : texColor *= texture(u_Texture[20], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 21 : texColor *= texture(u_Texture[21], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 22 : texColor *= texture(u_Texture[22], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 23 : texColor *= texture(u_Texture[23], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 24 : texColor *= texture(u_Texture[24], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 25 : texColor *= texture(u_Texture[25], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 26 : texColor *= texture(u_Texture[26], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 27 : texColor *= texture(u_Texture[27], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 28 : texColor *= texture(u_Texture[28], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 29 : texColor *= texture(u_Texture[29], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 30 : texColor *= texture(u_Texture[30], v_Input.TexCoord * v_Input.TilingFector);		break;
		case 31 : texColor *= texture(u_Texture[31], v_Input.TexCoord * v_Input.TilingFector);		break;
	}
	color_path1 = texColor;
	color_path2 = v_entityID;
}