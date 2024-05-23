#pragma once
#include "TribleZ_Core/Render/UniformBuffer.h"
/*意外发现，Ctrl加~键可以调出控制台*/

namespace TribleZ
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	private:
		uint32_t m_UniformBufferRendererID = 0;
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t bindIndex);
		virtual ~OpenGLUniformBuffer();

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

	};
}