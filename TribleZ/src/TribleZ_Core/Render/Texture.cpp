#include "tzpch.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace TribleZ
{
	Ref<Texture2D> Texture2D::Create(unsigned int Width, unsigned int Height)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::NONE:
			TZ_CORE_ASSERT(false, "RendererAPI::NONE is not surpported!");
			break;
		case RendererAPI::API::OpenGL:
			return CreatRef<OpenGLTexture2D>(Width, Height);
		case RendererAPI::API::DirectX:
			TZ_CORE_ASSERT(false, "RendererAPI::DirectX is not surpported yet!");
			break;
		}


		TZ_CORE_ASSERT(false, "Unkonw Renderer API!");
		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(const std::string& FilePath)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::NONE:
			TZ_CORE_ASSERT(false, "RendererAPI::NONE is not surpported!");
			break;
		case RendererAPI::API::OpenGL:
			return CreatRef<OpenGLTexture2D>(FilePath);
		case RendererAPI::API::DirectX:
			TZ_CORE_ASSERT(false, "RendererAPI::DirectX is not surpported yet!");
			break;
		}


		TZ_CORE_ASSERT(false, "Unkonw Renderer API!");
		return nullptr;
	}
}