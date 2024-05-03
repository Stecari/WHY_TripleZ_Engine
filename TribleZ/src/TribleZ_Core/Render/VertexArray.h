#pragma once
#include "TribleZ_Core/Core.h"

#include "Buffer.h"
//#include "memory"

namespace TribleZ
{
	/*-------------------¶¥µãÊý×é------------------------------------------------*/
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& vertex_buffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};
	/*-------------------------------------------------------------------------*/
}