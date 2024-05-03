#include "tzpch.h"

#include "Renderer.h"
#include "FrameBuffer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace TribleZ
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& Spec)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::NONE:
			TZ_CORE_ASSERT(false, "RendererAPI::NONE is not surpported!");
			break;
		case RendererAPI::API::OpenGL:
			return CreatRef<OpenGLFrameBuffer>(Spec);
		case RendererAPI::API::DirectX:
			TZ_CORE_ASSERT(false, "RendererAPI::DirectX is not surpported yet!");
			break;
		}


		TZ_CORE_ASSERT(false, "Unkonw Renderer API!");
		return nullptr;
	}
}