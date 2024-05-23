#include <tzpch.h>
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace TribleZ
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t bindIndex)
	{
		glCreateBuffers(1, &m_UniformBufferRendererID);		//准备一个缓存区
		glNamedBufferData(m_UniformBufferRendererID, size, nullptr, GL_DYNAMIC_DRAW);	//提交数据到准备好的缓存区	// TODO: investigate usage hint	有问题
		glBindBufferBase(GL_UNIFORM_BUFFER, bindIndex, m_UniformBufferRendererID);		//将其和UniformBuffer绑定在一起
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