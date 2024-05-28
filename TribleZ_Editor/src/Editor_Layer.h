#pragma once
#include "TribleZ.h"
#include <vector>

#include "Platform/OpenGL/OpenGLShader.h"
#include "TribleZ_Core/Render/Renderer2D.h"

#include "ParticleSystem.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanels.h"

namespace TribleZ
{
	

	class Editor_Layer : public Layer
	{
	private:
		Renderer2D			m_Flag_Renderer;

		ShaderLibrary		ShaderLib_2D;

		Ref<VertexArray>	VertexArray_2D;

		Ref<Texture2D>		Texture_2D;
		Ref<Texture2D>		SpriteSheet_tex;
		Ref<SubTexture2D>	subTex;

		Ref<Scene>			ActiveScene;
		Ref<FrameBuffer>	FrameBuffer_2D;

		Ref<Texture2D>		m_IconPlay;
		Ref<Texture2D>		m_IconStop;

		OrthoGraphicCameraContraller CameraController;
		Editor_Camera m_EditorCamera;

#ifdef DELETE_CODE
		//实体
		Entity m_Square;
		Entity m_second_Square;
		Entity Camera_Entity;
		Entity Obj_Camera;
		bool main_cam = true;
#endif
		//粒子系统
		ParticleSystem m_ParticleSystem;
		ParticleProps m_Particle;
		//子模块
		std::unordered_map<char, Ref<TribleZ::SubTexture2D>> m_SubTexMap;
		//视口大小
		glm::vec2 m_ViewportBound[2];		//第一位装左上角坐标(minBound),第一位装右下角坐标(maxBound)
		glm::vec2 m_ViewSize;
		uint32_t Map_Width, Map_Height;
		//画面面板
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanels m_ContentBrowserPanels;
		//窗口选中和鼠标悬停
		bool m_ViewPortFocused;
		bool m_ViewPortHovered;

		Entity m_HoverdEntity;
		//ImGui控件操作模式：平移、旋转、缩放
		int m_GuizmoOperationType = -1;

		//场景状态：编辑，游玩
		enum class SceneState
		{
			Edit = 1,
			Play = 2
		};
		SceneState m_SceneState = SceneState::Edit;

	public:
		Editor_Layer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdata(TimeStep time_step) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		void UI_Toolbar();
	private:
		bool OnEventKeyPressed(EventKeyPress& event);
		bool OnEventMouseButtonPressed(EventMouseButtonPressed& event);

		void SaveSceneAs();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void NewScene();

		void OnScenePlay();
		void OnSceneEdit();
	};
}
