#pragma once
#include "TribleZ_Core/Core.h"

#include "TribleZ_Core/Render/RendererAPI.h"

namespace TribleZ
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void Clear() override;

		virtual void SetViewPort(unsigned int x, unsigned int y, 
								 unsigned int width, unsigned int height) override;
		virtual void SetClearColor(const glm::vec4& color) override;

		virtual void DrawElements(const Ref<VertexArray>& vertex_array, uint32_t IndexCount = 0) override;

	};
}