#include "tzpch.h"
#include "SandBox2D.h"

#include "TribleZ_Core/Core.h"
#include "TribleZ_Core/Render/Texture.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective(透视，投影关系、比例)
#include <glm/gtc/type_ptr.hpp>

#include "Tool/BenchMarking.h"

#define PARTICLE_SYSTEM_ON 0

SandBox2D_layer::SandBox2D_layer()
	:Layer("SandBox2D"), CameraController(1280.0f / 720.0f, true), m_ParticleSystem(5000)
{

}
static const uint32_t  s_MapWidth = 25;
static const char* TileMap =
"GGGRGGGGGGGGGRRRGGGGGGGGG"
"GGGGGGGGGGGGRRGGGGGGGGGGG"
"GFGGGDDDDDDDDDDDGGGGFFGGG"
"GGGDDDDDDDDDDDDDDDDGGGGGG"
"GGDDDDDDDDDDDDDDDDDDGGGGG"
"GDDDDDDDDDDDDDDDDDDDDDDGG"
"GDDDDDDDDDDDDDDDDDDDDDDDG"
"DDDDDDDDDDDDDDDDDDDDDDDDG"
"DDDDDDDDDDRRDDDDDDDDDDDDD"
"DDDDDDDDDDDDDDDDDDDDDDDDD"
"DDDDDDDDDDDDDDDDDDDDDDDDD"
"DDDDDDDDDDDDDDDDDDDDDDDDD"
"DDDDDDDDDDDDDDDDDDDDDDDDD"
"WWWWWWWWWWWWWWWWWWWWWWWWW";

void SandBox2D_layer::OnAttach()
{
	//纹理加载
	Texture_2D = TribleZ::Texture2D::Create("asserts/img/LOGO/LOGO4.png");
	SpriteSheet_tex = TribleZ::Texture2D::Create("asserts/img/game/texture/tilemap_sheet.png");

	//子纹理初始化
	subTex = TribleZ::SubTexture2D::CreateFromCoords(SpriteSheet_tex, { 10, 10 }, { 16.0, 16.0 }, {1.0f, 1.0f});
	m_SubTexMap['D'] = TribleZ::SubTexture2D::CreateFromCoords(SpriteSheet_tex, {  1, 8  }, { 16.0, 16.0 }, { 1.0f, 1.0f });
	m_SubTexMap['G'] = TribleZ::SubTexture2D::CreateFromCoords(SpriteSheet_tex, {  0, 10 }, { 16.0, 16.0 }, { 1.0f, 1.0f });
	m_SubTexMap['F'] = TribleZ::SubTexture2D::CreateFromCoords(SpriteSheet_tex, {  2, 10 }, { 16.0, 16.0 }, { 1.0f, 1.0f });
	m_SubTexMap['R'] = TribleZ::SubTexture2D::CreateFromCoords(SpriteSheet_tex, { 7, 7 }, { 16.0, 16.0 }, { 1.0f, 1.0f });
	m_SubTexMap['W'] = TribleZ::SubTexture2D::CreateFromCoords(SpriteSheet_tex, {  0, 6  }, { 16.0, 16.0 }, { 1.0f, 1.0f });
	//m_SubTexMap['H'] = TribleZ::SubTexture2D::CreateFromCoords(SpriteSheet_tex, {  0, 3  }, { 16.0, 16.0 }, { 4.0f, 4.0f });

	//地图属性初始化
	Map_Width = s_MapWidth;
	Map_Height = strlen(TileMap) / s_MapWidth;

	//粒子初始化
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd   = { 254 / 255.0f, 109 / 255.0f,  41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 35.0f, m_Particle.SizeVariation = 20.0f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.2f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 400.0f, 200.0f };
	m_Particle.Position = { 0.0f, 0.0f };

	//视口初始化
	CameraController.SetZoomLevel(720.0f * 2);
}

void SandBox2D_layer::OnDetach()
{

}

void SandBox2D_layer::OnUpdata(TribleZ::TimeStep time_step)
{
	TZ_PROFILE_FUNCTION();	//计时器，基准测试

		CameraController.OnUpdata(time_step);

		TribleZ::Renderer2D::ResetStats();

		static float rotation = 20.0f;
		rotation += 20.0f * time_step;
	{
		TZ_PROFILE_SCOPE("RENDERER");
		TribleZ::RendererCommand::SetClearColor({ 0.3f, 0.3f, 0.3f, 1.0f });
		TribleZ::RendererCommand::Clear();

		TribleZ::Renderer2D::SceneBegin(CameraController.GetCamera());
		//TribleZ::Renderer2D::DrawQuad({ 1000.0f, -500.0f, 0.1f }, { 800.0f, 800.0f }, { 0.3f, 0.2f, 0.9f, 1.0f });
		//TribleZ::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 800.0f, 800.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		//TribleZ::Renderer2D::DrawRotationQuad({ -40.0f, -80.0f, -0.5f }, glm::radians(rotation), { 500.0f, 500.0f }, { 0.3f, 0.2f, 0.8f, 1.0f });
		//TribleZ::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.2f }, { 1920.0f / 2.0f, 1760.0f / 2.0f }, SpriteSheet_tex);
		//for (float x = -500.0f; x < 500.0f; x += 50.0f)
		//{
		//	for (float y = -500.0f; y < 500.0f; y += 50.0f)
		//	{
		//		glm::vec4 color = { (x + 500.0f) / 1000.0f, 0.4f, (y + 500.0f) / 1000.0f , 0.7f };
		//		TribleZ::Renderer2D::DrawQuad({ x, y }, { 45.0f,45.0f }, color);
		//	}
		//}

		TribleZ::Renderer2D::SceneEnd();


		/*这里有一个很莫名其妙的bug，假如后渲染图形是放在下层的，在它上面的图形的透明度会失效*/
		TribleZ::Renderer2D::SceneBegin(CameraController.GetCamera());
		for (int y = 0; y < Map_Height; y++)
		{
			for (int x = 0; x < Map_Width; x++)
			{
				char block = TileMap[x + y * Map_Width];
				TribleZ::Ref<TribleZ::SubTexture2D> tex;
				if (m_SubTexMap.find(block) != m_SubTexMap.end()) {
					tex = m_SubTexMap[block];
				}
				else {
					tex = m_SubTexMap['R'];
				}

				TribleZ::Renderer2D::DrawQuad({ 160.0f * x - Map_Width * 160.0f / 2 , 160.0f * y - Map_Height * 160.0f / 2, -0.2f }, { 160.0f, 160.0f }, tex);
			}
		}

		//TribleZ::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.2f }, { 160.0f, 160.0f }, subTex);
		//TribleZ::Renderer2D::DrawQuad({ 160.0f, 0.0f, 0.2f }, { 160.0f, 160.0f }, subTex);
		//TribleZ::Renderer2D::DrawQuad({ 320.0f, 0.0f, 0.2f }, { 160.0f, 160.0f }, subTex);
		//TribleZ::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.2f }, { 1920.0f / 2.0f, 1760.0f / -2.0f }, SpriteSheet_tex);
		TribleZ::Renderer2D::SceneEnd();

#if PARTICLE_SYSTEM_ON
		//粒子系统：点击鼠标发射粒子
		if (TribleZ::Input::IsMouseButtonPressed(TZ_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = TribleZ::Input::GetMousePosition();
			auto width  = TribleZ::Application::GetInstence().GetWindow().GetWidth();
			auto height = TribleZ::Application::GetInstence().GetWindow().GetHeight();

			auto bounds = CameraController.GetBound();		//获取边界
			auto pos = CameraController.GetCamera().GetPosition();	//获取相机位置
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5;
			y = bounds.GetHeight() * 0.5 - (y / height) * bounds.GetHeight();
			m_Particle.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 20; i++)
			{
				m_ParticleSystem.Emit(m_Particle);
			}
		}
		m_ParticleSystem.OnUpdata(time_step);
		m_ParticleSystem.OnRender(CameraController.GetCamera());
#endif
	}
	
}

#define Docking_Excemple_Code 0
void SandBox2D_layer::OnImGuiRender()
{
	TZ_PROFILE_FUNCTION_SIG();
	//ImGui::ShowDemoWindow()
	auto stats = TribleZ::Renderer2D::GetStats();

	ImGui::Begin("SandBox2D_layer");
	ImGui::Text("Renderer2D:");
	ImGui::Text("DrawCall: %d", stats.DrawCallCount);
	ImGui::Text("QuadCount: %d", stats.QuadCount);
	ImGui::Text("VertexCount: %d", stats.GetVertexCount());
	ImGui::Text("Index: %d", stats.GetIndexCount());
	ImGui::ColorEdit4("Flat Color", glm::value_ptr(color_editor));
	ImGui::End();
}

void SandBox2D_layer::OnEvent(TribleZ::Event& event)
{
	TZ_PROFILE_FUNCTION_SIG();
	CameraController.OnEvent(event);
}
