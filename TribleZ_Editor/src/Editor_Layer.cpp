#include "tzpch.h"
#include "Editor_Layer.h"

#include "TribleZ_Core/Core.h"
#include "TribleZ_Core/Render/Texture.h"

#include "ImGui/imgui.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective(͸�ӣ�ͶӰ��ϵ������)
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
		//�������
		Texture_2D = Texture2D::Create("asserts/img/LOGO/LOGO4.png");
		SpriteSheet_tex = Texture2D::Create("asserts/img/game/texture/tilemap_sheet.png");

		m_IconPlay = Texture2D::Create("Resources/Icons/Player/PalyBottom.png");
		m_IconStop = Texture2D::Create("Resources/Icons/Player/StopBottom.png");

		FrameBufferSpecification FrameBuffer_Spec;
		FrameBuffer_Spec.Attachments = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER ,FramebufferTextureFormat::Depth };
		FrameBuffer_Spec.Width = 1280;
		FrameBuffer_Spec.Height = 720;
		FrameBuffer_2D = FrameBuffer::Create(FrameBuffer_Spec);//�����һ�ζ����޺�������֢
													//��Release��ʱ��֡��������������m_Specification��

		{
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
		}

		//�ӿڳ�ʼ��
		CameraController.SetZoomLevel(720.0f * 2);

		//������ʼ��
		ActiveScene = CreatRef<Scene>();

		//������ ??
		auto commondLineArgs = Application::GetInstence().GetCommandLineArgs();
		if (commondLineArgs.Count > 1)
		{
			auto scenefilePath = commondLineArgs[1];
			SceneSerializer serializer(ActiveScene);
			serializer.Deserializer(scenefilePath);
		}

		m_EditorCamera = Editor_Camera(60.0f, 1.788f, 0.01f, 10000.0f);
		
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
		m_SceneHierarchyPanel.SetContext(ActiveScene);
	}

	void Editor_Layer::OnDetach()
	{

	}

	void Editor_Layer::OnUpdata(TimeStep time_step)
	{
		TZ_PROFILE_FUNCTION();	//��ʱ������׼����


		//���ô��ڳߴ硪�ӿڳߴ�
		if (TribleZ::FrameBufferSpecification t_spec = FrameBuffer_2D->GetSpecification();	//if����ж��п��Բ���һ���ֺţ��ֺ�ǰ�����ж�ǰҪ���еز������
			m_ViewSize.x > 0.0f && m_ViewSize.y > 0.0f &&	//��Gui�ӿ�(����)�Ĵ�СҪ����0
			(t_spec.Width != m_ViewSize.x || t_spec.Height != m_ViewSize.y) )		//�ӿڴ�С������֡��������С
		{
			FrameBuffer_2D->Resize((uint32_t)m_ViewSize.x, (uint32_t)m_ViewSize.y);
			CameraController.ResizeView(m_ViewSize.x, m_ViewSize.y);
			m_EditorCamera.SetViewportSize(m_ViewSize.x, m_ViewSize.y);
			ActiveScene->ResizeView((uint32_t)m_ViewSize.x, (uint32_t)m_ViewSize.y);
		}
		

		////�������
		//if (m_ViewPortFocused){		//ֻ��ѡ�иô���ʱ���������������
		//	CameraController.OnUpdata(time_step);
		//}
		//m_EditorCamera.OnUpdata(time_step);

		//����ͳ������
		Renderer2D::ResetStats();

		

		//��Ⱦ
		static float rotation = 20.0f;
		rotation += 20.0f * time_step;

		FrameBuffer_2D->Bind();
		RendererCommand::SetClearColor({ 0.3f, 0.3f, 0.3f, 1.0f });
		RendererCommand::Clear();
		FrameBuffer_2D->ClearAttachment(1, -1);


		//���³���
		switch (m_SceneState)
		{
			case SceneState::Edit: 
			{
				//�������
				if (m_ViewPortFocused) {		//ֻ��ѡ�иô���ʱ���������������
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


		//					/*ȫ�����꣬���Ͻ���0��0��һ���������Ļ��Ϊ����*/
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBound[0].x;
		my -= m_ViewportBound[0].y;
		/*����ɾֲ����꣬���½���0��0���ӿ���Ϊ����*/
		//glm::vec2 viewportSize = m_ViewportBound[1] - m_ViewportBound[0];		//�о�û��Ҫ���͵���չʾһ�º���
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
				if (ImGui::MenuItem("New", "Ctrl+N"))	{
					NewScene();
				}
				if (ImGui::MenuItem("Open...", "Ctrl+O")){																													/*  ��ʾ����ʾ��Ϣ   */
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


			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));		//�߿����

			//��ʾ�ֲ����
			m_SceneHierarchyPanel.OnImGuiRender();
			m_ContentBrowserPanels.OnImGuiRender();

			//�����ͣ��ʵ��ID
			std::string hoverd_name = "NONE";
			if (m_HoverdEntity){
				hoverd_name = m_HoverdEntity.GetComponent<TagComponent>().Tag;
			}


			//���ݺͱ༭
			ImGui::Begin("Editor_Layer");

			ImGui::Text("Hoverd Entity: %s", hoverd_name.c_str());	//��ʾ��ͣ��ʵ��

			ImGui::Text("Renderer2D:");
			ImGui::Text("DrawCall: %d", stats.DrawCallCount);
			ImGui::Text("QuadCount: %d", stats.QuadCount);
			ImGui::Text("VertexCount: %d", stats.GetVertexCount());
			ImGui::Text("Index: %d", stats.GetIndexCount());

			 
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

			//��ȡ�����ȫ���е�λ�ã�ȫ�־��Ƕ���������ʾ�������Ǳ���������ʽ������ƣ����Ͻ�Ϊ0��0
			//ImVec2 viewportOffset = ImGui::GetCursorPos();
			//��һ���¼ӵģ�֮ǰ�ļ�������͵�һЩ��ֵĵط����������ص�ʱ��������ᵼ��ʶ�𱨴�
			ImVec2 viewportMaxReigion = ImGui::GetWindowContentRegionMax();
			ImVec2 viewportMinReigion = ImGui::GetWindowContentRegionMin();
			ImVec2 viewportOffset = ImGui::GetWindowPos();

			m_ViewportBound[0] = { viewportOffset.x + viewportMinReigion.x, viewportOffset.y + viewportMinReigion.y };
			m_ViewportBound[1] = { viewportOffset.x + viewportMaxReigion.x, viewportOffset.y + viewportMaxReigion.y };
			//TZ_CORE_TRACE("m_ViewportBound 2 :{0}, {1}", m_ViewportBound[0].x, m_ViewportBound[0].y);
			//TZ_CORE_ERROR("m_ViewportBound 1 :{0}, {1}", m_ViewportBound[1].x, m_ViewportBound[1].y);




			//����ѡ��
			m_ViewPortFocused = ImGui::IsWindowFocused();		//����Ƿ���ͣ�����������
			m_ViewPortHovered = ImGui::IsWindowHovered();		//�Ƿ�ѡ���������
			Application::GetInstence().GetImGuiLayer()->SetBlockEvent(!m_ViewPortFocused && !m_ViewPortHovered);

			//�ֶ��ĵģ���Ҫ�Ӹ�������Ĭ����
			uint64_t textureID = FrameBuffer_2D->GetColorAttachmentRendererID();
			//ImGui::Image((void*)textureID, ImVec2(m_ViewSize.x, m_ViewSize.y), ImVec2(0, 1), ImVec2(1, 0));
			//����ȫ�İ汾����û��������ʱ�򿴵���
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2(m_ViewSize.x, m_ViewSize.y), ImVec2(0, 1), ImVec2(1, 0));

			/*-------------���������Ϣ-----------�����е����⣬����ŵ�����viewportOffset�Ա�ȥ��----------------------------------------------------*/
			//                      /*ȫ�����꣬���Ͻ���0��0��һ���������Ļ��Ϊ����*/
			//ImVec2 windowSize = ImGui::GetWindowSize();
			//ImVec2 minBound = ImGui::GetWindowPos();
			//minBound.x += viewportOffset.x;
			//minBound.y += viewportOffset.y;
			////TZ_CORE_INFO("Pixcel: {0} , {1}", minBound.x, minBound.y);
			//ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
			//m_ViewportBound[0] = { minBound.x, minBound.y };
			//m_ViewportBound[1] = { maxBound.x, maxBound.y };		
			/*-------------���������Ϣ---------------------------------------------------------------------------------------------------------------*/

			/*-------------�ļ���ק---------------------------------------------------------------------------------------------------------------*/
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content_Browers_panel"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					OpenScene(g_AssertPath / path);
				}
				ImGui::EndDragDropTarget();
			}
			/*-------------�ļ���ק---------------------------------------------------------------------------------------------------------------*/



			//ImGuizmo	ImGuiС���
			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();	//��������е㲻������Ϊ����������Ҫ���Ǹ���������ĵط������Ǹ�ʵ�帳�����������ʱ����������
			if (selectedEntity && m_GuizmoOperationType != -1)
			{
				ImGuizmo::SetOrthographic(false);		//��ʱ�ر�����ģʽ
				ImGuizmo::SetDrawlist();				//�������������һ����ʽ���Ķ�����

				//float windowWidth = ImGui::GetWindowWidth();
				//float windowHeight = ImGui::GetWindowHeight();
				//ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);		//SetRectangle Rectangle:����
				/*���ڰѷ�Χ��С��*/
				ImGuizmo::SetRect(m_ViewportBound[0].x, m_ViewportBound[0].y, m_ViewportBound[1].x - m_ViewportBound[0].x, m_ViewportBound[1].y - m_ViewportBound[0].y);	

				//��ȡ���������
#ifdef RUNTIME
				Entity cameraEntity = ActiveScene->GetPrimaryCameraEntity();
				const auto& PrimaryCamera = cameraEntity.GetComponent<CameraComponent>().Camera;
				const glm::mat4& CameraProjection = PrimaryCamera.GetProjection();
				glm::mat4 CameraView = inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());
#else
				const glm::mat4& CameraProjection = m_EditorCamera.GetProjection();
				glm::mat4 CameraView = m_EditorCamera.GetViewMatrix();
#endif
				//��ȡʵ����Ϣ
				auto& SelectedEntity_TC = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 SelectEntitytransform = SelectedEntity_TC.GetTransform();

				//snap  �������������ʽ�仯
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
				* ������� ������ͨ��ImGuizmo::OPERATION::�����л�ƽ����ת���Ż���һ�ֲ�֪���Ĳ���ģʽ
				* ������ͬһ֡���Բ�ͬ�� ����ģʽ ��ε����������
				* ��Ȼ�Ļ��������ȵ�����ƽ�ƺ�����
				* ��ô����ƽ��ʱ��ƽ�ƵĲ��������������������ݣ��������ŵĺ����õ�����ƽ�Ƶ�����
				* �ͻ�ָ�����
				* ����ֻ������Ⱦ�������л�ģʽ
				*/
				ImGuizmo::Manipulate(glm::value_ptr(CameraView), glm::value_ptr(CameraProjection), 
					(ImGuizmo::OPERATION)m_GuizmoOperationType, ImGuizmo::LOCAL, glm::value_ptr(SelectEntitytransform), nullptr, snap? SnapValues : nullptr);

				if (ImGuizmo::IsUsing())		//������һ���ж��Ƿ�����ʹ��ImGuizmo����ķ������������ʹ������Ͳ��ܵ��ѡ��ʵ�壬��Ȼ���϶���һ˲����л�ѡ��ʵ��
				{									//��һ����Ҫ�ĺ���ʱIsOver()�����Ƿ��ڿռ��Ϸ�ͣ�����������Ҫдѡ�о͵���IsOver()
#ifdef STATIC_EULER_POINT
					/*
					* ����ֱ��ʹ��Ҳ�ǿ��Եģ�������漰��һ����̬ŷ���ǻ��Ƕ�̬ŷ���ǵ�����
					* ֱ��ʹ�þ��Ǻ���gimbal lock��Ӱ��ֱ��ʹ�þ�̬ŷ����
					* ��ô��ת����������ǹ̶��ģ�ֵҲ���������ڡ�180���ڡ�	���淢�ֲ������ԭ�򣬷ǳ���������Ҳ��֪����ʲôԭ����
					* ��ʵ��������ϣ��������������֮ת���Ķ�̬ŷ���ǣ�Ҳ���������ϵ�ת���ڡ�������
					* ��������cherno���ý�rotation�ľ���ֵ�ı��Ϊdeltaֵ(�仯ֵ)
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
		if (event.GetRepeatCount() > 0) {	//��ֹ�����������Ļ�ֻ�е�һ������
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
														/*����Ƿ���ImGuizmo�ؼ����Ϸ����������Ϸ���˵������Ҫʹ�ÿؼ����Ͳ���ѡ��ʵ��*/
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
		std::string file_path = FileDialogs::OpenFile("TribleZ Scene (*.tz)\0*.tz\0");	//��������Զ����ļ��������ĸ�ʽ TribleZ Scene (*.tz) \0*.tz\0		��������ͻὫ����(*)��.tzΪ��׺���ļ���ʾ����
		if (!file_path.empty())	//·����Ϊ�գ�˵���ɹ�������															/*  ������ \0 �������Ĳ��־��������Ĺ�����*/
		{
			OpenScene(file_path);
		}
	}
	void Editor_Layer::OpenScene(const std::filesystem::path& path)
	{
		ActiveScene = CreatRef<Scene>();	//����һ���µĳ���
		ActiveScene->ResizeView((uint32_t)m_ViewSize.x, (uint32_t)m_ViewSize.y);	//��ʼ���ӿڴ�С��ֹ������űȳ�����
		m_SceneHierarchyPanel.SetContext(ActiveScene);	//���ﱾ������û��ָ��ˢ�£����Ի������

		SceneSerializer TZ_Serializer(ActiveScene);
		TZ_Serializer.Deserializer(path.string());
	}
	void Editor_Layer::NewScene()
	{
#if 0
		std::string file_path = FileDialogs::SaveFile("TribleZ Scene (*.tz)\0*.tz\0");
		if (!file_path.empty())	//·����Ϊ�գ�˵���ɹ�������															/*  ������ \0 �������Ĳ��־��������Ĺ�����*/
		{
			ActiveScene = CreatRef<Scene>();	//����һ���µĳ���
			ActiveScene->ResizeView((uint32_t)m_ViewSize.x, (uint32_t)m_ViewSize.y);	//��ʼ���ӿڴ�С��ֹ������űȳ�����
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


