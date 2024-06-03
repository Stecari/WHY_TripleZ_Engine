#include "tzpch.h"
#include "OpenGLRenderer.h"

#include <glad/glad.h>

namespace TribleZ
{
	void OpenGLRendererAPI::Init()
	{
		TZ_PROFILE_FUNCTION_SIG();

		/*-------�������(͸����)-----------------------------*/
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		/*-------�������(͸����)-----------------------------*/

		/*-------������Ȳ���(ǰ���ϵ)-----------------------*/
		glEnable(GL_DEPTH_TEST);
		/*-------������Ȳ���(ǰ���ϵ)-----------------------*/
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClearBufferiv()
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClearDepth(1.0);
		glClearStencil(1);		//�����⣬���Լ��ӵģ�NewScene��֡������������
	}


	void OpenGLRendererAPI::DrawElements(const Ref<VertexArray>& vertex_array, uint32_t IndexCount)
	{
		uint32_t count = IndexCount ? IndexCount : vertex_array->GetIndexBuffer()->GetCount();
		//uint32_t count = IndexCount ? vertex_array->GetIndexBuffer()->GetCount() : IndexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)0);
		/*ÿһ�λ��궼�����������ÿһ�λ���Ҫ����װ������(����������)*/
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLRendererAPI::SetViewPort(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		glViewport(x, y, width, height);
	}
}