#include "tzpch.h"
#include "Editor_Layer.h"

#include "TribleZ_Core/Core.h"
#include "TribleZ_Core/Render/Texture.h"

#include "ImGui/imgui.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective(͸�ӣ�ͶӰ��ϵ������)
#include <glm/gtc/type_ptr.hpp>

#include "Tool/BenchMarking.h"

#include "Scene/SceneSerializer.h"

#define PARTICLE_SYSTEM_ON 0

namespace TribleZ
{
	Editor_Layer::Editor_Layer()
		:Layer("Editor_Layer"), CameraController(1280.0f / 720.0f, true), m_ParticleSystem(5000)
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

	

	void Editor_Layer::OnAttach()
	{
		//�������
		Texture_2D = Texture2D::Create("asserts/img/LOGO/LOGO4.png");
		SpriteSheet_tex = Texture2D::Create("asserts/img/game/texture/tilemap_sheet.png");

		FrameBufferSpecification FrameBuffer_Spec;
		FrameBuffer_Spec.Width = 1280;
		FrameBuffer_Spec.Height = 720;
		FrameBuffer_2D = FrameBuffer::Create(FrameBuffer_Spec);

		//�������ʼ��
		subTex = SubTexture2D::CreateFromCoords(SpriteSheet_tex, { 10, 10 }, { 16.0, 16.0 }, { 1.0f, 1.0f });
		m_SubTexMap['D'] = SubTexture2D::CreateFromCoords(SpriteSheet_tex, { 1, 8 }, { 16.0, 16.0 }, { 1.0f, 1.0f });
		m_SubTexMap['G'] = SubTexture2D::CreateFromCoords(SpriteSheet_tex, { 0, 10 }, { 16.0, 16.0 }, { 1.0f, 1.0f });
		m_SubTexMap['F'] = SubTexture2D::CreateFromCoords(SpriteSheet_tex, { 2, 10 }, { 16.0, 16.0 }, { 1.0f, 1.0f });
		m_SubTexMap['R'] = SubTexture2D::CreateFromCoords(SpriteSheet_tex, { 7, 7 }, { 16.0, 16.0 }, { 1.0f, 1.0f });
		m_SubTexMap['W'] = SubTexture2D::CreateFromCoords(SpriteSheet_tex, { 0, 6 }, { 16.0, 16.0 }, { 1.0f, 1.0f });
		//m_SubTexMap['H'] = SubTexture2D::CreateFromCoords(SpriteSheet_tex, {  0, 3  }, { 16.0, 16.0 }, { 4.0f, 4.0f });

		//��ͼ���Գ�ʼ��
		Map_Width = s_MapWidth;
		Map_Height = strlen(TileMap) / s_MapWidth;

		//���ӳ�ʼ��
		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f,  41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 35.0f, m_Particle.SizeVariation = 20.0f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 1.2f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 400.0f, 200.0f };
		m_Particle.Position = { 0.0f, 0.0f };

		//�ӿڳ�ʼ��
		CameraController.SetZoomLevel(720.0f * 2);

		//������ʼ��
		ActiveScene = CreatRef<Scene>();
		
#if 0
		//��ʼ��ʵ��
		m_Square = ActiveScene->CreateEntity("Square");		//�Դ�һ���任�����Tag���
		m_Square.GetComponent<TransformComponent>().Scale = glm::vec3{ 400.0f };		//Ĭ����1.0�ĵ�λ���󣬵��Ǽ���Ĭ����400�����������ʵ�������ͬʱ����һ���任�������Ⱦ��ʱ��任����ֵ�ᱻƽ����
		m_Square.AddComponent<SpriteRendererComponent>();

		m_second_Square = ActiveScene->CreateEntity("SecondSquare");
		m_second_Square.GetComponent<TransformComponent>().Scale = glm::vec3{ 400.0f };
		m_second_Square.AddComponent<SpriteRendererComponent>(glm::vec4{0.8f, 0.3f, 0.2f, 1.0f });

		Camera_Entity = ActiveScene->CreateEntity("Main_Camera");;
		Camera_Entity.AddComponent<CameraComponent>();

		Obj_Camera = ActiveScene->CreateEntity("Object_Camera");;
		Obj_Camera.AddComponent<CameraComponent>();
		Obj_Camera.GetComponent<CameraComponent>().Primary = false;


		//������ĵ�һ���࣬д������
		class CameraControllerEntity : public ScriptableEntity		//һ�� ��Ϊ��
		{
		public:
			virtual void OnCreate()	override	//����շ���ֵ����������Createʲô������
			{
				GetComponent<TransformComponent>().Translation.x = rand() % 720 - 360;
			}

			virtual void OnDestroy() override
			{
			}

			virtual void OnUpdata(TimeStep timestep) override
			{
				auto& Camera_pos_x = this->GetComponent<TransformComponent>().Translation.x;
				auto& Camera_pos_y = this->GetComponent<TransformComponent>().Translation.y;


				if (Input::IsKeyPressed(TZ_KEY_W))
				{
					Camera_pos_y += 400.0f * timestep;	
				}
				else if (Input::IsKeyPressed(TZ_KEY_S))
				{
					Camera_pos_y -= 400.0f * timestep;	//��ʽת��������д����float(timestep)
				}
				if (Input::IsKeyPressed(TZ_KEY_A))
				{
					Camera_pos_x -= 400.0f * timestep;//�൱����timestep.GetSecond()
				}
				else if (Input::IsKeyPressed(TZ_KEY_D))
				{
					Camera_pos_x += 400.0f * timestep;
				}
			}
		};

		Camera_Entity.AddComponent<NativeScriptComponent>().Bind<CameraControllerEntity>();
		Obj_Camera.AddComponent<NativeScriptComponent>().Bind<CameraControllerEntity>();
#endif

		//����ֲ�����ʼ��
		m_ScenePanel.SetContext(ActiveScene);
	}

	void Editor_Layer::OnDetach()
	{

	}

	void Editor_Layer::OnUpdata(TimeStep time_step)
	{
		TZ_PROFILE_FUNCTION();	//��ʱ������׼����


		//��֪��ʲôʱ��ӵ�resizing
		if (TribleZ::FrameBufferSpecification t_spec = FrameBuffer_2D->GetSpecification();	//if����ж��п��Բ���һ���ֺţ��ֺ�ǰ�����ж�ǰҪ���еز������
			m_ViewSize.x > 0.0f && m_ViewSize.y > 0.0f &&	//��Gui�ӿ�(����)�Ĵ�СҪ����0
			t_spec.Width != m_ViewSize.x || t_spec.Height != m_ViewSize.y)		//�ӿڴ�С������֡��������С
		{
			FrameBuffer_2D->Resize((uint32_t)m_ViewSize.x, (uint32_t)m_ViewSize.y);
			CameraController.ResizeView(m_ViewSize.x, m_ViewSize.y);

			ActiveScene->ResizeView((uint32_t)m_ViewSize.x, (uint32_t)m_ViewSize.y);
		}

		//�������
		if (m_ViewPortFocused){		//ֻ��ѡ�иô���ʱ���������������
			CameraController.OnUpdata(time_step);
		}

		//����ͳ������
		Renderer2D::ResetStats();

		

		//��Ⱦ
		static float rotation = 20.0f;
		rotation += 20.0f * time_step;

		FrameBuffer_2D->Bind();
		{
			TZ_PROFILE_SCOPE("RENDERER");
			RendererCommand::SetClearColor({ 0.3f, 0.3f, 0.3f, 1.0f });
			RendererCommand::Clear();

			//Renderer2D::SceneBegin(CameraController.GetCamera());
			//���³���
			ActiveScene->OnUpdata(time_step);

			//Renderer2D::SceneEnd();

			/*������һ����Ī�������bug���������Ⱦͼ���Ƿ����²�ģ����������ͼ�ε�͸���Ȼ�ʧЧ*/
			//Renderer2D::SceneBegin(CameraController.GetCamera());
			//for (int y = 0; y < Map_Height; y++)
			//{
			//	for (int x = 0; x < Map_Width; x++)
			//	{
			//		char block = TileMap[x + y * Map_Width];
			//		Ref<SubTexture2D> tex;
			//		if (m_SubTexMap.find(block) != m_SubTexMap.end()) {
			//			tex = m_SubTexMap[block];
			//		}
			//		else {
			//			tex = m_SubTexMap['R'];
			//		}
			//		Renderer2D::DrawQuad({ 160.0f * x - Map_Width * 160.0f / 2 , 160.0f * y - Map_Height * 160.0f / 2, -0.2f }, { 160.0f, 160.0f }, tex);
			//	}
			//}
			//Renderer2D::SceneEnd();

			FrameBuffer_2D->UnBind();

#if PARTICLE_SYSTEM_ON
			//����ϵͳ�������귢������
			if (Input::IsMouseButtonPressed(TZ_MOUSE_BUTTON_LEFT))
			{
				auto [x, y] = Input::GetMousePosition();
				auto width = Application::GetInstence().GetWindow().GetWidth();
				auto height = Application::GetInstence().GetWindow().GetHeight();

				auto bounds = CameraController.GetBound();		//��ȡ�߽�
				auto pos = CameraController.GetCamera().GetPosition();	//��ȡ���λ��
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
	void Editor_Layer::OnImGuiRender()
	{
		TZ_PROFILE_FUNCTION_SIG();
		auto stats = Renderer2D::GetStats();
		static bool docking_Enabled = true;
		//static bool opt_fullscreen_persisteand = true;		//persistand  ��������
		static bool opt_fullscreen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		/*
		*��ʹ��ImGuiDockNodeFlags_PasshruCentralNodeʱ��
		*DockSpace()����Ⱦ���ǵı���,���Ҵ���ͨ���ף���������Ҫ��Begin()��Ҫ��Ⱦ������
		*/
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		/*
		*��Ҫ��ʾ����ע�⣬��ʹBegin()����false�������ڱ��۵���������Ҳ�������
		*������Ϊ����ϣ������DockSpace()���ڻ״̬�����DockSpace()���ڷǻ״̬��
		*����ͣ�������еĻ���ڶ���ʧȥ�丸���ڲ���Ϊδͣ�����ڡ�
		*���ǲ��ܱ�������ںͷǻͣ��֮���ͣ����ϵ������
		*dockspace/settings���κθ��Ķ��ᵼ�´���������������Զ��������
		*/
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &docking_Enabled, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)		ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		/*----------------������С���ڿ��-------------------------------*/
		ImGuiStyle& style = ImGui::GetStyle();
		float ori_minWinSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 300.0f;
		/*----------------������С���ڿ��-------------------------------*/
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else {
			TZ_CORE_ERROR("Docking Disabled!!");
		}
		/*----------------������С���ڿ��-------------------------------*/
		style.WindowMinSize.x = ori_minWinSize;
		/*----------------������С���ڿ��-------------------------------*/



		//�����Ǹ�ѡ����
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::Separator();
#if Docking_Excemple_Code 
				if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
				if (ImGui::MenuItem("Flag: NoDockingSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
				if (ImGui::MenuItem("Flag: NoUndocking", "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
#endif
				if (ImGui::MenuItem("Serialize")) 
				{
					SceneSerializer TZ_Serializer(ActiveScene);
					TZ_Serializer.Serializer("asserts/scenes/excmple.tz");
				}

				if (ImGui::MenuItem("Deserialize"))
				{
					/*
					* ���ﻹ��������Bug
					* ����ÿһ�ζ������ݵ�ʱ�򣬲�û�д���һ��ɾ��ԭ����Scene�µ�Scene
					* ��ͻᵼ��ÿһ�ζ��붼����һ���ظ���Scene��һ��Scene������Ӻü���
					*/
					SceneSerializer TZ_Serializer(ActiveScene);
					TZ_Serializer.Deserializer("asserts/scenes/excmple.tz");
				}

				if (ImGui::MenuItem("Exit")) { Application::GetInstence().Close(); }

				ImGui::Separator();

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();


			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));		//�߿����

			//��ʾ�ֲ����
			m_ScenePanel.OnImGuiRender();

			//���ݺͱ༭
			ImGui::Begin("Editor_Layer");
			ImGui::Text("Renderer2D:");
			ImGui::Text("DrawCall: %d", stats.DrawCallCount);
			ImGui::Text("QuadCount: %d", stats.QuadCount);
			ImGui::Text("VertexCount: %d", stats.GetVertexCount());
			ImGui::Text("Index: %d", stats.GetIndexCount());

#ifdef OLD_CODE

			ImGui::DragFloat3("main_camera", glm::value_ptr(Camera_Entity.GetComponent<TransformComponent>().Transform[3]));

			if (ImGui::Checkbox("main_cam", &main_cam))
			{
				Camera_Entity.GetComponent<CameraComponent>().Primary = main_cam;
				Obj_Camera.GetComponent<CameraComponent>().Primary = !main_cam;
			}

			{
				auto& camera = Obj_Camera.GetComponent<CameraComponent>().Camera;
				float size = camera.GetOrthoGraphicSize();
				if (ImGui::DragFloat("Second cam size", &size))
				{
					camera.SetOrthoGraphicSize(size);
				}	
			}

#endif // OLD_CODE


			 
			//��ͼ����ʾ���������һ�������У�ImGui��API�����������Ի�
			/*ImGui::Begin("ViewPort");
			uint32_t textureID = FrameBuffer_2D->GetColorAttachment();
			ImGui::Image((void*)textureID, ImVec2(1280.0f, 720.0f), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();*/
			ImGui::End();

			//ͼ����ʾ��
			ImGui::Begin("ViewPort");

			//Gui�ӿڴ�С����
			ImVec2 ViewPortPanesize = ImGui::GetContentRegionAvail();
			if (m_ViewSize != *(glm::vec2*)&ViewPortPanesize && ViewPortPanesize.x > 0 && ViewPortPanesize.y > 0)
			{
				m_ViewSize = { ViewPortPanesize.x, ViewPortPanesize.y };
			}

			//����ѡ��
			m_ViewPortFocused = ImGui::IsWindowFocused();		//����Ƿ���ͣ�����������
			m_ViewPortHovered = ImGui::IsWindowHovered();		//�Ƿ�ѡ���������
			Application::GetInstence().GetImGuiLayer()->SetBlockEvent(!m_ViewPortFocused || !m_ViewPortHovered);

			uint32_t textureID = FrameBuffer_2D->GetColorAttachment();
			ImGui::Image((void*)textureID, ImVec2(m_ViewSize.x, m_ViewSize.y), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();

			ImGui::PopStyleVar();
		}
		ImGui::End();
	}

	void Editor_Layer::OnEvent(Event& event)
	{
		TZ_PROFILE_FUNCTION_SIG();
		CameraController.OnEvent(event);
	}
}


