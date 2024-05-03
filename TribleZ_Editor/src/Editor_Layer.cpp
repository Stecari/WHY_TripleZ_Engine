#include "tzpch.h"
#include "Editor_Layer.h"

#include "TribleZ_Core/Core.h"
#include "TribleZ_Core/Render/Texture.h"

#include "ImGui/imgui.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective(透视，投影关系、比例)
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
		//纹理加载
		Texture_2D = Texture2D::Create("asserts/img/LOGO/LOGO4.png");
		SpriteSheet_tex = Texture2D::Create("asserts/img/game/texture/tilemap_sheet.png");

		FrameBufferSpecification FrameBuffer_Spec;
		FrameBuffer_Spec.Width = 1280;
		FrameBuffer_Spec.Height = 720;
		FrameBuffer_2D = FrameBuffer::Create(FrameBuffer_Spec);

		//子纹理初始化
		subTex = SubTexture2D::CreateFromCoords(SpriteSheet_tex, { 10, 10 }, { 16.0, 16.0 }, { 1.0f, 1.0f });
		m_SubTexMap['D'] = SubTexture2D::CreateFromCoords(SpriteSheet_tex, { 1, 8 }, { 16.0, 16.0 }, { 1.0f, 1.0f });
		m_SubTexMap['G'] = SubTexture2D::CreateFromCoords(SpriteSheet_tex, { 0, 10 }, { 16.0, 16.0 }, { 1.0f, 1.0f });
		m_SubTexMap['F'] = SubTexture2D::CreateFromCoords(SpriteSheet_tex, { 2, 10 }, { 16.0, 16.0 }, { 1.0f, 1.0f });
		m_SubTexMap['R'] = SubTexture2D::CreateFromCoords(SpriteSheet_tex, { 7, 7 }, { 16.0, 16.0 }, { 1.0f, 1.0f });
		m_SubTexMap['W'] = SubTexture2D::CreateFromCoords(SpriteSheet_tex, { 0, 6 }, { 16.0, 16.0 }, { 1.0f, 1.0f });
		//m_SubTexMap['H'] = SubTexture2D::CreateFromCoords(SpriteSheet_tex, {  0, 3  }, { 16.0, 16.0 }, { 4.0f, 4.0f });

		//地图属性初始化
		Map_Width = s_MapWidth;
		Map_Height = strlen(TileMap) / s_MapWidth;

		//粒子初始化
		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f,  41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 35.0f, m_Particle.SizeVariation = 20.0f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 1.2f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 400.0f, 200.0f };
		m_Particle.Position = { 0.0f, 0.0f };

		//视口初始化
		CameraController.SetZoomLevel(720.0f * 2);

		//场景初始化
		ActiveScene = CreatRef<Scene>();
		
#if 0
		//初始化实体
		m_Square = ActiveScene->CreateEntity("Square");		//自带一个变换组件和Tag组件
		m_Square.GetComponent<TransformComponent>().Scale = glm::vec3{ 400.0f };		//默认是1.0的单位矩阵，但是假如默认是400，但由于相机实体和物体同时用用一个变换组件，渲染的时候变换的数值会被平方掉
		m_Square.AddComponent<SpriteRendererComponent>();

		m_second_Square = ActiveScene->CreateEntity("SecondSquare");
		m_second_Square.GetComponent<TransformComponent>().Scale = glm::vec3{ 400.0f };
		m_second_Square.AddComponent<SpriteRendererComponent>(glm::vec4{0.8f, 0.3f, 0.2f, 1.0f });

		Camera_Entity = ActiveScene->CreateEntity("Main_Camera");;
		Camera_Entity.AddComponent<CameraComponent>();

		Obj_Camera = ActiveScene->CreateEntity("Object_Camera");;
		Obj_Camera.AddComponent<CameraComponent>();
		Obj_Camera.GetComponent<CameraComponent>().Primary = false;


		//极其恶心的一个类，写在这干嘛？
		class CameraControllerEntity : public ScriptableEntity		//一个 行为类
		{
		public:
			virtual void OnCreate()	override	//这个空返回值函数倒底再Create什么？？？
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
					Camera_pos_y -= 400.0f * timestep;	//隐式转换后这里写的是float(timestep)
				}
				if (Input::IsKeyPressed(TZ_KEY_A))
				{
					Camera_pos_x -= 400.0f * timestep;//相当于是timestep.GetSecond()
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

		//画面分层面板初始化
		m_ScenePanel.SetContext(ActiveScene);
	}

	void Editor_Layer::OnDetach()
	{

	}

	void Editor_Layer::OnUpdata(TimeStep time_step)
	{
		TZ_PROFILE_FUNCTION();	//计时器，基准测试


		//不知道什么时候加的resizing
		if (TribleZ::FrameBufferSpecification t_spec = FrameBuffer_2D->GetSpecification();	//if语句判断中可以采用一个分号，分号前是我判断前要进行地操作语句
			m_ViewSize.x > 0.0f && m_ViewSize.y > 0.0f &&	//我Gui视口(窗口)的大小要大于0
			t_spec.Width != m_ViewSize.x || t_spec.Height != m_ViewSize.y)		//视口大小不等于帧缓冲区大小
		{
			FrameBuffer_2D->Resize((uint32_t)m_ViewSize.x, (uint32_t)m_ViewSize.y);
			CameraController.ResizeView(m_ViewSize.x, m_ViewSize.y);

			ActiveScene->ResizeView((uint32_t)m_ViewSize.x, (uint32_t)m_ViewSize.y);
		}

		//更新相机
		if (m_ViewPortFocused){		//只有选中该窗口时才能启用相机更新
			CameraController.OnUpdata(time_step);
		}

		//更行统计数据
		Renderer2D::ResetStats();

		

		//渲染
		static float rotation = 20.0f;
		rotation += 20.0f * time_step;

		FrameBuffer_2D->Bind();
		{
			TZ_PROFILE_SCOPE("RENDERER");
			RendererCommand::SetClearColor({ 0.3f, 0.3f, 0.3f, 1.0f });
			RendererCommand::Clear();

			//Renderer2D::SceneBegin(CameraController.GetCamera());
			//更新场景
			ActiveScene->OnUpdata(time_step);

			//Renderer2D::SceneEnd();

			/*这里有一个很莫名其妙的bug，假如后渲染图形是放在下层的，在它上面的图形的透明度会失效*/
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
			//粒子系统：点击鼠标发射粒子
			if (Input::IsMouseButtonPressed(TZ_MOUSE_BUTTON_LEFT))
			{
				auto [x, y] = Input::GetMousePosition();
				auto width = Application::GetInstence().GetWindow().GetWidth();
				auto height = Application::GetInstence().GetWindow().GetHeight();

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
	void Editor_Layer::OnImGuiRender()
	{
		TZ_PROFILE_FUNCTION_SIG();
		auto stats = Renderer2D::GetStats();
		static bool docking_Enabled = true;
		//static bool opt_fullscreen_persisteand = true;		//persistand  持续命令
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
		*当使用ImGuiDockNodeFlags_PasshruCentralNode时，
		*DockSpace()将渲染我们的背景,并且处理通过孔，所以我们要求Begin()不要渲染背景。
		*/
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		/*
		*重要提示：请注意，即使Begin()返回false（即窗口被折叠），我们也会继续。
		*这是因为我们希望保持DockSpace()处于活动状态。如果DockSpace()处于非活动状态，
		*所有停靠到其中的活动窗口都将失去其父窗口并变为未停靠窗口。
		*我们不能保留活动窗口和非活动停靠之间的停靠关系，否则
		*dockspace/settings的任何更改都会导致窗口陷入困境，永远看不见。
		*/
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &docking_Enabled, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)		ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		/*----------------设置最小窗口宽度-------------------------------*/
		ImGuiStyle& style = ImGui::GetStyle();
		float ori_minWinSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 300.0f;
		/*----------------设置最小窗口宽度-------------------------------*/
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else {
			TZ_CORE_ERROR("Docking Disabled!!");
		}
		/*----------------重置最小窗口宽度-------------------------------*/
		style.WindowMinSize.x = ori_minWinSize;
		/*----------------重置最小窗口宽度-------------------------------*/



		//顶上那个选项栏
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
					* 这里还是有问题Bug
					* 我们每一次读出数据的时候，并没有创建一个删掉原来的Scene新的Scene
					* 这就会导致每一次读入都导入一个重复的Scene在一个Scene上面叠加好几层
					*/
					SceneSerializer TZ_Serializer(ActiveScene);
					TZ_Serializer.Deserializer("asserts/scenes/excmple.tz");
				}

				if (ImGui::MenuItem("Exit")) { Application::GetInstence().Close(); }

				ImGui::Separator();

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();


			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));		//边框填充

			//显示分层面板
			m_ScenePanel.OnImGuiRender();

			//数据和编辑
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


			 
			//把图像显示层放在这里一样能运行，ImGui的API做的真是人性化
			/*ImGui::Begin("ViewPort");
			uint32_t textureID = FrameBuffer_2D->GetColorAttachment();
			ImGui::Image((void*)textureID, ImVec2(1280.0f, 720.0f), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();*/
			ImGui::End();

			//图像显示层
			ImGui::Begin("ViewPort");

			//Gui视口大小调整
			ImVec2 ViewPortPanesize = ImGui::GetContentRegionAvail();
			if (m_ViewSize != *(glm::vec2*)&ViewPortPanesize && ViewPortPanesize.x > 0 && ViewPortPanesize.y > 0)
			{
				m_ViewSize = { ViewPortPanesize.x, ViewPortPanesize.y };
			}

			//窗口选中
			m_ViewPortFocused = ImGui::IsWindowFocused();		//鼠标是否悬停在这个窗口上
			m_ViewPortHovered = ImGui::IsWindowHovered();		//是否选中这个窗口
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


