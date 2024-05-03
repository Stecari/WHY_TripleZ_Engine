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

		//����һ�����׵�bug���������������ͼ�����ģ������и�����SubTexture�Ƿ���
		stbi_set_flip_vertically_on_load(1);		//��ת��ֱ����
		/*------------����ͼƬ��ʽ--------------------------------------------*/
		m_Internal_format = GL_RGBA8;
		m_Data_format = GL_RGBA;
		/*------------����ͼƬ��ʽ--------------------------------------------*/


						//0��������
		glCreateTextures(GL_TEXTURE_2D, 1, &m_Texture_ID);	//��һ���������ݵ�һ����������
		glTextureStorage2D(m_Texture_ID, 1, m_Internal_format, m_Width, m_Height);	//�������ǵ���������,GL_RGBA8����������ʲô��ʽ�洢��OpenGL�У�������Ҫ���Ǹ��ߣ���channels������ʵ
		/*���level = 1�Ǹ�mipmap�йصĲ���*/
		glTextureParameteri(m_Texture_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //��С���õĲ�����ʽ
		glTextureParameteri(m_Texture_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//�Ŵ���õĲ�����ʽ
		//glTextureParameteri(m_Texture_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//�Ŵ���õĲ�����ʽ
		glTextureParameteri(m_Texture_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);//���ڿ���ռ�ʱ���õĻ��Ʒ�ʽ(repeatΪĬ��)
		glTextureParameteri(m_Texture_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_FilePath(path)
	{
		TZ_PROFILE_FUNCTION_SIG();
		
		int width, height, channels;
		//����һ�����׵�bug�������������ͼ�����ģ������и�����SubTexture�Ƿ���
		stbi_set_flip_vertically_on_load(1);		//��ת��ֱ����
		stbi_uc* data = nullptr;


		{
			TZ_PROFILE_SCOPE("stbi_load - OpenGLTexture2D(path)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0); //ʧ���˾ͷ��ؿ�ָ��
		}

		TZ_CORE_ASSERT(data, "Path invalied!");
		m_Width = width;
		m_Height = height;


		/*------------����ͼƬ��ʽ--------------------------------------------*/
			/*����GL�ڲ��ĸ�ʽ   �����ϴ���ͼƬ���ݵĸ�ʽ*/
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
		/*------------����ͼƬ��ʽ--------------------------------------------*/
		m_Internal_format = internal_format;
		m_Data_format = data_format;


						//0��������
		glCreateTextures(GL_TEXTURE_2D, 1, &m_Texture_ID);	//��һ���������ݵ�һ����������
		glTextureStorage2D(m_Texture_ID, 1, internal_format, m_Width, m_Height);	//�������ǵ���������,GL_RGBA8����������ʲô��ʽ�洢��OpenGL�У�������Ҫ���Ǹ��ߣ���channels������ʵ
							/*���level = 1�Ǹ�mipmap�йصĲ���*/
		//glTextureParameteri(m_Texture_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_Texture_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //��С���õĲ�����ʽ
		//glTextureParameteri(m_Texture_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_Texture_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//�Ŵ���õĲ�����ʽ
		glTextureParameteri(m_Texture_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);//���ڿ���ռ�ʱ���õĻ��Ʒ�ʽ(repeatΪĬ��)
		glTextureParameteri(m_Texture_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTextureParameteri(m_Texture_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

										/*�����������е㲻��*/
		glTextureSubImage2D(m_Texture_ID, 0, 0, 0, m_Width, m_Height, 
							data_format, GL_UNSIGNED_BYTE, data);	//�����ϴ�ʵ�ʵ�ͼ�����ݣ�GL_RGBA��ʾ�����ṩ�����ݸ�ʽ
		
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

					  /*���*/
		glBindTextureUnit(slot, m_Texture_ID);	//��ĳһ������ĳһ��������
	}
}