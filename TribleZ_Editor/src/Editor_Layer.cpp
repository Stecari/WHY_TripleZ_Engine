#include "tzpch.h"
#include "Editor_Layer.h"

#include "TribleZ_Core/Core.h"
#include "TribleZ_Core/Render/Texture.h"

#include "ImGui/imgui.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective(透视，投影关系、比例)
#include <glm/gtc/type_ptr.hpp>

#include "Tool/BenchMarking.h"

#include "Scene/SceneSerializer.h"
#include "Utils/PlatformUtils.h"

#include "ImGuizmo/ImGuizmo.h"
#include "TribleZMath/TribleZMath.h"

#define PARTICLE_SYSTEM_ON 0

namespace TribleZ
{
	extern const std::filesystem::path g_AssertPath;

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

		m_IconPlay = Texture2D::Create("Resources/Icons/Player/PalyBottom.png");
		m_IconStop = Texture2D::Create("Resources/Icons/Player/StopBottom.png");

		FrameBufferSpecification FrameBuffer_Spec;
		FrameBuffer_Spec.Attachments = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER ,FramebufferTextureFormat::Depth };
		FrameBuffer_Spec.Width = 1280;
		FrameBuffer_Spec.Height = 720;
		FrameBuffer_2D = FrameBuffer::Create(FrameBuffer_Spec);//人身第一次独立修好疑难杂症
													//开Release的时候帧缓冲区不完整，m_Specification的

		{
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
		}

		//视口初始化
		CameraController.SetZoomLevel(720.0f * 2);

		//场景初始化
		ActiveScene = CreatRef<Scene>();

		//命令行 ??
		auto commondLineArgs = Application::GetInstence().GetCommandLineArgs();
		if (commondLineArgs.Count > 1)
		{
			auto scenefilePath = commondLineArgs[1];
			SceneSerializer serializer(ActiveScene);
			serializer.Deserializer(scenefilePath);
		}

		m_EditorCamera = Editor_Camera(60.0f, 1.788f, 0.01f, 10000.0f);
		
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
		m_SceneHierarchyPanel.SetContext(ActiveScene);
	}

	void Editor_Layer::OnDetach()
	{

	}

	void Editor_Layer::OnUpdata(TimeStep time_step)
	{
		TZ_PROFILE_FUNCTION();	//计时器，基准测试


		//设置窗口尺寸—视口尺寸
		if (TribleZ::FrameBufferSpecification t_spec = FrameBuffer_2D->GetSpecification();	//if语句判断中可以采用一个分号，分号前是我判断前要进行地操作语句
			m_ViewSize.x > 0.0f && m_ViewSize.y > 0.0f &&	//我Gui视口(窗口)的大小要大于0
			(t_spec.Width != m_ViewSize.x || t_spec.Height != m_ViewSize.y) )		//视口大小不等于帧缓冲区大小
		{
			FrameBuffer_2D->Resize((uint32_t)m_ViewSize.x, (uint32_t)m_ViewSize.y);
			CameraController.ResizeView(m_ViewSize.x, m_ViewSize.y);
			m_EditorCamera.SetViewportSize(m_ViewSize.x, m_ViewSize.y);
			ActiveScene->ResizeView((uint32_t)m_ViewSize.x, (uint32_t)m_ViewSize.y);
		}
		

		////更新相机
		//if (m_ViewPortFocused){		//只有选中该窗口时才能启用相机更新
		//	CameraController.OnUpdata(time_step);
		//}
		//m_EditorCamera.OnUpdata(time_step);

		//更行统计数据
		Renderer2D::ResetStats();

		

		//渲染
		static float rotation = 20.0f;
		rotation += 20.0f * time_step;

		FrameBuffer_2D->Bind();
		RendererCommand::SetClearColor({ 0.3f, 0.3f, 0.3f, 1.0f });
		RendererCommand::Clear();
		FrameBuffer_2D->ClearAttachment(1, -1);


		//更新场景
		switch (m_SceneState)
		{
			case SceneState::Edit: 
			{
				//更新相机
				if (m_ViewPortFocused) {		//只有选中该窗口时才能启用相机更新
					CameraController.OnUpdata(time_step);
				}
				m_EditorCamera.OnUpdata(time_step);

				ActiveScene->OnUpdataEditor(time_step, m_EditorCamera);
				break;
			}
			case SceneState::Play:
			{
				ActiveScene->OnUpdataRuntime(time_step);
				break;
			}
		}


		//					/*全局坐标，左上角是0，0，一整块电脑屏幕作为载体*/
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBound[0].x;
		my -= m_ViewportBound[0].y;
		/*换算成局部坐标，左下角是0，0，视口作为载体*/
		//glm::vec2 viewportSize = m_ViewportBound[1] - m_ViewportBound[0];		//感觉没必要，就当是展示一下好了
		int MouseX = (int)mx;
		int MouseY = (int)(m_ViewSize.y - my);
		//TZ_CORE_INFO("Pixcel: {0} , {1}", MouseX, MouseY);

		if (MouseX > 0 && MouseX < m_ViewSize.x && MouseY > 0 && MouseY < m_ViewSize.y)
		{
			int Pixcel = FrameBuffer_2D->ReadPixel(1, MouseX, MouseY);
			if (Pixcel == -1) {
				m_HoverdEntity = {};
			}
			else{
				m_HoverdEntity = { (entt::entity)Pixcel, ActiveScene.get() };
				//TZ_CORE_INFO("Pixcel: {0}", Pixcel);
			}
		}
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
				if (ImGui::MenuItem("New", "Ctrl+N"))	{
					NewScene();
				}
				if (ImGui::MenuItem("Open...", "Ctrl+O")){																													/*  显示的提示信息   */
					OpenScene();
				}
				if (ImGui::MenuItem("Save As ...", "Shift+Ctrl+S")){
					SaveSceneAs();
				}

				if (ImGui::MenuItem("Exit")) { Application::GetInstence().Close(); }

				ImGui::Separator();

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();


			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));		//边框填充

			//显示分层面板
			m_SceneHierarchyPanel.OnImGuiRender();
			m_ContentBrowserPanels.OnImGuiRender();

			//鼠标悬停的实体ID
			std::string hoverd_name = "NONE";
			if (m_HoverdEntity){
				hoverd_name = m_HoverdEntity.GetComponent<TagComponent>().Tag;
			}


			//数据和编辑
			ImGui::Begin("Editor_Layer");

			ImGui::Text("Hoverd Entity: %s", hoverd_name.c_str());	//显示悬停的实体

			ImGui::Text("Renderer2D:");
			ImGui::Text("DrawCall: %d", stats.DrawCallCount);
			ImGui::Text("QuadCount: %d", stats.QuadCount);
			ImGui::Text("VertexCount: %d", stats.GetVertexCount());
			ImGui::Text("Index: %d", stats.GetIndexCount());

			 
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

			//获取光标在全局中的位置，全局就是对于整块显示器，考虑标题栏下拉式光标下移，右上角为0，0
			//ImVec2 viewportOffset = ImGui::GetCursorPos();
			//下一集新加的，之前的假如鼠标送到一些奇怪的地方读到的像素点时随机数，会导致识别报错
			ImVec2 viewportMaxReigion = ImGui::GetWindowContentRegionMax();
			ImVec2 viewportMinReigion = ImGui::GetWindowContentRegionMin();
			ImVec2 viewportOffset = ImGui::GetWindowPos();

			m_ViewportBound[0] = { viewportOffset.x + viewportMinReigion.x, viewportOffset.y + viewportMinReigion.y };
			m_ViewportBound[1] = { viewportOffset.x + viewportMaxReigion.x, viewportOffset.y + viewportMaxReigion.y };
			//TZ_CORE_TRACE("m_ViewportBound 2 :{0}, {1}", m_ViewportBound[0].x, m_ViewportBound[0].y);
			//TZ_CORE_ERROR("m_ViewportBound 1 :{0}, {1}", m_ViewportBound[1].x, m_ViewportBound[1].y);




			//窗口选中
			m_ViewPortFocused = ImGui::IsWindowFocused();		//鼠标是否悬停在这个窗口上
			m_ViewPortHovered = ImGui::IsWindowHovered();		//是否选中这个窗口
			Application::GetInstence().GetImGuiLayer()->SetBlockEvent(!m_ViewPortFocused && !m_ViewPortHovered);

			//怪恶心的，还要加个索引，默认零
			uint64_t textureID = FrameBuffer_2D->GetColorAttachmentRendererID();
			//ImGui::Image((void*)textureID, ImVec2(m_ViewSize.x, m_ViewSize.y), ImVec2(0, 1), ImVec2(1, 0));
			//更安全的版本，他没讲，看的时候看到的
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2(m_ViewSize.x, m_ViewSize.y), ImVec2(0, 1), ImVec2(1, 0));

			/*-------------窗口鼠标信息-----------后面有点问题，改完放到上面viewportOffset旁边去了----------------------------------------------------*/
			//                      /*全局坐标，左上角是0，0，一整块电脑屏幕作为载体*/
			//ImVec2 windowSize = ImGui::GetWindowSize();
			//ImVec2 minBound = ImGui::GetWindowPos();
			//minBound.x += viewportOffset.x;
			//minBound.y += viewportOffset.y;
			////TZ_CORE_INFO("Pixcel: {0} , {1}", minBound.x, minBound.y);
			//ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
			//m_ViewportBound[0] = { minBound.x, minBound.y };
			//m_ViewportBound[1] = { maxBound.x, maxBound.y };		
			/*-------------窗口鼠标信息---------------------------------------------------------------------------------------------------------------*/

			/*-------------文件拖拽---------------------------------------------------------------------------------------------------------------*/
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content_Browers_panel"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					OpenScene(g_AssertPath / path);
				}
				ImGui::EndDragDropTarget();
			}
			/*-------------文件拖拽---------------------------------------------------------------------------------------------------------------*/



			//ImGuizmo	ImGui小组件
			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();	//这个功能有点不合理，因为理论上我们要的是根据鼠标点击的地方来将那个实体赋给这个，但暂时我们先这样
			if (selectedEntity && m_GuizmoOperationType != -1)
			{
				ImGuizmo::SetOrthographic(false);		//暂时关闭正交模式
				ImGuizmo::SetDrawlist();				//看描述这个就是一个格式化的东西？

				//float windowWidth = ImGui::GetWindowWidth();
				//float windowHeight = ImGui::GetWindowHeight();
				//ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);		//SetRectangle Rectangle:矩形
				/*后期把范围缩小了*/
				ImGuizmo::SetRect(m_ViewportBound[0].x, m_ViewportBound[0].y, m_ViewportBound[1].x - m_ViewportBound[0].x, m_ViewportBound[1].y - m_ViewportBound[0].y);	

				//获取主相机参数
#ifdef RUNTIME
				Entity cameraEntity = ActiveScene->GetPrimaryCameraEntity();
				const auto& PrimaryCamera = cameraEntity.GetComponent<CameraComponent>().Camera;
				const glm::mat4& CameraProjection = PrimaryCamera.GetProjection();
				glm::mat4 CameraView = inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());
#else
				const glm::mat4& CameraProjection = m_EditorCamera.GetProjection();
				glm::mat4 CameraView = m_EditorCamera.GetViewMatrix();
#endif
				//获取实体信息
				auto& SelectedEntity_TC = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 SelectEntitytransform = SelectedEntity_TC.GetTransform();

				//snap  这个好像是跳动式变化
				bool snap = Input::IsKeyPressed(TZ_KEY_LEFT_CONTROL);
				float snapValue = 50.0f;
				if (m_GuizmoOperationType == ImGuizmo::OPERATION::ROTATE) {
					snapValue = 15.0f;
				}
				if (m_GuizmoOperationType == ImGuizmo::OPERATION::SCALE) {
					snapValue = 0.2f;
				}
				float SnapValues[3] = { snapValue , snapValue , snapValue };

				/*
				* 组件操作 函数，通过ImGuizmo::OPERATION::参数切换平移旋转缩放还有一种不知道的操作模式
				* 不能在同一帧中以不同的 操作模式 多次调用这个函数
				* 不然的话，比如先调用了平移和缩放
				* 那么当你平移时，平移的操作函数读到正常的数据，但是缩放的函数拿到的是平移的数据
				* 就会指针出错
				* 所以只能在渲染过程中切换模式
				*/
				ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(CameraProjection), 
					(ImGuizmo::OPERATION)m_GuizmoOperationType, ImGuizmo::LOCAL, glm::value_ptr(SelectEntitytransform), nullptr, snap? SnapValues : nullptr);

				if (ImGuizmo::IsUsing())		//给我们一个判断是否正在使用ImGuizmo组件的方法，如果正在使用组件就不能点击选择实体，不然在拖动的一瞬间会切换选中实体
				{									//另一个需要的函数时IsOver()返回是否在空间上方停留，假如真的要写选中就得用IsOver()
#ifdef STATIC_EULER_POINT
					/*
					* 假如直接使用也是可以的，但这个涉及到一个静态欧拉角还是动态欧拉角的问题
					* 直接使用就是忽略gimbal lock的影响直接使用静态欧拉角
					* 那么旋转的坐标轴就是固定的，值也将被限制在±180°内。	后面发现不是这个原因，非常好我现在也不知道是什么原因了
					* 但实际上我们希望的是坐标轴随之转动的动态欧拉角，也就是理论上的转动在±无限上
					* 所以这里cherno采用将rotation的绝对值改变改为delta值(变化值)
					*/
					glm::vec3 translation, rotation, scale;
					Math::Decomposed(SelectEntitytransform, translation, rotation, scale);
					SelectedEntity_TC.Translation = translation;
					SelectedEntity_TC.Rotation = rotation;
					SelectedEntity_TC.Scale = scale;
#endif // STATIC_EULER_POINT
					glm::vec3 translation, rotation, scale;
					Math::Decomposed(SelectEntitytransform, translation, rotation, scale);

					glm::vec3 delta_rotation = rotation - SelectedEntity_TC.Rotation;

					SelectedEntity_TC.Translation = translation;
					SelectedEntity_TC.Rotation += delta_rotation;
					SelectedEntity_TC.Scale = scale;
					
				}
			}
			ImGui::End();

			ImGui::PopStyleVar();

		}
		UI_Toolbar();

		ImGui::End();
	}

	void Editor_Layer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& BottomHovCol = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(BottomHovCol.x, BottomHovCol.y, BottomHovCol.z, 0.7f));
		const auto& BottomActCol = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(BottomActCol.x, BottomActCol.y, BottomActCol.z, 0.7f));


		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		float size = ImGui::GetWindowHeight() - 6.0f;
		//ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.5 - size * 0.5);
		ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.5 - size * 0.5);
		Ref<Texture2D> Icon = m_SceneState == SceneState::Edit ? m_IconPlay : m_IconStop;
		if (ImGui::ImageButton((ImTextureID)Icon->GetID(), ImVec2( size, size )))
		{
			if (m_SceneState == SceneState::Edit){
				OnScenePlay();
			}
			else if (m_SceneState == SceneState::Play){
				OnSceneEdit();
			}
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void Editor_Layer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;
	}

	void Editor_Layer::OnSceneEdit()
	{
		m_SceneState = SceneState::Edit;
	}

	void Editor_Layer::OnEvent(Event& event)
	{
		TZ_PROFILE_FUNCTION_SIG();
		CameraController.OnEvent(event);
		m_EditorCamera.OnEvent(event);
		//TZ_CORE_INFO("IsOver : {0}", ImGuizmo::IsOver());
		EventDispatcher dispatcher(event);
		dispatcher.DisPatcher<EventKeyPress>(TZ_CORE_BIND_EVENT_Fn(Editor_Layer::OnEventKeyPressed));
		dispatcher.DisPatcher<EventMouseButtonPressed>(TZ_CORE_BIND_EVENT_Fn(Editor_Layer::OnEventMouseButtonPressed));
	}

	bool Editor_Layer::OnEventKeyPressed(EventKeyPress& event)
	{
		if (event.GetRepeatCount() > 0) {	//防止长按，长按的话只有第一次有用
			return false;
		}

		bool Ctrl = Input::IsKeyPressed(TZ_KEY_LEFT_CONTROL) || Input::IsKeyPressed(TZ_KEY_RIGHT_CONTROL);
		bool Shift = Input::IsKeyPressed(TZ_KEY_LEFT_SHIFT) || Input::IsKeyPressed(TZ_KEY_RIGHT_SHIFT);

		switch (event.GetKeyCode())
		{
			//file dialogs
			case TZ_KEY_O:
			{
				if (Ctrl) { OpenScene(); }
				break;
			}
			case TZ_KEY_N:
			{
				if (Ctrl) { NewScene(); }
				break;
			}
			case TZ_KEY_S:
			{
				if (Shift && Ctrl) { SaveSceneAs(); }
				break;
			}
			//Guizmo
			case TZ_KEY_Q:
			{
				m_GuizmoOperationType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case TZ_KEY_W:
			{
				m_GuizmoOperationType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case TZ_KEY_E:
			{
				m_GuizmoOperationType = ImGuizmo::OPERATION::SCALE;
				break;
			}
			case TZ_KEY_R:
			{
				m_GuizmoOperationType = -1;
				break;
			}
		}
	}
	bool Editor_Layer::OnEventMouseButtonPressed(EventMouseButtonPressed& event)
	{
														/*鼠标是否在ImGuizmo控件的上方，假如在上方就说明你想要使用控件，就不能选中实体*/
			if (event.GetMouseButton() == TZ_MOUSE_BUTTON_1 && !ImGuizmo::IsOver() && !Input::IsKeyPressed(TZ_KEY_LEFT_ALT) ) {
				if (m_ViewPortHovered) {
					m_SceneHierarchyPanel.SetSelectionEntity(m_HoverdEntity);
				}
			}
		return false;
	}
	void Editor_Layer::SaveSceneAs()
	{
		std::string file_path = FileDialogs::SaveFile("TribleZ Scene (*.tz)\0*.tz\0");
		if (!file_path.empty())
		{
			SceneSerializer TZ_Serializer(ActiveScene);
			TZ_Serializer.Serializer(file_path);
		}
	}
	void Editor_Layer::OpenScene()
	{
		std::string file_path = FileDialogs::OpenFile("TribleZ Scene (*.tz)\0*.tz\0");	//这个就是自定义文件过滤器的格式 TribleZ Scene (*.tz) \0*.tz\0		比如这里就会将所有(*)以.tz为后缀的文件显示出来
		if (!file_path.empty())	//路径不为空，说明成功进行了															/*  被两个 \0 夹起来的部分就是真正的过滤器*/
		{
			OpenScene(file_path);
		}
	}
	void Editor_Layer::OpenScene(const std::filesystem::path& path)
	{
		ActiveScene = CreatRef<Scene>();	//创建一个新的场景
		ActiveScene->ResizeView((uint32_t)m_ViewSize.x, (uint32_t)m_ViewSize.y);	//初始化视口大小防止相机缩放比出问题
		m_SceneHierarchyPanel.SetContext(ActiveScene);	//这里本来里面没有指针刷新，所以会出问题

		SceneSerializer TZ_Serializer(ActiveScene);
		TZ_Serializer.Deserializer(path.string());
	}
	void Editor_Layer::NewScene()
	{
#if 0
		std::string file_path = FileDialogs::SaveFile("TribleZ Scene (*.tz)\0*.tz\0");
		if (!file_path.empty())	//路径不为空，说明成功进行了															/*  被两个 \0 夹起来的部分就是真正的过滤器*/
		{
			ActiveScene = CreatRef<Scene>();	//创建一个新的场景
			ActiveScene->ResizeView((uint32_t)m_ViewSize.x, (uint32_t)m_ViewSize.y);	//初始化视口大小防止相机缩放比出问题
			m_SceneHierarchyPanel.SetContext(ActiveScene);

			SceneSerializer TZ_Serializer(ActiveScene);
			TZ_Serializer.Serializer(file_path);
		}
#endif
		ActiveScene = CreatRef<Scene>();
		ActiveScene->ResizeView((uint32_t)m_ViewSize.x, (uint32_t)m_ViewSize.y);
		m_SceneHierarchyPanel.SetContext(ActiveScene);
	}
}


