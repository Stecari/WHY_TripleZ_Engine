#include "tzpch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"

#include "future"
namespace TribleZ
{
	OpenGLTexture2D::OpenGLTexture2D(unsigned int Width, unsigned int Height)
		: m_Width(Width), m_Height(Height)
	{
		TZ_PROFILE_FUNCTION_SIG();

		//又是一个离谱的bug，不开了这个整张图是正的，但是切割过后的SubTexture是反的
		stbi_set_flip_vertically_on_load(1);		//翻转垂直坐标
		/*------------处理图片格式--------------------------------------------*/
		m_Internal_format = GL_RGBA8;
		m_Data_format = GL_RGBA;
		/*------------处理图片格式--------------------------------------------*/


						//0号纹理插槽
		glCreateTextures(GL_TEXTURE_2D, 1, &m_Texture_ID);	//绑定一个纹理数据到一个纹理插槽上
		glTextureStorage2D(m_Texture_ID, 1, m_Internal_format, m_Width, m_Height);	//缓存我们的纹理数据,GL_RGBA8决定我们以什么格式存储到OpenGL中，所以需要我们告诉，填channels就行其实
		/*这个level = 1是跟mipmap有关的参数*/
		glTextureParameteri(m_Texture_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //缩小采用的采样方式
		glTextureParameteri(m_Texture_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//放大采用的采样方式
		//glTextureParameteri(m_Texture_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//放大采用的采样方式
		glTextureParameteri(m_Texture_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);//存在空余空间时采用的环绕方式(repeat为默认)
		glTextureParameteri(m_Texture_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_FilePath(path)
	{
		TZ_PROFILE_FUNCTION_SIG();
		
		int width, height, channels;
		//又是一个离谱的bug，开了这个整张图是正的，但是切割过后的SubTexture是反的
		stbi_set_flip_vertically_on_load(1);		//翻转垂直坐标
		stbi_uc* data = nullptr;


		{
			TZ_PROFILE_SCOPE("stbi_load - OpenGLTexture2D(path)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0); //失败了就返回空指针
		}

		TZ_CORE_ASSERT(data, "Path invalied!");
		m_Width = width;
		m_Height = height;


		/*------------处理图片格式--------------------------------------------*/
			/*存在GL内部的格式   我们上传的图片数据的格式*/
		GLenum internal_format = 0, data_format = 0;
		if (channels == 4)
		{
			internal_format = GL_RGBA8;
			data_format = GL_RGBA;
		}
		else if (channels == 3)
		{
			internal_format = GL_RGB8;
			data_format = GL_RGB;
		}
		/*------------处理图片格式--------------------------------------------*/
		m_Internal_format = internal_format;
		m_Data_format = data_format;


						//0号纹理插槽
		glCreateTextures(GL_TEXTURE_2D, 1, &m_Texture_ID);	//绑定一个纹理数据到一个纹理插槽上
		glTextureStorage2D(m_Texture_ID, 1, internal_format, m_Width, m_Height);	//缓存我们的纹理数据,GL_RGBA8决定我们以什么格式存储到OpenGL中，所以需要我们告诉，填channels就行其实
							/*这个level = 1是跟mipmap有关的参数*/
		//glTextureParameteri(m_Texture_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_Texture_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //缩小采用的采样方式
		//glTextureParameteri(m_Texture_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_Texture_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//放大采用的采样方式
		glTextureParameteri(m_Texture_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);//存在空余空间时采用的环绕方式(repeat为默认)
		glTextureParameteri(m_Texture_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTextureParameteri(m_Texture_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

										/*这三个参数有点不懂*/
		glTextureSubImage2D(m_Texture_ID, 0, 0, 0, m_Width, m_Height, 
							data_format, GL_UNSIGNED_BYTE, data);	//用于上传实际的图像数据，GL_RGBA表示我们提供的数据格式
		
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		TZ_PROFILE_FUNCTION_SIG();

		glDeleteTextures(1, &m_Texture_ID);
	}

	void OpenGLTexture2D::SetData(void* data, unsigned int data_size)
	{
		TZ_PROFILE_FUNCTION_SIG();

		unsigned int bit_per_pixel = m_Data_format == GL_RGBA ? 4 : 3;
		TZ_CORE_ASSERT(data_size == m_Width * m_Height * bit_per_pixel, "Data must be entire texture!");

		glTextureSubImage2D(m_Texture_ID, 0, 0, 0,m_Width, m_Height, 
							m_Data_format, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(unsigned int slot) const
	{
		TZ_PROFILE_FUNCTION_SIG();

					  /*插槽*/
		glBindTextureUnit(slot, m_Texture_ID);	//绑定某一个纹理到某一个纹理插槽
	}
}