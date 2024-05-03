#pragma once
#include "vector"
//#include "memory"
#include "TribleZ_Core/Core.h"
#include "TribleZ_Core/Render/VertexArray.h"

namespace TribleZ
{
	class OpenGLVertexArray : public VertexArray
	{
	private:
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
		unsigned int m_VertexArrayID;

	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& vertex_buffer) override;
		
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const override { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	};
}