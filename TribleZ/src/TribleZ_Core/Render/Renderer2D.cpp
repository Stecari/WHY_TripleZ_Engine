#include "tzpch.h"
#include "Renderer2D.h"
#include "TribleZ_Core/Render/Shader.h"
#include "TribleZ_Core/Render/UniformBuffer.h"
#include "TribleZ_Core/Render/VertexArray.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "TribleZ_Core/Render/RendererCommand.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include <glad/glad.h>

struct Vertex
{
	glm::vec3	Position;
	glm::vec4	Color;
	glm::vec2	TexCoord;
	float		TexIndex;
	float		TilingFector;

	//仅仅为了Editor
	int		EntityID;
};


namespace TribleZ
{
	/*我觉得这一段放到2D渲染器class中好一点*/  /*后继，这里是Cpp源文件，没有关系*/
	struct Renderer2D_Data
	{
		static const uint32_t MaxQuads		 = 256;			//单个顶点缓冲区中存储的最大的四边形的数量
		static const uint32_t MaxVertex		 = MaxQuads * 4;	//单个顶点缓冲区中存储的最大的顶点数
		static const uint32_t MaxIndex		 = MaxQuads * 6;	//单个索引缓冲区中存储的最大的索引数
		static const uint32_t MaxTextureSlot = 32;				//硬件最大支持的同时绑定的纹理数

		Ref<Shader>				Texture_Shader;
		Ref<VertexArray>		Quad_VertexArray;
		Ref<VertexBuffer>		Quad_VertexBuffer;
		Ref<Texture2D>			WhiteTexture;

		uint32_t Quad_IndexCount	= 0;

		Vertex* VertexBufferDataBase	= nullptr;		//存储顶点缓存区的数据，同时也是顶点缓冲区数据的首地址
		Vertex* VertexBufferDataPtr		= nullptr;		//指向顶点缓存区数据的末地址，也即是当前指针

		std::array<Ref<Texture>, MaxTextureSlot> TextureSlotBase;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 VertexOriPosition[4];

		Renderer2D::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;					
		Ref<UniformBuffer> CameraUniformBuffer; //相机缓存区
	};
	static Renderer2D_Data m_DataBase;


	void Renderer2D::Init()
	{
		TZ_PROFILE_FUNCTION_SIG();

		/*---------创建顶点数组---------------------------------------------------------*/
		m_DataBase.Quad_VertexArray = VertexArray::Create();
		/*---------创建顶点缓冲---------*/

		m_DataBase.Quad_VertexBuffer = VertexBuffer::Create(m_DataBase.MaxVertex * sizeof(Vertex));												//shared_ptr.reset(ptr) 重定向，让这个共享指针指向ptr的内容
		//m_VertexBuffer = VertexBuffer::Create(sizeof(Flat_VertexData), Flat_VertexData);												//shared_ptr.reset(ptr) 重定向，让这个共享指针指向ptr的内容
		//m_VertexBuffer->Bind();
		/*----顶点数组使能并设置布局----*/
		BufferLayout layout({
			{ "a_Position",		ShaderDataType::Float3	},
			{ "a_Color",		ShaderDataType::Float4	},
			{ "a_TexCoord",		ShaderDataType::Float2	},
			{ "a_TexIndex",		ShaderDataType::Float1	},
			{ "a_Tiling",		ShaderDataType::Float1	},
			{ "a_EntityID",		ShaderDataType::Int1	}
			});
		m_DataBase.Quad_VertexBuffer->SetLayout(layout); //绑定缓冲区前先设置布局，不然里面没东西
		m_DataBase.Quad_VertexArray->AddVertexBuffer(m_DataBase.Quad_VertexBuffer);
		
		m_DataBase.VertexBufferDataBase = new Vertex[m_DataBase.MaxVertex];		//开辟足够多的空间
		/*---------创建索引缓冲---------*/
		uint32_t* QuadIndexTable = new uint32_t[m_DataBase.MaxIndex];


		uint32_t offset = 0;
		for (uint32_t i = 0; i < m_DataBase.MaxIndex; i += 6)
		{
			QuadIndexTable[i + 0] = offset + 0;
 			QuadIndexTable[i + 1] = offset + 1;
			QuadIndexTable[i + 2] = offset + 2;

			QuadIndexTable[i + 3] = offset + 2;
			QuadIndexTable[i + 4] = offset + 3;
			QuadIndexTable[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> QuadIndex = IndexBuffer::Create(m_DataBase.MaxIndex, QuadIndexTable);

		QuadIndex->Bind();
		m_DataBase.Quad_VertexArray->SetIndexBuffer(QuadIndex);
		delete[] QuadIndexTable;
		//Ref<IndexBuffer> m_IndexBuffer;
		//m_IndexBuffer = IndexBuffer::Create(sizeof(Flat_Index) / sizeof(unsigned int), Flat_Index);
		//m_IndexBuffer->Bind();
		//m_DataBase.Quad_VertexArray->SetIndexBuffer(m_IndexBuffer);
		/*------------------------------------------------------------------------------*/


		/*-----------------------设置参考------------------------------------------------------------*/
		/*--------------------纹理白纹理-------------------------------------------------------------*/
		m_DataBase.WhiteTexture = Texture2D::Create(1, 1);		 /*   R  G  B  A  */
		unsigned int WhiteTextureData = 0xffffffff;				/* 0xff ff ff ff */
		m_DataBase.WhiteTexture->SetData(&WhiteTextureData, sizeof(uint32_t));
		m_DataBase.TextureSlotBase[0] = m_DataBase.WhiteTexture;	//0号插槽是白纹理
		/*--------------------纹理采样槽-------------------------------------------------------------*/
		int texture[32];
		for (int i = 0; i < m_DataBase.MaxTextureSlot; i++){
			texture[i] = i;
		}
		/*--------------------绘制基准点-------------------------------------------------------------*/
		m_DataBase.VertexOriPosition[0] = {-0.5f, -0.5f, 0.0f, 1.0f};		//左下
		m_DataBase.VertexOriPosition[1] = { 0.5f, -0.5f, 0.0f, 1.0f};		//右下
		m_DataBase.VertexOriPosition[2] = { 0.5f,  0.5f, 0.0f, 1.0f};		//右上
		m_DataBase.VertexOriPosition[3] = {-0.5f,  0.5f, 0.0f, 1.0f};		//左上
		/*------------------创建相机缓冲区-----------------------------------------------------------*/
		m_DataBase.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2D_Data::CameraData), 0);

		/*-------------------读取着色器源码-----------------------------------------------------------*/
		//m_DataBase.Texture_Shader = Shader::Create("asserts/shader/Editor_TextureShader.glsl");
		//m_DataBase.Texture_Shader = Shader::Create("asserts/shader/Editor_TextureShader_official.glsl");
		m_DataBase.Texture_Shader = Shader::Create("asserts/shader/cherno.glsl");
		/*-------------------读取着色器源码-----------------------------------------------------------*/
		//m_DataBase.Texture_Shader->Bind();
		//m_DataBase.Texture_Shader->SetIntArray("u_Texture", texture, 32);//将texture[32]数组传给uniform u_Texture
	}

	void Renderer2D::ShutDown()
	{
		TZ_PROFILE_FUNCTION_SIG()
	}


	void Renderer2D::SceneBegin(const BaseCamera& camera, const glm::mat4& transform)	//初始化渲染器的数据库m_DataBase
	{
		TZ_PROFILE_FUNCTION_SIG();
		StartBatch();

		glm::mat4 ViewProjection = camera.GetProjection() * glm::inverse(transform);	//由于是相机的变换所以要取反

		m_DataBase.CameraBuffer.ViewProjection = ViewProjection;
		m_DataBase.CameraUniformBuffer->SetData(&m_DataBase.CameraBuffer, sizeof(Renderer2D_Data::CameraData), 0);
		//老API
		//m_DataBase.Texture_Shader->Bind();
		//m_DataBase.Texture_Shader->SetMat4("u_ViewProjection", ViewProjection);

	}
	void Renderer2D::SceneBegin(const OrthoGraphicCamera& camera)	
	{
		TZ_PROFILE_FUNCTION_SIG();
		StartBatch();
		//不知道为什么这个不用改，可能是用不上吧
		//m_DataBase.CameraBuffer.ViewProjection = camera.GetViewProjectMat();
		//m_DataBase.CameraUniformBuffer->SetData(&m_DataBase.CameraBuffer, sizeof(Renderer2D_Data::CameraData), 0)
		m_DataBase.Texture_Shader->Bind();
		m_DataBase.Texture_Shader->SetMat4("u_ViewProjection", camera.GetViewProjectMat());
	}
	void Renderer2D::SceneBegin(const Editor_Camera& camera)
	{
		TZ_PROFILE_FUNCTION_SIG();
		StartBatch();

		m_DataBase.CameraBuffer.ViewProjection = camera.GetViewProjection();
		m_DataBase.CameraUniformBuffer->SetData(&m_DataBase.CameraBuffer, sizeof(Renderer2D_Data::CameraData), 0);

		//glm::mat4 ViewProjection = camera.GetViewProjection();	//由于是相机的变换所以要取反
		//m_DataBase.Texture_Shader->Bind();
		//m_DataBase.Texture_Shader->SetMat4("u_ViewProjection", ViewProjection);
	}

	void Renderer2D::StartBatch()
	{
		m_DataBase.Quad_IndexCount = 0;	//索引重新开始计数
		m_DataBase.TextureSlotIndex = 1;//纹理插槽重新开始计数(0号是白纹理)
		m_DataBase.VertexBufferDataPtr = m_DataBase.VertexBufferDataBase;				//运行指针赋值
	}

	void Renderer2D::SceneEnd()
	{
		TZ_PROFILE_FUNCTION_SIG();

		uint32_t data_size = (uint8_t*)m_DataBase.VertexBufferDataPtr - (uint8_t*)m_DataBase.VertexBufferDataBase;//计算字节数，直接减会求出数据个数
		m_DataBase.Quad_VertexBuffer->SetData(m_DataBase.VertexBufferDataBase, data_size);

		Flush();
	}

	void Renderer2D::Flush()
	{
		//if (m_DataBase.Quad_IndexCount == 0)
			//return; // Nothing to draw

		for (int i = 0; i < m_DataBase.TextureSlotIndex; i++) {		//绑定纹理									
			m_DataBase.TextureSlotBase[i]->Bind(i);	//绑定i号插槽   
		}
		/*-----???----------------------*/		//有问题
		m_DataBase.Texture_Shader->Bind();		//不知道为什么有
		m_DataBase.Quad_VertexArray->Bind();	//不知道为什么没有		//绑定顶点数组
		/*-----???----------------------*/
		RendererCommand::DrawIndex(m_DataBase.Quad_VertexArray, m_DataBase.Quad_IndexCount);
		m_DataBase.Stats.DrawCallCount++;
	}

	void Renderer2D::ResetAndFlush()
	{
		SceneEnd();

		m_DataBase.Quad_IndexCount = 0;
		m_DataBase.TextureSlotIndex = 1;
		m_DataBase.VertexBufferDataPtr = m_DataBase.VertexBufferDataBase;
	}
	/*-------------------------------------------------------渲染图元基础模式------------------------------------------------------------------------------------*/
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		const float tex_index = 0.0f;
		constexpr float tex_coord[8] = {	//用来绑定纹理的四个点
			0.0f, 0.0f,		//左下
			1.0f, 0.0f,		//右下
			1.0f, 1.0f,		//右上
			0.0f, 1.0f		//左上
		};
		if (m_DataBase.Quad_IndexCount >= Renderer2D_Data::MaxIndex){
			ResetAndFlush();
		}
		for (size_t i = 0; i < 4; i++)	//四个点挨个放到顶点缓冲区数据中
		{
			m_DataBase.VertexBufferDataPtr->Position = transform * m_DataBase.VertexOriPosition[i];
			m_DataBase.VertexBufferDataPtr->Color = color;
			m_DataBase.VertexBufferDataPtr->TexCoord = { tex_coord[i * 2], tex_coord[i * 2 + 1] };
			m_DataBase.VertexBufferDataPtr->TexIndex = tex_index;
			m_DataBase.VertexBufferDataPtr->TilingFector = 1.0f;
			m_DataBase.VertexBufferDataPtr->EntityID = entityID;
			m_DataBase.VertexBufferDataPtr++;
		}
		m_DataBase.Quad_IndexCount += 6;
		m_DataBase.Stats.QuadCount++;
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingfactor, const glm::vec4& tintcolor, int entityID)
	{
		float tex_index = 0;			//我们要画的纹理的索引
		constexpr float tex_coord[8] = {	//用来绑定纹理的四个点
			0.0f, 0.0f,		//左下
			1.0f, 0.0f,		//右下
			1.0f, 1.0f,		//右上
			0.0f, 1.0f		//左上
		};
		if (m_DataBase.Quad_IndexCount >= Renderer2D_Data::MaxIndex){
			ResetAndFlush();
		}
		/*----------------------------绑定纹理-----------------------------------------*/
		/*0号位为白纹理*/
		for (int i = 1; i < m_DataBase.TextureSlotIndex; i++)
		{
			/*先找我们已经存了的纹理中有没有我们要画的那个*/
			if (*texture.get() == *m_DataBase.TextureSlotBase[i].get())
			{
				tex_index = (float)i;	/*有的话就设为那个值*/
				break;
			}
		}
		if (tex_index == 0)	/*假如循环过后依旧为0说明没找到*/
		{
			tex_index = (float)m_DataBase.TextureSlotIndex;

			m_DataBase.TextureSlotBase[m_DataBase.TextureSlotIndex] = texture;
			m_DataBase.TextureSlotIndex++;
		}
		/*----------------------------绑定纹理-----------------------------------------*/
		/*-----------------------------------------------------------------------------*/
		for (size_t i = 0; i < 4; i++)	//四个点挨个放到顶点缓冲区数据中
		{
			m_DataBase.VertexBufferDataPtr->Position = transform * m_DataBase.VertexOriPosition[i];
			m_DataBase.VertexBufferDataPtr->Color = tintcolor;
			m_DataBase.VertexBufferDataPtr->TexCoord = { tex_coord[i * 2], tex_coord[i * 2 + 1] };
			m_DataBase.VertexBufferDataPtr->TexIndex = tex_index;
			m_DataBase.VertexBufferDataPtr->TilingFector = tilingfactor;
			m_DataBase.VertexBufferDataPtr++;
		}
		m_DataBase.Quad_IndexCount += 6;
		m_DataBase.Stats.QuadCount++;
	}
	/*-----------------------------------------------------------纯色方块----------------------------------------------------------------------------------------*/
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		TZ_PROFILE_FUNCTION_SIG();
		/*----------------------------变换矩阵-----------------------------------------*/
		glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
			* glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
		/*----------------------------变换矩阵-----------------------------------------*/

		DrawQuad(transform, color);
	}
	/*-----------------------------------------------------------纹理方块----------------------------------------------------------------------------------------*/
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingfactor, const glm::vec4& tintcolor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingfactor, tintcolor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingfactor, const glm::vec4& tintcolor)
	{
		TZ_PROFILE_FUNCTION_SIG();
		/*----------------------------变换矩阵-----------------------------------------*/
		glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
			* glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
		/*----------------------------变换矩阵-----------------------------------------*/

		DrawQuad(transform, texture, tilingfactor, tintcolor);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingfactor, const glm::vec4& tintcolor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, tilingfactor, tintcolor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tilingfactor, const glm::vec4& tintcolor)
	{
		TZ_PROFILE_FUNCTION_SIG();
		const Ref<Texture2D>& texture = subtexture->GetTextureSheet();
		const glm::vec2* tex_coord = subtexture->GetTextureCoords();
		if (m_DataBase.Quad_IndexCount >= Renderer2D_Data::MaxIndex)
		{
			ResetAndFlush();
		}
		/*----------------------------绑定纹理-----------------------------------------*/
		float tex_index = 0;		//我们要画的纹理的索引
		/*0号位为白纹理*/
		for (int i = 1; i < m_DataBase.TextureSlotIndex; i++)
		{
			/*先找我们已经存了的纹理中有没有我们要画的那个*/
			if (*texture.get() == *m_DataBase.TextureSlotBase[i].get())
			{
				tex_index = (float)i;	/*有的话就设为那个值*/
				break;
			}
		}
		if (tex_index == 0)	/*假如循环过后依旧为0说明没找到*/
		{
			tex_index = (float)m_DataBase.TextureSlotIndex;

			m_DataBase.TextureSlotBase[m_DataBase.TextureSlotIndex] = texture;
			m_DataBase.TextureSlotIndex++;
		}
		/*----------------------------绑定纹理-----------------------------------------*/
		/*-----------------------------------------------------------------------------*/
		/*----------------------------变换矩阵-----------------------------------------*/
		glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
			* glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
		/*----------------------------变换矩阵-----------------------------------------*/
		/*-----------------------------------------------------------------------------*/
		for (size_t i = 0; i < 4; i++)	//四个点挨个放到顶点缓冲区数据中
		{
			m_DataBase.VertexBufferDataPtr->Position = transform * m_DataBase.VertexOriPosition[i];
			m_DataBase.VertexBufferDataPtr->Color = tintcolor;
			m_DataBase.VertexBufferDataPtr->TexCoord = tex_coord[i];
			m_DataBase.VertexBufferDataPtr->TexIndex = tex_index;
			m_DataBase.VertexBufferDataPtr->TilingFector = tilingfactor;
			m_DataBase.VertexBufferDataPtr++;
		}
		m_DataBase.Quad_IndexCount += 6;
		m_DataBase.Stats.QuadCount++;
	}
	/*-----------------------------------------------------------纹理方块----------------------------------------------------------------------------------------*/


	/*-----------------------------------------------------------旋转版本----------------------------------------------------------------------------------------*/
	void Renderer2D::DrawRotationQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawRotationQuad({ position.x, position.y, 0.0f }, rotation, size, color);
	}
	void Renderer2D::DrawRotationQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		TZ_PROFILE_FUNCTION_SIG();
		const float		 tex_index    = 0.0f;
		const float		 tilingfactor = 1.0f;
		const glm::vec4& tintcolor	  = glm::vec4(1.0f);

		if (m_DataBase.Quad_IndexCount >= Renderer2D_Data::MaxIndex)
		{
			ResetAndFlush();
		}

		/*----------------------------变换矩阵-----------------------------------------*/
		glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
			* glm::rotate(glm::mat4(1.0), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
		/*----------------------------变换矩阵-----------------------------------------*/


		float tex_coord[8] = {	//用来绑定纹理的四个点
			0.0f, 0.0f,		//左下
			1.0f, 0.0f,		//右下
			1.0f, 1.0f,		//右上
			0.0f, 1.0f		//左上
		};
		for (size_t i = 0; i < 4; i++)	//四个点挨个放到顶点缓冲区数据中
		{
			m_DataBase.VertexBufferDataPtr->Position = transform * m_DataBase.VertexOriPosition[i];
			m_DataBase.VertexBufferDataPtr->Color = color;
			m_DataBase.VertexBufferDataPtr->TexCoord = { tex_coord[i * 2], tex_coord[i * 2 + 1] };
			m_DataBase.VertexBufferDataPtr->TexIndex = tex_index;
			m_DataBase.VertexBufferDataPtr->TilingFector = tilingfactor;
			m_DataBase.VertexBufferDataPtr++;
		}
		m_DataBase.Quad_IndexCount += 6;

		m_DataBase.Stats.QuadCount++;
	}
	void Renderer2D::DrawRotationQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const float tilingfactor, const glm::vec4& tintcolor)
	{
		DrawRotationQuad({ position.x, position.y, 0.0f }, rotation, size, texture, tilingfactor, tintcolor);
	}
	void Renderer2D::DrawRotationQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const float tilingfactor, const glm::vec4& tintcolor)
	{
		TZ_PROFILE_FUNCTION_SIG();
		
		/*-----------------------------------------------------------------------------*/
		/*-----------------------检测批渲染最大方块数----------------------------------*/
		if (m_DataBase.Quad_IndexCount >= Renderer2D_Data::MaxIndex)
		{
			ResetAndFlush();
		}
		/*-----------------------检测批渲染最大方块数----------------------------------*/
		/*-----------------------------------------------------------------------------*/
		/*----------------------------绑定纹理-----------------------------------------*/
		float tex_index = 0;		//我们要画的纹理的索引
		/*0号位为白纹理*/
		for (int i = 1; i < m_DataBase.TextureSlotIndex; i++)
		{
			/*先找我们已经存了的纹理中有没有我们要画的那个*/
			if (*texture.get() == *m_DataBase.TextureSlotBase[i].get())
			{
				tex_index = (float)i;	/*有的话就设为那个值*/
				break;
			}
		}
		if (tex_index == 0)	/*假如循环过后依旧为0说明没找到*/
		{
			tex_index = (float)m_DataBase.TextureSlotIndex;

			m_DataBase.TextureSlotBase[m_DataBase.TextureSlotIndex] = texture;
			m_DataBase.TextureSlotIndex++;
		}
		/*----------------------------绑定纹理-----------------------------------------*/
		/*-----------------------------------------------------------------------------*/
		/*----------------------------变换矩阵-----------------------------------------*/
		glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
							* glm::rotate(glm::mat4(1.0), rotation, { 0.0f, 0.0f, 1.0f })
							* glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
		/*----------------------------变换矩阵-----------------------------------------*/
		/*-----------------------------------------------------------------------------*/
		float tex_coord[8] = {	//用来绑定纹理的四个点
			0.0f, 0.0f,		//左下
			1.0f, 0.0f,		//右下
			1.0f, 1.0f,		//右上
			0.0f, 1.0f		//左上
		};
		for (size_t i = 0; i < 4; i++)	//四个点挨个放到顶点缓冲区数据中
		{
			m_DataBase.VertexBufferDataPtr->Position = transform * m_DataBase.VertexOriPosition[i];
			m_DataBase.VertexBufferDataPtr->Color = tintcolor;
			m_DataBase.VertexBufferDataPtr->TexCoord = { tex_coord[i * 2], tex_coord[i * 2 + 1]};
			m_DataBase.VertexBufferDataPtr->TexIndex = tex_index;
			m_DataBase.VertexBufferDataPtr->TilingFector = tilingfactor;
			m_DataBase.VertexBufferDataPtr++;
		}
		m_DataBase.Quad_IndexCount += 6;
		m_DataBase.Stats.QuadCount++;
		//m_DataBase.VertexBufferDataPtr->Position = transform * m_DataBase.VertexOriPosition[0];
		//m_DataBase.VertexBufferDataPtr->Color = tintcolor;
		//m_DataBase.VertexBufferDataPtr->TexCoord = { 0.0f, 0.0f };	//0
		//m_DataBase.VertexBufferDataPtr->TexIndex = tex_index;
		//m_DataBase.VertexBufferDataPtr->TilingFector = tilingfactor;
		//m_DataBase.VertexBufferDataPtr++;
		//m_DataBase.VertexBufferDataPtr->Position = transform * m_DataBase.VertexOriPosition[1];
		//m_DataBase.VertexBufferDataPtr->Color = tintcolor;
		//m_DataBase.VertexBufferDataPtr->TexCoord = { 1.0f, 0.0f };	//1
		//m_DataBase.VertexBufferDataPtr->TexIndex = tex_index;
		//m_DataBase.VertexBufferDataPtr->TilingFector = tilingfactor;
		//m_DataBase.VertexBufferDataPtr++;
		//m_DataBase.VertexBufferDataPtr->Position = transform * m_DataBase.VertexOriPosition[2];
		//m_DataBase.VertexBufferDataPtr->Color = tintcolor;
		//m_DataBase.VertexBufferDataPtr->TexCoord = { 1.0f, 1.0f };	//2
		//m_DataBase.VertexBufferDataPtr->TexIndex = tex_index;
		//m_DataBase.VertexBufferDataPtr->TilingFector = tilingfactor;
		//m_DataBase.VertexBufferDataPtr++;
		//m_DataBase.VertexBufferDataPtr->Position = transform * m_DataBase.VertexOriPosition[3];
		//m_DataBase.VertexBufferDataPtr->Color = tintcolor;
		//m_DataBase.VertexBufferDataPtr->TexCoord = { 0.0f, 1.0f };	//3
		//m_DataBase.VertexBufferDataPtr->TexIndex = tex_index;
		//m_DataBase.VertexBufferDataPtr->TilingFector = tilingfactor;
		//m_DataBase.VertexBufferDataPtr++;
	}
	void Renderer2D::DrawRotationQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const float tilingfactor, const glm::vec4& tintcolor)
	{
		DrawRotationQuad({ position.x, position.y, 0.0f }, rotation, size, subtexture, tilingfactor, tintcolor);
	}
	void Renderer2D::DrawRotationQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const float tilingfactor, const glm::vec4& tintcolor)
	{
		TZ_PROFILE_FUNCTION_SIG();
		const Ref<Texture2D> texture = subtexture->GetTextureSheet();
		const glm::vec2* tex_coord = subtexture->GetTextureCoords();

		/*-----------------------------------------------------------------------------*/
		/*-----------------------检测批渲染最大方块数----------------------------------*/
		if (m_DataBase.Quad_IndexCount >= Renderer2D_Data::MaxIndex)
		{
			ResetAndFlush();
		}
		/*-----------------------检测批渲染最大方块数----------------------------------*/
		/*-----------------------------------------------------------------------------*/
		/*----------------------------绑定纹理-----------------------------------------*/
		float tex_index = 0;		//我们要画的纹理的索引
		/*0号位为白纹理*/
		for (int i = 1; i < m_DataBase.TextureSlotIndex; i++)
		{
			/*先找我们已经存了的纹理中有没有我们要画的那个*/
			if (*texture.get() == *m_DataBase.TextureSlotBase[i].get())
			{
				tex_index = (float)i;	/*有的话就设为那个值*/
				break;
			}
		}
		if (tex_index == 0)	/*假如循环过后依旧为0说明没找到*/
		{
			tex_index = (float)m_DataBase.TextureSlotIndex;

			m_DataBase.TextureSlotBase[m_DataBase.TextureSlotIndex] = texture;
			m_DataBase.TextureSlotIndex++;
		}
		/*----------------------------绑定纹理-----------------------------------------*/
		/*-----------------------------------------------------------------------------*/
		/*----------------------------变换矩阵-----------------------------------------*/
		glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
			* glm::rotate(glm::mat4(1.0), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
		/*----------------------------变换矩阵-----------------------------------------*/
		/*-----------------------------------------------------------------------------*/

		for (size_t i = 0; i < 4; i++)	//四个点挨个放到顶点缓冲区数据中
		{
			m_DataBase.VertexBufferDataPtr->Position = transform * m_DataBase.VertexOriPosition[i];
			m_DataBase.VertexBufferDataPtr->Color = tintcolor;
			m_DataBase.VertexBufferDataPtr->TexCoord = tex_coord[i];
			m_DataBase.VertexBufferDataPtr->TexIndex = tex_index;
			m_DataBase.VertexBufferDataPtr->TilingFector = tilingfactor;
			m_DataBase.VertexBufferDataPtr++;
		}
		m_DataBase.Quad_IndexCount += 6;
		m_DataBase.Stats.QuadCount++;
	}
	/*-----------------------------------------------------------旋转版本----------------------------------------------------------------------------------------*/

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& SpriteRenderComponent, int entityID)
	{
		if (SpriteRenderComponent.Texture){
			DrawQuad(transform, SpriteRenderComponent.Texture, SpriteRenderComponent.Tilingfactor, SpriteRenderComponent.Color, entityID);
		}
		else{
			DrawQuad(transform, SpriteRenderComponent.Color, entityID);
		}
	}



	/*-------------------统计相关----------------------------*/
	/*-------------重置统计----------------*/
	void Renderer2D::ResetStats(){
		memset(&m_DataBase.Stats, 0, sizeof(Statistics));
	}
	/*-----------返回统计数据--------------*/
	Renderer2D::Statistics Renderer2D::GetStats(){
		return m_DataBase.Stats;
	}
}