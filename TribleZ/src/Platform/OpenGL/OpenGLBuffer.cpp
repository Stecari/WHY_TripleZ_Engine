#include "tzpch.h"
#include "OpenGLBuffer.h"

#include "glad/glad.h"

namespace TribleZ
{
	/*------------------------------------------------------------------------------*/
	/*-----------------OpenGL���㻺����---------------------------------------------*/
	/*------------------------------------------------------------------------------*/
	/*-----------------------��̬����----------------------*/
	OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size, float* vertex_data)
	{
		TZ_PROFILE_FUNCTION_SIG();

		glCreateBuffers(1, &m_VertexBuffer_ID);	//����Gen, Ϊ�˱��ֽӿ�һ�£�4.5�汾�Ժ��֧��
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer_ID);
		glBufferData(GL_ARRAY_BUFFER, size, vertex_data, GL_STATIC_DRAW);
	}
	/*-----------------------��̬����----------------------*/
	OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size)
	{
		TZ_PROFILE_FUNCTION_SIG();

		glCreateBuffers(1, &m_VertexBuffer_ID);	//����Gen, Ϊ�˱��ֽӿ�һ�£�4.5�汾�Ժ��֧��
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
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);	//���0������ƻ��
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
	/*-----------------OpenGL���㻺����---------------------------------------------*/
	/*------------------------------------------------------------------------------*/


	/*------------------------------------------------------------------------------*/
	/*-----------------OpenGL����������---------------------------------------------*/
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
	/*-----------------OpenGL����������---------------------------------------------*/
	/*------------------------------------------------------------------------------*/

}