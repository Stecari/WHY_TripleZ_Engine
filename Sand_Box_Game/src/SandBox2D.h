#pragma once
#include "TribleZ.h"
#include <vector>


#include "ImGui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "TribleZ_Core/Render/Renderer2D.h"

#include "ParticleSystem.h"


class SandBox2D_layer : public TribleZ::Layer
{
private:
	TribleZ::Renderer2D					m_Flag_Renderer;

	TribleZ::ShaderLibrary				ShaderLib_2D;

	TribleZ::Ref<TribleZ::VertexArray>	VertexArray_2D;

	TribleZ::Ref<TribleZ::Texture2D>	Texture_2D;
	TribleZ::Ref<TribleZ::Texture2D>	SpriteSheet_tex;
	TribleZ::Ref<TribleZ::SubTexture2D>	subTex;

	TribleZ::OrthoGraphicCameraContraller CameraController;

	glm::vec4 color_editor = { 0.2f, 0.3f, 0.8f, 1.0f };

	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;

	std::unordered_map<char, TribleZ::Ref<TribleZ::SubTexture2D>> m_SubTexMap;
	uint32_t Map_Width, Map_Height;

public:
	SandBox2D_layer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdata(TribleZ::TimeStep time_step) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(TribleZ::Event& event) override;
};