#include "tzpch.h"

#include "Renderer.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace TribleZ
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::NONE:
			TZ_CORE_ASSERT(false, "RendererAPI::NONE is not surpported!");
			break;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		case RendererAPI::API::DirectX:
			TZ_CORE_ASSERT(false, "RendererAPI::DirectX is not surpported yet!");
			break;
		}


		TZ_CORE_ASSERT(false, "Unkonw Renderer API!");
		return nullptr;

	}
}