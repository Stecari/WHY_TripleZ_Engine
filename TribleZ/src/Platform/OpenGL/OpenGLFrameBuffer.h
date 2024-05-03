#pragma once
#include "TribleZ_Core/Render/FrameBuffer.h"


namespace TribleZ
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	private:
		FrameBufferSpecification m_Specification;

		uint32_t m_FrameBufferID = 0;		//֡������ID
		uint32_t m_ColorAttachment = 0;		//��ɫ���ID���������������
		uint32_t m_DepthAttachment = 0;		//��Ȳ��ID
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& Spec);
		virtual ~OpenGLFrameBuffer();

		/*����Ҫ��������һ֡*/
		void invalidate();		//��Ч�������߽���resize������recteate?

		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachment() const override { return m_ColorAttachment; }
		virtual FrameBufferSpecification& GetSpecification() override { return m_Specification; }
		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }
	};
}