#pragma once
#include "TribleZ_Core/Render/FrameBuffer.h"


namespace TribleZ
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	private:
		FrameBufferSpecification m_Specification;

		uint32_t m_FrameBufferID = 0;		//֡������ID
		//uint32_t m_ColorAttachment = 0;		//��ɫ���ID���������������
		//uint32_t m_DepthAttachment = 0;		//��Ȳ��ID
		std::vector<uint32_t> m_ColorAttachments;	//��ɫ���ID���������������
		uint32_t m_DepthAttachment = 0;				//��Ȳ��ID

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& Spec);
		virtual ~OpenGLFrameBuffer();

		/*����Ҫ��������һ֡*/
		void invalidate();		//��Ч�������߽���resize������recteate?

		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { TZ_CORE_ASSERT(index < m_ColorAttachments.size());  return m_ColorAttachments[index]; }
		virtual FrameBufferSpecification& GetSpecification() override { return m_Specification; }
		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }
	};
}