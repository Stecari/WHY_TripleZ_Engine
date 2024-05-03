#pragma once
#include "TribleZ.h"
#include <vector>

#include "Platform/OpenGL/OpenGLShader.h"
#include "TribleZ_Core/Render/Renderer2D.h"

#include "ParticleSystem.h"
#include "Panels/SceneHierarchyPanel.h"

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

		OrthoGraphicCameraContraller CameraController;

		//ΚµΜε
		Entity m_Square;
		Entity m_second_Square;
		Entity Camera_Entity;
		Entity Obj_Camera;
		bool main_cam = true;

		ParticleSystem m_ParticleSystem;
		ParticleProps m_Particle;

		std::unordered_map<char, Ref<TribleZ::SubTexture2D>> m_SubTexMap;
		glm::vec2 m_ViewSize;
		uint32_t Map_Width, Map_Height;

		SceneHierarchyPanel m_ScenePanel;

		bool m_ViewPortFocused;
		bool m_ViewPortHovered;

	public:
		Editor_Layer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdata(TimeStep time_step) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;
	};
}
