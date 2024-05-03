#pragma once

#include "RendererAPI.h"

namespace TribleZ
{
	class RendererCommand
	{
	private:
		static RendererAPI* s_Renderer_api;
	public:
		inline static void Clear()
		{
			s_Renderer_api->Clear();
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_Renderer_api->SetClearColor(color);
		}

		inline static void DrawIndex(const Ref<VertexArray>& vertex_array, uint32_t IndexCount = 0)
		{

			s_Renderer_api->DrawElements(vertex_array, IndexCount);
			
		}

		inline static void Init()
		{
			s_Renderer_api->Init();
		}

		inline static void SetViewPort(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
		{
			s_Renderer_api->SetViewPort(x, y, width, height);
		}

	};

}