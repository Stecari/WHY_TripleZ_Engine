#include "tzpch.h"
#include "OpenGLBuffer.h"

#include "glad/glad.h"

namespace TribleZ
{
	/*------------------------------------------------------------------------------*/
	/*-----------------OpenGL顶点缓冲区---------------------------------------------*/
	/*------------------------------------------------------------------------------*/
	/*-----------------------静态创建----------------------*/
	OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size, float* vertex_data)
	{
		TZ_PROFILE_FUNCTION_SIG();

		glCreateBuffers(1, &m_VertexBuffer_ID);	//不用Gen, 为了保持接口一致，4.5版本以后才支持
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer_ID);
		glBufferData(GL_ARRAY_BUFFER, size, vertex_data, GL_STATIC_DRAW);
	}
	/*-----------------------动态创建----------------------*/
	OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size)
	{
		TZ_PROFILE_FUNCTION_SIG();

		glCreateBuffers(1, &m_VertexBuffer_ID);	//不用Gen, 为了保持接口一致，4.5版本以后才支持
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer_ID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}
	/*-----------------------------------------------------*/
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		TZ_PROFILE_FUNCTION_SIG();

		glDeleteBuffers(1, &m_VertexBuffer_ID);
	}
	/*-----------------------------------------------------*/
	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer_ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);	//这个0后面估计会改
	}
	/*-----------------------------------------------------*/
	void OpenGLVertexBuffer::Bind() const
	{
		TZ_PROFILE_FUNCTION_SIG();

		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer_ID);
	}
	/*-----------------------------------------------------*/
	void OpenGLVertexBuffer::UnBind() const
	{
		TZ_PROFILE_FUNCTION_SIG();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	/*------------------------------------------------------------------------------*/
	/*-----------------OpenGL顶点缓冲区---------------------------------------------*/
	/*------------------------------------------------------------------------------*/


	/*------------------------------------------------------------------------------*/
	/*-----------------OpenGL索引缓冲区---------------------------------------------*/
	/*------------------------------------------------------------------------------*/
	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int count, unsigned int* index_data)
		:m_Count(count)
	{
		TZ_PROFILE_FUNCTION_SIG();

		glCreateBuffers(1, &m_IndexBuffer_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), index_data, GL_STATIC_DRAW);
	}
	/*-----------------------------------------------------*/
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		TZ_PROFILE_FUNCTION_SIG();

		glDeleteBuffers(1, &m_IndexBuffer_ID);
	}
	/*-----------------------------------------------------*/
	void OpenGLIndexBuffer::Bind() const
	{
		TZ_PROFILE_FUNCTION_SIG();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer_ID);
	}
	/*-----------------------------------------------------*/
	void OpenGLIndexBuffer::UnBind() const
	{
		TZ_PROFILE_FUNCTION_SIG();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	/*------------------------------------------------------------------------------*/
	/*-----------------OpenGL索引缓冲区---------------------------------------------*/
	/*------------------------------------------------------------------------------*/

}