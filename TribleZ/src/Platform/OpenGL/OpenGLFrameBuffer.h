#pragma once
#include "TribleZ_Core/Render/FrameBuffer.h"


namespace TribleZ
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	private:
		FrameBufferSpecification m_Specification;

		uint32_t m_FrameBufferID = 0;		//帧缓冲区ID
		uint32_t m_ColorAttachment = 0;		//颜色插件ID，纹理包括在这里
		uint32_t m_DepthAttachment = 0;		//深度插件ID
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& Spec);
		virtual ~OpenGLFrameBuffer();

		/*像是要重新生成一帧*/
		void invalidate();		//无效化，或者叫做resize，或者recteate?

		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachment() const override { return m_ColorAttachment; }
		virtual FrameBufferSpecification& GetSpecification() override { return m_Specification; }
		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }
	};
}