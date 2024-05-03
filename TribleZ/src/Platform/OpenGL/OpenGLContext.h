#pragma once
#include "TribleZ_Core/Core.h"

#include "TribleZ_Core/Render/GraphicRenderContext.h"

struct GLFWwindow;	//声明,主要是包含头文件的话会导致到处乱包含glfw3.h

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