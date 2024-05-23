#pragma once
#include "TribleZ_Core/Render/UniformBuffer.h"
/*���ⷢ�֣�Ctrl��~�����Ե�������̨*/

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