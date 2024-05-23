#include <tzpch.h>

#include "UniformBuffer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"
#include "RendererAPI.h"

namespace TribleZ
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t bindIndex)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::NONE:
			TZ_CORE_ASSERT(false, "RendererAPI::NONE is not surpported!");
			break;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLUniformBuffer>(size, bindIndex);
		case RendererAPI::API::DirectX:
			TZ_CORE_ASSERT(false, "RendererAPI::DirectX is not surpported yet!");
			break;
		}


		TZ_CORE_ASSERT(false, "Unkonw Renderer API!");
		return nullptr;

	}
}