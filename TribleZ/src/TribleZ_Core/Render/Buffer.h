#pragma once
#include "TribleZ_Core/Core.h"

namespace TribleZ
{
	/*----------------布局数据-------------------------------------------------*/
	enum class ShaderDataType
	{
		NONE = 0,
		Float1, Float2, Float3, Float4,	/* 1 - 4 */
		Int1, Int2, Int3, Int4,	/* 5 - 8 */
		Mat3, Mat4,					/* 9 - 10*/
		Bool							/*11 - 11*/
	};
	/*------------获取着色器数据类型的大小------------*/
	static size_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float1:	return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4: 	return 4 * 4;
		case ShaderDataType::Int1:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Bool:		return sizeof(bool);
		}

		TZ_CORE_ASSERT(false, "GetType error, ShaderDataType NONE or error!");
		return 0;
	}
	/*-----------------布局元素结构--------------------*/
	struct BufferElement
	{
		size_t Size;
		std::string Name;
		ShaderDataType Type;
		unsigned int Offset;
		bool Normalized;

		BufferElement() {}

		BufferElement(std::string name, ShaderDataType type, bool normalized = false)
			:Size(ShaderDataTypeSize(type)),
			Name(name), Type(type), Offset(0), Normalized(normalized)
		{}

		unsigned int GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Bool:
			case ShaderDataType::Float1:
			case ShaderDataType::Int1:		return 1;

			case ShaderDataType::Float2:
			case ShaderDataType::Int2:		return 2;

			case ShaderDataType::Float3:
			case ShaderDataType::Int3:		return 3;

			case ShaderDataType::Float4:
			case ShaderDataType::Int4:		return 4;

			case ShaderDataType::Mat3:		return 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4;
			}

			TZ_CORE_ASSERT(false, "GetCount error,ShaderDataType NONE or error!");
			return 0;
		}

	};
	/*----------------顶点属性布局---------------------*/
	class BufferLayout
	{
	private:
		std::vector<BufferElement> m_ElementLayout;
		unsigned int m_Stride = 0;	/*两个顶点之间的字节数*/
	public:
		BufferLayout()
		{}

		BufferLayout(const std::initializer_list<BufferElement>& layout_ele)
			:m_ElementLayout(layout_ele)
		{
			CalculateOffsetandStride();
		}

		void CalculateOffsetandStride()
		{
			unsigned int offset = 0;
			m_Stride = 0;
			for (auto& element : m_ElementLayout)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

		inline const unsigned int GetStride() const { return m_Stride; }

		inline std::vector<BufferElement>::iterator begin() { return m_ElementLayout.begin(); }
		inline std::vector<BufferElement>::const_iterator begin() const { return m_ElementLayout.begin(); }
		inline std::vector<BufferElement>::iterator end() { return m_ElementLayout.end(); }
		inline std::vector<BufferElement>::const_iterator end() const { return m_ElementLayout.end(); }

		inline const std::vector<BufferElement>& GetElementLayout() const { return m_ElementLayout; }

	};
	/*-------------------------------------------------------------------------*/


	/*-------------------顶点缓冲区---------------------------------------------*/
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static Ref<VertexBuffer> Create(size_t size, float* vertex_data);
		static Ref<VertexBuffer> Create(size_t size);
	};
	/*-------------------------------------------------------------------------*/


	/*-------------------顶点缓冲区---------------------------------------------*/
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual unsigned int GetCount() const = 0;

		static Ref<IndexBuffer> Create(unsigned int count, unsigned int* index_data);
	};
	/*-------------------------------------------------------------------------*/



}