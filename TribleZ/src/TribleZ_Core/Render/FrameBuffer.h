#pragma once
#include "TribleZ_Core/Core.h"

namespace TribleZ
{
	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		//FrameBufferFormat Format;
		uint32_t Sample = 1;		//��Ҳ��֪������ʲô�ã�ȡ����������

		bool SwapChainTargets = false;	//�����㴴��һ������� ֡����ȡ
										//��Ȼ�����������������ʵ���ڵ�  ��ǣ����ֲ�ȷ����
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