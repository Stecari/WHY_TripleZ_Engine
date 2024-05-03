#pragma once
#include "TribleZ_Core/Core.h"
#include "TribleZ_Core/Render/Buffer.h"


namespace TribleZ
{

	/*---------OpenGL顶点缓冲区---------------------------------------*/
	class OpenGLVertexBuffer : public VertexBuffer
	{
	private:
		unsigned int m_VertexBuffer_ID;
		BufferLayout m_Layout;

	public:
		OpenGLVertexBuffer(size_t size_t, float* vertex_data);
		OpenGLVertexBuffer(size_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void SetData(const void* data, uint32_t size) override;

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		virtual const BufferLayout& GetLayout() const override { return m_Layout; }



		virtual void Bind() const override;
		virtual void UnBind() const override;
	};
	/*----------------------------------------------------------------*/



	/*---------OpenGL索引缓冲区---------------------------------------*/
	class OpenGLIndexBuffer : public IndexBuffer
	{
	private:
		unsigned int m_IndexBuffer_ID;
		unsigned int m_Count;
	public:
		OpenGLIndexBuffer(unsigned int count, unsigned int* vertex_data);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void UnBind() const;

		virtual unsigned int GetCount() const { return m_Count; }
	};
	/*----------------------------------------------------------------*/

}