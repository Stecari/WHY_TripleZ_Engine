#pragma once
#include "TribleZ_Core/Core.h"

namespace TribleZ
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		// 颜色
		RGBA8,
		RED_INTEGER,

		// 深度
		DEPTH24_STENCIL8,

		// Defaults
		Depth = DEPTH24_STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments_list(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments_list;
	};

	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		//FrameBufferFormat Format;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Sample = 1;		//我也不知道这有什么用，取样？案例？

		bool SwapChainTargets = false;	//允许你创建一个虚拟的 帧缓冲取
										//当然这个缓冲区并不是真实存在的  后记：我又不确定了
	};
	class FrameBuffer
	{
	private:

	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;


		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual FrameBufferSpecification& GetSpecification() = 0;
		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& Spec);
	};
}