#include "tzpch.h"

#include "OpenGLVertexArray.h"
#include "glad/glad.h"

namespace TribleZ
{

	static GLenum ShaderDataTypeToGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
		case ShaderDataType::Float1:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4: 	return GL_FLOAT;

		case ShaderDataType::Int1:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:		return GL_INT;

		case ShaderDataType::Bool:		return GL_BOOL;
		}

		TZ_CORE_ASSERT(false, "ShaderDataType NONE or error!");
		return 0;
	}


	OpenGLVertexArray::OpenGLVertexArray()
	{
		TZ_PROFILE_FUNCTION_SIG();

		glGenVertexArrays(1, &m_VertexArrayID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		TZ_PROFILE_FUNCTION_SIG();

		glDeleteVertexArrays(1, &m_VertexArrayID);
	}

	void OpenGLVertexArray::Bind() const
	{
		TZ_PROFILE_FUNCTION_SIG();

		glBindVertexArray(m_VertexArrayID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		TZ_PROFILE_FUNCTION_SIG();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer)
	{
		TZ_PROFILE_FUNCTION_SIG();

		TZ_CORE_ASSERT(vertex_buffer->GetLayout().GetElementLayout().size(), "Buffer has no Layout!");

		glBindVertexArray(m_VertexArrayID);		//先绑定顶点数组
		vertex_buffer->Bind();					//再绑定缓冲区
		int index = 0;
		for (const BufferElement& element : vertex_buffer->GetLayout())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, /*顶点元素索引*/
			/*有几个数据*/		  element.GetComponentCount(),
			/*数据类型*/		  ShaderDataTypeToGLType(element.Type),
			/*标准化*/			  element.Normalized ? GL_FALSE : GL_TRUE,
			/*两个点间的字节数*/  vertex_buffer->GetLayout().GetStride(),
			/*元素起始字节*/	  (const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertex_buffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& index_buffer)
	{
		TZ_PROFILE_FUNCTION_SIG();

		glBindVertexArray(m_VertexArrayID);
		index_buffer->Bind();

		m_IndexBuffer = index_buffer;
	}
}