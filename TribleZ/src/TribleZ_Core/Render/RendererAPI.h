#pragma once
#include "TribleZ_Core/Core.h"

#include "glm/glm.hpp"
#include "memory"
#include "VertexArray.h"

namespace TribleZ
{
	class RendererAPI
	{
	public:
		enum class API
		{
			NONE = 0, OpenGL, DirectX
		};

	public:
		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;

		virtual void DrawElements(const Ref<VertexArray>& vertex_array, uint32_t IndexCount = 0) = 0;
		virtual void Init() = 0;

		virtual void SetViewPort(unsigned int x, unsigned int y, unsigned int width, unsigned int hight) = 0;

		inline static API GetAPI() { return render_api; }
	private:
		static API render_api;
	};
}