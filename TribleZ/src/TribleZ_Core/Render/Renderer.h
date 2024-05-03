#pragma once
#include "TribleZ_Core/Core.h"
#include "RendererCommand.h"
#include "Renderer2D.h"
#include "Camera.h"
#include "Shader.h"

namespace TribleZ
{

	class Renderer
	{
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectMat;
		};

		static SceneData* m_SceneData;

	public:
		inline static void Init() 
		{
			TZ_PROFILE_FUNCTION_SIG();

			RendererCommand::Init(); 
			Renderer2D::Init();
		}
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void SceneBegin(OrthoGraphicCamera& camera);
		static void SceneEnd();

		static void Submit(const Ref<Shader>& shader,
						   const Ref<VertexArray>& vertex_array, 
						   const glm::mat4& modle_mat = glm::mat4(1.0f));

		static void OnWindowResize(unsigned int Width, unsigned int Height);
	};
}