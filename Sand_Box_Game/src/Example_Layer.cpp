#include <vector>
#include "TribleZ.h"
//#include <TribleZ.h>  //���Ҳ���ԣ�������Ϊ�˷ֵ����һ���������Ǹ�

#include <glm/vec3.hpp>		// glm::vec3
#include <glm/vec4.hpp>		// glm::vec4
#include <glm/mat4x4.hpp>	// glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective(͸�ӣ�ͶӰ��ϵ������)
#include <glm/gtc/type_ptr.hpp>

#include "ImGui/imgui.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Platform/OpenGL/OpenGLShader.h"


glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

std::string GetShaderSourceCode(const std::string& filepath)
{
	std::ifstream fstream(filepath);	//�����ļ���
	std::stringstream code_stream;
	std::string Line;
	while (getline(fstream, Line))
	{
		code_stream << Line << "\n";
	}
	return code_stream.str();
}


class ExampleLayer : public TribleZ::Layer
{
private:
	/*--------------��Ⱦ���------------------------------------*/
	TribleZ::ShaderLibrary				m_ShaderLib;		//���ﲻ����Ref,��Ȼ����û��������Ӧ�ã����ɿ�ָ�롣

	TribleZ::Ref<TribleZ::VertexArray>	m_VertexArray;

	TribleZ::Ref<TribleZ::VertexArray>	m_VertexArray_s;

	TribleZ::Ref<TribleZ::Texture2D>	m_Texture, Tmp_Texture;
	//TribleZ::Ref<TribleZ::Texture2D>	SpriteSheet_Tex;

	TribleZ::OrthoGraphicCameraContraller CameraController;
	/*--------------��Ⱦ���------------------------------------*/

public:
	ExampleLayer()
		:Layer("Example"), CameraController(1280.0f / 720.0f, true),
		ModelPosition({ 0.0f, 0.0f, 0.0f }), ModelRotation(0.0f), ModelScale(1.0)
	{
		/*---------��������-------------------------------------------------------------*/
		float VertexData[4 * 7]{
			/*λ��*/
		 300.0f ,300.0f, 0.0f, 0.4f, 0.5f, 0.1f, 1.0f,
		 600.0f, 300.0f, 0.0f, 0.6f, 0.5f, 0.2f, 1.0f,
		 600.0f, 600.0f, 0.0f, 0.5f, 0.1f, 0.8f, 1.0f,
		 300.0f, 600.0f, 0.0f, 0.5f, 0.2f, 0.9f, 1.0f
		};
		/*---------��������-------------------------------------------------------------*/
		unsigned int IndexTable[]{
			0, 1, 2,
			2, 0, 3
		};
		/*------------------------------------------------------------------------------*/


		/*---------������������---------------------------------------------------------*/
		m_VertexArray = TribleZ::VertexArray::Create();
		/*---------�������㻺��---------*/
		TribleZ::Ref<TribleZ::VertexBuffer> m_VertexBuffer;
		m_VertexBuffer = TribleZ::VertexBuffer::Create(sizeof(VertexData), VertexData);
		//m_VertexBuffer->Bind();
		/*----��������ʹ�ܲ����ò���----*/
		TribleZ::BufferLayout layout({
			{ "a_Position", TribleZ::ShaderDataType::Float3 },
			{ "a_Color",    TribleZ::ShaderDataType::Float4 },
			//{ "m_Normal",   ShaderDataType::Float1 },
			});

		m_VertexBuffer->SetLayout(layout); //�󶨻�����ǰ�����ò��֣���Ȼ����û����
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		/*---------������������---------*/
		TribleZ::Ref<TribleZ::IndexBuffer> m_IndexBuffer;
		m_IndexBuffer = TribleZ::IndexBuffer::Create(sizeof(IndexTable) / sizeof(unsigned int), IndexTable);
		m_IndexBuffer->Bind();
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		/*------------------------------------------------------------------------------*/


		/*---------��������-------------------------------------------------------------*/
		float VertexData_s[4 * 5]{
			/*	   λ     ��	  | ��������	*/
			 400.0f ,400.0f, 0.0f, 0.0f, 0.0f,
			 700.0f, 400.0f, 0.0f, 1.0f, 0.0f,
			 700.0f, 700.0f, 0.0f, 1.0f, 1.0f,
			 400.0f, 700.0f, 0.0f, 0.0f, 1.0f
		};
		/*---------��������-------------------------------------------------------------*/
		unsigned int IndexTable_s[]{
			0, 1, 2,
			2, 3, 0
		};
		/*------------------------------------------------------------------------------*/


		/*---------������������---------------------------------------------------------*/
		m_VertexArray_s = TribleZ::VertexArray::Create();
		//m_VertexArray->Bind();
		/*---------�������㻺��---------*/
		TribleZ::Ref<TribleZ::VertexBuffer> vb;
		vb = TribleZ::VertexBuffer::Create(sizeof(VertexData_s), VertexData_s);
		//m_VertexBuffer->Bind();
		/*----��������ʹ�ܲ����ò���----*/
		vb->SetLayout({ { "a_Position", TribleZ::ShaderDataType::Float3 },
						{ "a_TexCoord", TribleZ::ShaderDataType::Float2 } }); //�󶨻�����ǰ�����ò��֣���Ȼ����û����
		m_VertexArray_s->AddVertexBuffer(vb);
		/*---------������������---------*/
		TribleZ::Ref<TribleZ::IndexBuffer> IB;
		IB = TribleZ::IndexBuffer::Create(sizeof(IndexTable_s) / sizeof(unsigned int), IndexTable_s);
		IB->Bind();
		m_VertexArray_s->SetIndexBuffer(IB);
		/*------------------------------------------------------------------------------*/

		/*-------------------��ȡ��ɫ��Դ��--------------------------------------------------------------------------------------------------------*/
		//�����ž���һ����ʼ��������������ָ���õ�
		auto MemberShader = m_ShaderLib.LoadShader("asserts/shader/m_Shader.glsl");

		auto MashShader = m_ShaderLib.LoadShader("asserts/shader/Mash_Shader.glsl");

		auto m_Texture_Shader = m_ShaderLib.LoadShader("Texture", "asserts/shader/Texture_Shader.glsl");

		/*-------------------��ȡ��ɫ��Դ��--------------------------------------------------------------------------------------------------------*/

		/*--------------��ȡ��������������ɫ��---------------------------------------------------------------------------------------------------*/
		m_Texture = TribleZ::Texture2D::Create("asserts/img/LOGO/LOGO4.png");
		Tmp_Texture = TribleZ::Texture2D::Create("asserts/img/LOGO/LOGO6.png");
		std::dynamic_pointer_cast<TribleZ::OpenGLShader>(m_Texture_Shader)->Bind();
		std::dynamic_pointer_cast<TribleZ::OpenGLShader>(m_Texture_Shader)->SetUniform1i("u_Texture1", 0);
		/*--------------��ȡ��������������ɫ��---------------------------------------------------------------------------------------------------*/

	}

	void OnUpdata(TribleZ::TimeStep time_step) override		//��ѯ
	{
		CameraController.OnUpdata(time_step);


		TribleZ::RendererCommand::SetClearColor({ 0.3f, 0.3f, 0.3f, 1.0f });
		TribleZ::RendererCommand::Clear();

		//TZ_CLIENT_INFO("delta time = {0} ( {1} )", time_step.GetSecond(), time_step.GetMilliSecond());


		/*-------------------------ģ�ͱ任----------------------------------------------------*/
		//if (TribleZ::Input::IsKeyPressed(TZ_KEY_W))
		//{
		//	ModelPosition.y += ModelPositionSpeed * time_step;	//��������������һ��float()
		//}
		//else if (TribleZ::Input::IsKeyPressed(TZ_KEY_S))
		//{
		//	ModelPosition.y -= ModelPositionSpeed * time_step;	//��ʽת��������д����float(time_step)
		//}
		//if (TribleZ::Input::IsKeyPressed(TZ_KEY_A))
		//{
		//	ModelPosition.x -= ModelPositionSpeed * time_step;//�൱����time_step.GetSecond()
		//}
		//else if (TribleZ::Input::IsKeyPressed(TZ_KEY_D))
		//{
		//	ModelPosition.x += ModelPositionSpeed * time_step;
		//}
		//if (TribleZ::Input::IsMouseButtonPressed(TZ_MOUSE_BUTTON_1))
		//{
		//	ModelRotation += ModelRotationSpeed * time_step.GetSecond();
		//}
		//else if (TribleZ::Input::IsMouseButtonPressed(TZ_MOUSE_BUTTON_2))
		//{
		//	ModelRotation -= ModelRotationSpeed * time_step.GetSecond();
		//}
		/*-------------------------ģ�ͱ任----------------------------------------------------*/


		/*------------------��ʼ����-------------------------------------------------------------------------------------*/
		TribleZ::Renderer::SceneBegin(CameraController.GetCamera());	//���������ز���

		std::dynamic_pointer_cast<TribleZ::OpenGLShader>(m_ShaderLib.GetShader("Mash_Shader"))->Bind();
		std::dynamic_pointer_cast<TribleZ::OpenGLShader>(m_ShaderLib.GetShader("Mash_Shader"))->SetUniform3f("u_Color", color_editor);

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				glm::vec3 pos = { 400.0f, 400.0f, -0.1f };
				pos.x += i * 1.1 * 300 * 0.2;
				pos.y += j * 1.1 * 300 * 0.2;
				glm::mat4 Model_mat_s = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), glm::vec3{ 0.2 });
				TribleZ::Renderer::Submit(m_ShaderLib.GetShader("Mash_Shader"), m_VertexArray_s, Model_mat_s);
			}
		}

		m_Texture->Bind(0);
		TribleZ::Renderer::Submit(m_ShaderLib.GetShader("Texture"), m_VertexArray_s, glm::scale(glm::mat4(1.0f), glm::vec3{ 1.1f }));
		Tmp_Texture->Bind(0);
		TribleZ::Renderer::Submit(m_ShaderLib.GetShader("Texture"), m_VertexArray_s, glm::scale(glm::mat4(1.0f), glm::vec3{ 1.1f }));

		TribleZ::Renderer::SceneEnd();
		/*------------------��������-------------------------------------------------------------------------------------*/

	}

	virtual void OnImGuiRender() override
	{
		//ImGui::Begin("Nice Color");
		//ImGui::ColorEdit3("Mesh Color", glm::value_ptr(color_editor));
		//ImGui::End();
	}

	virtual void OnEvent(TribleZ::Event& event) override		//�����¼�
	{
		TribleZ::EventDispatcher Dispatcher(event);
		Dispatcher.DisPatcher<TribleZ::EventMouseScrolled>(TZ_CORE_BIND_EVENT_Fn(&ExampleLayer::EventOnMouseScrolled));

		CameraController.OnEvent(event);
		//if (event.GetEventType() == TribleZ::EventType::KeyPressed)
		//{
		//	if (TribleZ::Input::IsKeyPressed(TZ_KEY_TAB))
		//	{
		//		TZ_CLIENT_INFO("TAB is Pressed!");
		//	}
		//	TZ_CLIENT_INFO("{0}", (char)((TribleZ::EventKeyPress&)event).GetKeyCode());
		//}
	}

	bool EventOnMouseScrolled(TribleZ::EventMouseScrolled& event)
	{
		//float offset_X = event.GetOffsetX();
		float offset_Y = event.GetOffsetY();

		ModelScale += offset_Y * ModelScaleSpeed;
		ModelScale = (ModelScale < 0.3 ? 0.3 : (ModelScale > 3 ? 3 : ModelScale));

		//TZ_CLIENT_INFO("offset_x : {0}, offset_y : {1}", offset_X, offset_Y);
		//TZ_CLIENT_INFO("ModelScale : {0}", ModelScale);

		return true;
	}

private:


	/*-------------ģ�ͱ任����--------------------------*/
	/*---------�ƶ��ٶ�---------*/
	glm::vec3 ModelPosition;
	float ModelPositionSpeed = 500;
	/*---------ת���ٶ�---------*/
	float ModelRotation;
	float ModelRotationSpeed = 60;
	/*---------�����ٶ�---------*/
	float ModelScale;
	float ModelScaleSpeed = 0.05;
	/*-------------ģ�ͱ任����--------------------------*/


	/*-------------��ɫ----------------------------------*/
	glm::vec4 BuleColor = { 0.3f, 0.2f, 0.8f, 1.0f };
	glm::vec4 RedColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec4 GreenColor = { 0.3f, 0.8f, 0.2f, 1.0f };

	glm::vec3 color_editor = { 0.2f, 0.2f, 0.2f };
	/*-------------��ɫ----------------------------------*/

};