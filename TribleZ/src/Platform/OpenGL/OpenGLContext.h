#pragma once
#include "TribleZ_Core/Core.h"

#include "TribleZ_Core/Render/GraphicRenderContext.h"

struct GLFWwindow;	//����,��Ҫ�ǰ���ͷ�ļ��Ļ��ᵼ�µ����Ұ���glfw3.h

namespace TribleZ
{
	class OpenGLContext : public GraphicRenderContext
	{
	private:
		GLFWwindow* m_Window_Handle;

	public:
		OpenGLContext(GLFWwindow* glfwWindow);
		~OpenGLContext();

		virtual void Init() override ;

		virtual void SwapBuffers() override;

	};
};