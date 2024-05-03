#pragma once
#include "TribleZ_Core/Core.h"
#include "TribleZ_Core/Render/Texture.h"
#include "string"

#include "glad/glad.h"

namespace TribleZ
{
	class OpenGLTexture2D : public Texture2D
	{
	private:
		std::string m_FilePath;
		unsigned int m_Width;
		unsigned int m_Height;

		unsigned int m_Texture_ID;
			   /*GL内部存储的格式*/		/*我们读取的格式*/
		GLenum m_Internal_format = GL_RGBA8, m_Data_format = GL_RGBA;

	public:
		OpenGLTexture2D(unsigned int Width, unsigned int Height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		//virtual unsigned int  GetTexID() const override { return m_Texture_ID; }
		virtual uint32_t  GetWidth() const override { return m_Width; }
		virtual uint32_t  GetHeight() const override { return m_Height; }
		virtual uint32_t  GetID() const override { return m_Texture_ID; }
		virtual void SetData(void* data, unsigned int data_size) override;

		virtual bool operator==(const Texture& other) const override
		{
			return (m_Texture_ID == ((OpenGLTexture2D&)other).m_Texture_ID);
		}

		virtual void Bind(unsigned int slot = 0) const override;
	};
}
