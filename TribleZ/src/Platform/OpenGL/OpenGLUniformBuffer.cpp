#include <tzpch.h>
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace TribleZ
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t bindIndex)
	{
		glCreateBuffers(1, &m_UniformBufferRendererID);		//׼��һ��������
		glNamedBufferData(m_UniformBufferRendererID, size, nullptr, GL_DYNAMIC_DRAW);	//�ύ���ݵ�׼���õĻ�����	// TODO: investigate usage hint	������
		glBindBufferBase(GL_UNIFORM_BUFFER, bindIndex, m_UniformBufferRendererID);		//�����UniformBuffer����һ��
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_UniformBufferRendererID);	
	}

	void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(m_UniformBufferRendererID, offset, size, data);
	}



}