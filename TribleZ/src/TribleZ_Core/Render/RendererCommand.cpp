#include "tzpch.h"
#include "RendererCommand.h"
#include "Platform/OpenGL/OpenGLRenderer.h"

namespace TribleZ
{
	RendererAPI* RendererCommand::s_Renderer_api = new OpenGLRendererAPI;
}