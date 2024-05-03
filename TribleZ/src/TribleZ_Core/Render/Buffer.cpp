#include "tzpch.h"

#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include "Renderer.h"

namespace TribleZ
{
	/*---------------------------------------------顶点缓冲区---------------------------------------------*/
	Ref<VertexBuffer> VertexBuffer::Create(size_t size, float* vertex_data)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::NONE:
				TZ_CORE_ASSERT(false, "RendererAPI::NONE is not surpported!");
				break;
			case RendererAPI::API::OpenGL:
				return CreatRef<OpenGLVertexBuffer>(size, vertex_data);
			case RendererAPI::API::DirectX:
				TZ_CORE_ASSERT(false, "RendererAPI::DirectX is not surpported yet!");
				break;
		}


		TZ_CORE_ASSERT(false, "Unkonw Renderer API!");
		return nullptr;
	}
	Ref<VertexBuffer> VertexBuffer::Create(size_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::NONE:
			TZ_CORE_ASSERT(false, "RendererAPI::NONE is not surpported!");
			break;
		case RendererAPI::API::OpenGL:
			return CreatRef<OpenGLVertexBuffer>(size);
		case RendererAPI::API::DirectX:
			TZ_CORE_ASSERT(false, "RendererAPI::DirectX is not surpported yet!");
			break;
		}


		TZ_CORE_ASSERT(false, "Unkonw Renderer API!");
		return nullptr;
	}
	/*---------------------------------------------顶点缓冲区---------------------------------------------*/



	/*---------------------------------------------索引缓冲区---------------------------------------------*/
	Ref<IndexBuffer> IndexBuffer::Create(unsigned int count, unsigned int* index_data)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::NONE:
			TZ_CORE_ASSERT(false, "RendererAPI::NONE is not surpported!");
			break;
		case RendererAPI::API::OpenGL:
			return CreatRef<OpenGLIndexBuffer>(count, index_data);
		case RendererAPI::API::DirectX:
			TZ_CORE_ASSERT(false, "RendererAPI::DirectX is not surpported yet!");
			break;
		}


		TZ_CORE_ASSERT(false, "Unkonw Renderer API!");
		return nullptr;
	}
	/*---------------------------------------------索引缓冲区---------------------------------------------*/

}