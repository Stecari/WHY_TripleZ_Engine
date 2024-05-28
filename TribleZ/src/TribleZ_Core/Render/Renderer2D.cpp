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

	//����Ϊ��Editor
	int		EntityID;
};


namespace TribleZ
{
	/*�Ҿ�����һ�ηŵ�2D��Ⱦ��class�к�һ��*/  /*��̣�������CppԴ�ļ���û�й�ϵ*/
	struct Renderer2D_Data
	{
		static const uint32_t MaxQuads		 = 256;			//�������㻺�����д洢�������ı��ε�����
		static const uint32_t MaxVertex		 = MaxQuads * 4;	//�������㻺�����д洢�����Ķ�����
		static const uint32_t MaxIndex		 = MaxQuads * 6;	//���������������д洢������������
		static const uint32_t MaxTextureSlot = 32;				//Ӳ�����֧�ֵ�ͬʱ�󶨵�������

		Ref<Shader>				Texture_Shader;
		Ref<VertexArray>		Quad_VertexArray;
		Ref<VertexBuffer>		Quad_VertexBuffer;
		Ref<Texture2D>			WhiteTexture;

		uint32_t Quad_IndexCount	= 0;

		Vertex* VertexBufferDataBase	= nullptr;		//�洢���㻺���������ݣ�ͬʱҲ�Ƕ��㻺�������ݵ��׵�ַ
		Vertex* VertexBufferDataPtr		= nullptr;		//ָ�򶥵㻺�������ݵ�ĩ��ַ��Ҳ���ǵ�ǰָ��

		std::array<Ref<Texture>, MaxTextureSlot> TextureSlotBase;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 VertexOriPosition[4];

		Renderer2D::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;					
		Ref<UniformBuffer> CameraUniformBuffer; //���������
	};
	static Renderer2D_Data m_DataBase;


	void Renderer2D::Init()
	{
		TZ_PROFILE_FUNCTION_SIG();

		/*---------������������---------------------------------------------------------*/
		m_DataBase.Quad_VertexArray = VertexArray::Create();
		/*---------�������㻺��---------*/

		m_DataBase.Quad_VertexBuffer = VertexBuffer::Create(m_DataBase.MaxVertex * sizeof(Vertex));												//shared_ptr.reset(ptr) �ض������������ָ��ָ��ptr������
		//m_VertexBuffer = VertexBuffer::Create(sizeof(Flat_VertexData), Flat_VertexData);												//shared_ptr.reset(ptr) �ض������������ָ��ָ��ptr������
		//m_VertexBuffer->Bind();
		/*----��������ʹ�ܲ����ò���----*/
		BufferLayout layout({
			{ "a_Position",		ShaderDataType::Float3	},
			{ "a_Color",		ShaderDataType::Float4	},
			{ "a_TexCoord",		ShaderDataType::Float2	},
			{ "a_TexIndex",		ShaderDataType::Float1	},
			{ "a_Tiling",		ShaderDataType::Float1	},
			{ "a_EntityID",		ShaderDataType::Int1	}
			});
		m_DataBase.Quad_VertexBuffer->SetLayout(layout); //�󶨻�����ǰ�����ò��֣���Ȼ����û����
		m_DataBase.Quad_VertexArray->AddVertexBuffer(m_DataBase.Quad_VertexBuffer);
		
		m_DataBase.VertexBufferDataBase = new Vertex[m_DataBase.MaxVertex];		//�����㹻��Ŀռ�
		/*---------������������---------*/
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


		/*-----------------------���òο�------------------------------------------------------------*/
		/*--------------------���������-------------------------------------------------------------*/
		m_DataBase.WhiteTexture = Texture2D::Create(1, 1);		 /*   R  G  B  A  */
		unsigned int WhiteTextureData = 0xffffffff;				/* 0xff ff ff ff */
		m_DataBase.WhiteTexture->SetData(&WhiteTextureData, sizeof(uint32_t));
		m_DataBase.TextureSlotBase[0] = m_DataBase.WhiteTexture;	//0�Ų���ǰ�����
		/*--------------------���������-------------------------------------------------------------*/
		int texture[32];
		for (int i = 0; i < m_DataBase.MaxTextureSlot; i++){
			texture[i] = i;
		}
		/*--------------------���ƻ�׼��-------------------------------------------------------------*/
		m_DataBase.VertexOriPosition[0] = {-0.5f, -0.5f, 0.0f, 1.0f};		//����
		m_DataBase.VertexOriPosition[1] = { 0.5f, -0.5f, 0.0f, 1.0f};		//����
		m_DataBase.VertexOriPosition[2] = { 0.5f,  0.5f, 0.0f, 1.0f};		//����
		m_DataBase.VertexOriPosition[3] = {-0.5f,  0.5f, 0.0f, 1.0f};		//����
		/*------------------�������������-----------------------------------------------------------*/
		m_DataBase.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2D_Data::CameraData), 0);

		/*-------------------��ȡ��ɫ��Դ��-----------------------------------------------------------*/
		//m_DataBase.Texture_Shader = Shader::Create("asserts/shader/Editor_TextureShader.glsl");
		//m_DataBase.Texture_Shader = Shader::Create("asserts/shader/Editor_TextureShader_official.glsl");
		m_DataBase.Texture_Shader = Shader::Create("asserts/shader/cherno.glsl");
		/*-------------------��ȡ��ɫ��Դ��-----------------------------------------------------------*/
		//m_DataBase.Texture_Shader->Bind();
		//m_DataBase.Texture_Shader->SetIntArray("u_Texture", texture, 32);//��texture[32]���鴫��uniform u_Texture
	}

	void Renderer2D::ShutDown()
	{
		TZ_PROFILE_FUNCTION_SIG()
	}


	void Renderer2D::SceneBegin(const BaseCamera& camera, const glm::mat4& transform)	//��ʼ����Ⱦ�������ݿ�m_DataBase
	{
		TZ_PROFILE_FUNCTION_SIG();
		StartBatch();

		glm::mat4 ViewProjection = camera.GetProjection() * glm::inverse(transform);	//����������ı任����Ҫȡ��

		m_DataBase.CameraBuffer.ViewProjection = ViewProjection;
		m_DataBase.CameraUniformBuffer->SetData(&m_DataBase.CameraBuffer, sizeof(Renderer2D_Data::CameraData), 0);
		//��API
		//m_DataBase.Texture_Shader->Bind();
		//m_DataBase.Texture_Shader->SetMat4("u_ViewProjection", ViewProjection);

	}
	void Renderer2D::SceneBegin(const OrthoGraphicCamera& camera)	
	{
		TZ_PROFILE_FUNCTION_SIG();
		StartBatch();
		//��֪��Ϊʲô������øģ��������ò��ϰ�
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

		//glm::mat4 ViewProjection = camera.GetViewProjection();	//����������ı任����Ҫȡ��
		//m_DataBase.Texture_Shader->Bind();
		//m_DataBase.Texture_Shader->SetMat4("u_ViewProjection", ViewProjection);
	}

	void Renderer2D::StartBatch()
	{
		m_DataBase.Quad_IndexCount = 0;	//�������¿�ʼ����
		m_DataBase.TextureSlotIndex = 1;//���������¿�ʼ����(0���ǰ�����)
		m_DataBase.VertexBufferDataPtr = m_DataBase.VertexBufferDataBase;				//����ָ�븳ֵ
	}

	void Renderer2D::SceneEnd()
	{
		TZ_PROFILE_FUNCTION_SIG();

		uint32_t data_size = (uint8_t*)m_DataBase.VertexBufferDataPtr - (uint8_t*)m_DataBase.VertexBufferDataBase;//�����ֽ�����ֱ�Ӽ���������ݸ���
		m_DataBase.Quad_VertexBuffer->SetData(m_DataBase.VertexBufferDataBase, data_size);

		Flush();
	}

	void Renderer2D::Flush()
	{
		//if (m_DataBase.Quad_IndexCount == 0)
			//return; // Nothing to draw

		for (int i = 0; i < m_DataBase.TextureSlotIndex; i++) {		//������									
			m_DataBase.TextureSlotBase[i]->Bind(i);	//��i�Ų��   
		}
		/*-----???----------------------*/		//������
		m_DataBase.Texture_Shader->Bind();		//��֪��Ϊʲô��
		m_DataBase.Quad_VertexArray->Bind();	//��֪��Ϊʲôû��		//�󶨶�������
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
	/*-------------------------------------------------------��ȾͼԪ����ģʽ------------------------------------------------------------------------------------*/
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		const float tex_index = 0.0f;
		constexpr float tex_coord[8] = {	//������������ĸ���
			0.0f, 0.0f,		//����
			1.0f, 0.0f,		//����
			1.0f, 1.0f,		//����
			0.0f, 1.0f		//����
		};
		if (m_DataBase.Quad_IndexCount >= Renderer2D_Data::MaxIndex){
			ResetAndFlush();
		}
		for (size_t i = 0; i < 4; i++)	//�ĸ��㰤���ŵ����㻺����������
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
		float tex_index = 0;			//����Ҫ�������������
		constexpr float tex_coord[8] = {	//������������ĸ���
			0.0f, 0.0f,		//����
			1.0f, 0.0f,		//����
			1.0f, 1.0f,		//����
			0.0f, 1.0f		//����
		};
		if (m_DataBase.Quad_IndexCount >= Renderer2D_Data::MaxIndex){
			ResetAndFlush();
		}
		/*----------------------------������-----------------------------------------*/
		/*0��λΪ������*/
		for (int i = 1; i < m_DataBase.TextureSlotIndex; i++)
		{
			/*���������Ѿ����˵���������û������Ҫ�����Ǹ�*/
			if (*texture.get() == *m_DataBase.TextureSlotBase[i].get())
			{
				tex_index = (float)i;	/*�еĻ�����Ϊ�Ǹ�ֵ*/
				break;
			}
		}
		if (tex_index == 0)	/*����ѭ����������Ϊ0˵��û�ҵ�*/
		{
			tex_index = (float)m_DataBase.TextureSlotIndex;

			m_DataBase.TextureSlotBase[m_DataBase.TextureSlotIndex] = texture;
			m_DataBase.TextureSlotIndex++;
		}
		/*----------------------------������-----------------------------------------*/
		/*-----------------------------------------------------------------------------*/
		for (size_t i = 0; i < 4; i++)	//�ĸ��㰤���ŵ����㻺����������
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
	/*-----------------------------------------------------------��ɫ����----------------------------------------------------------------------------------------*/
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		TZ_PROFILE_FUNCTION_SIG();
		/*----------------------------�任����-----------------------------------------*/
		glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
			* glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
		/*----------------------------�任����-----------------------------------------*/

		DrawQuad(transform, color);
	}
	/*-----------------------------------------------------------������----------------------------------------------------------------------------------------*/
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingfactor, const glm::vec4& tintcolor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingfactor, tintcolor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingfactor, const glm::vec4& tintcolor)
	{
		TZ_PROFILE_FUNCTION_SIG();
		/*----------------------------�任����-----------------------------------------*/
		glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
			* glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
		/*----------------------------�任����-----------------------------------------*/

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
		/*----------------------------������-----------------------------------------*/
		float tex_index = 0;		//����Ҫ�������������
		/*0��λΪ������*/
		for (int i = 1; i < m_DataBase.TextureSlotIndex; i++)
		{
			/*���������Ѿ����˵���������û������Ҫ�����Ǹ�*/
			if (*texture.get() == *m_DataBase.TextureSlotBase[i].get())
			{
				tex_index = (float)i;	/*�еĻ�����Ϊ�Ǹ�ֵ*/
				break;
			}
		}
		if (tex_index == 0)	/*����ѭ����������Ϊ0˵��û�ҵ�*/
		{
			tex_index = (float)m_DataBase.TextureSlotIndex;

			m_DataBase.TextureSlotBase[m_DataBase.TextureSlotIndex] = texture;
			m_DataBase.TextureSlotIndex++;
		}
		/*----------------------------������-----------------------------------------*/
		/*-----------------------------------------------------------------------------*/
		/*----------------------------�任����-----------------------------------------*/
		glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
			* glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
		/*----------------------------�任����-----------------------------------------*/
		/*-----------------------------------------------------------------------------*/
		for (size_t i = 0; i < 4; i++)	//�ĸ��㰤���ŵ����㻺����������
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
	/*-----------------------------------------------------------������----------------------------------------------------------------------------------------*/


	/*-----------------------------------------------------------��ת�汾----------------------------------------------------------------------------------------*/
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

		/*----------------------------�任����-----------------------------------------*/
		glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
			* glm::rotate(glm::mat4(1.0), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
		/*----------------------------�任����-----------------------------------------*/


		float tex_coord[8] = {	//������������ĸ���
			0.0f, 0.0f,		//����
			1.0f, 0.0f,		//����
			1.0f, 1.0f,		//����
			0.0f, 1.0f		//����
		};
		for (size_t i = 0; i < 4; i++)	//�ĸ��㰤���ŵ����㻺����������
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
		/*-----------------------�������Ⱦ��󷽿���----------------------------------*/
		if (m_DataBase.Quad_IndexCount >= Renderer2D_Data::MaxIndex)
		{
			ResetAndFlush();
		}
		/*-----------------------�������Ⱦ��󷽿���----------------------------------*/
		/*-----------------------------------------------------------------------------*/
		/*----------------------------������-----------------------------------------*/
		float tex_index = 0;		//����Ҫ�������������
		/*0��λΪ������*/
		for (int i = 1; i < m_DataBase.TextureSlotIndex; i++)
		{
			/*���������Ѿ����˵���������û������Ҫ�����Ǹ�*/
			if (*texture.get() == *m_DataBase.TextureSlotBase[i].get())
			{
				tex_index = (float)i;	/*�еĻ�����Ϊ�Ǹ�ֵ*/
				break;
			}
		}
		if (tex_index == 0)	/*����ѭ����������Ϊ0˵��û�ҵ�*/
		{
			tex_index = (float)m_DataBase.TextureSlotIndex;

			m_DataBase.TextureSlotBase[m_DataBase.TextureSlotIndex] = texture;
			m_DataBase.TextureSlotIndex++;
		}
		/*----------------------------������-----------------------------------------*/
		/*-----------------------------------------------------------------------------*/
		/*----------------------------�任����-----------------------------------------*/
		glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
							* glm::rotate(glm::mat4(1.0), rotation, { 0.0f, 0.0f, 1.0f })
							* glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
		/*----------------------------�任����-----------------------------------------*/
		/*-----------------------------------------------------------------------------*/
		float tex_coord[8] = {	//������������ĸ���
			0.0f, 0.0f,		//����
			1.0f, 0.0f,		//����
			1.0f, 1.0f,		//����
			0.0f, 1.0f		//����
		};
		for (size_t i = 0; i < 4; i++)	//�ĸ��㰤���ŵ����㻺����������
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
		/*-----------------------�������Ⱦ��󷽿���----------------------------------*/
		if (m_DataBase.Quad_IndexCount >= Renderer2D_Data::MaxIndex)
		{
			ResetAndFlush();
		}
		/*-----------------------�������Ⱦ��󷽿���----------------------------------*/
		/*-----------------------------------------------------------------------------*/
		/*----------------------------������-----------------------------------------*/
		float tex_index = 0;		//����Ҫ�������������
		/*0��λΪ������*/
		for (int i = 1; i < m_DataBase.TextureSlotIndex; i++)
		{
			/*���������Ѿ����˵���������û������Ҫ�����Ǹ�*/
			if (*texture.get() == *m_DataBase.TextureSlotBase[i].get())
			{
				tex_index = (float)i;	/*�еĻ�����Ϊ�Ǹ�ֵ*/
				break;
			}
		}
		if (tex_index == 0)	/*����ѭ����������Ϊ0˵��û�ҵ�*/
		{
			tex_index = (float)m_DataBase.TextureSlotIndex;

			m_DataBase.TextureSlotBase[m_DataBase.TextureSlotIndex] = texture;
			m_DataBase.TextureSlotIndex++;
		}
		/*----------------------------������-----------------------------------------*/
		/*-----------------------------------------------------------------------------*/
		/*----------------------------�任����-----------------------------------------*/
		glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
			* glm::rotate(glm::mat4(1.0), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
		/*----------------------------�任����-----------------------------------------*/
		/*-----------------------------------------------------------------------------*/

		for (size_t i = 0; i < 4; i++)	//�ĸ��㰤���ŵ����㻺����������
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
	/*-----------------------------------------------------------��ת�汾----------------------------------------------------------------------------------------*/

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& SpriteRenderComponent, int entityID)
	{
		if (SpriteRenderComponent.Texture){
			DrawQuad(transform, SpriteRenderComponent.Texture, SpriteRenderComponent.Tilingfactor, SpriteRenderComponent.Color, entityID);
		}
		else{
			DrawQuad(transform, SpriteRenderComponent.Color, entityID);
		}
	}



	/*-------------------ͳ�����----------------------------*/
	/*-------------����ͳ��----------------*/
	void Renderer2D::ResetStats(){
		memset(&m_DataBase.Stats, 0, sizeof(Statistics));
	}
	/*-----------����ͳ������--------------*/
	Renderer2D::Statistics Renderer2D::GetStats(){
		return m_DataBase.Stats;
	}
}