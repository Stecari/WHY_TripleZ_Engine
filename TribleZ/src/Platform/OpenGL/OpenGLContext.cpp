#include "tzpch.h"

#include "OpenGLContext.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace TribleZ
{
	
	OpenGLContext::OpenGLContext(GLFWwindow* window_handle)
		:m_Window_Handle(window_handle)
	{
		TZ_CORE_ASSERT(m_Window_Handle, "OpenGLContext Window did not Initialized!")
		
	}

	OpenGLContext::~OpenGLContext()
	{
	}

	void OpenGLContext::Init()
	{
		TZ_PROFILE_FUNCTION_SIG();

		glfwMakeContextCurrent(m_Window_Handle);

		/*-----------Glad初始化-------------------------------------------------*/
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		TZ_CORE_ASSERT(status, "Glad did not Initialized!");
		/*----------------------------------------------------------------------*/


		/*-----------获得一些信息-----------------------------------------------*/
		TZ_CORE_INFO("OpenGL:");
		TZ_CORE_INFO("	Vender:{0}", (char*)glGetString(GL_VENDOR));
		TZ_CORE_INFO("	Renderer:{0}", (char*)glGetString(GL_RENDERER));
		TZ_CORE_INFO("	Vertion:{0}", (char*)glGetString(GL_VERSION));
		/*-----------------------------------------------------------------------*/
	
	}

	void OpenGLContext::SwapBuffers()
	{
		TZ_PROFILE_FUNCTION_SIG();

		glfwSwapBuffers(m_Window_Handle);
	}






}