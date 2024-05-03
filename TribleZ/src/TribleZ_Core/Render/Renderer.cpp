#include "tzpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace TribleZ
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::SceneBegin(OrthoGraphicCamera& camera)
	{
		//得到渲染对象的材质信息
		m_SceneData->ViewProjectMat = camera.GetViewProjectMat();
	}

	void Renderer::SceneEnd()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader,
						  const Ref<VertexArray>& vertex_array, 
						  const glm::mat4& modle_mat)
	{
		shader->Bind();	//选择着色器程序
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_ViewProjection", m_SceneData->ViewProjectMat);
		//shader->SetUniformMat4f("u_PVM", m_SceneData->ViewProjectMat * modle_mat);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4f("u_Model_trans", modle_mat);
		
		
		vertex_array->Bind();

		RendererCommand::DrawIndex(vertex_array);
	}

	void Renderer::OnWindowResize(unsigned int Width, unsigned int Height)
	{
		RendererCommand::SetViewPort(0, 0, Width, Height);
	}

}