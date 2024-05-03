#pragma once
#include "TribleZ_Core/Core.h"

namespace TribleZ
{
	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		//FrameBufferFormat Format;
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

		virtual uint32_t GetColorAttachment() const = 0;
		virtual FrameBufferSpecification& GetSpecification() = 0;
		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& Spec);
	};
}