#include "tzpch.h"
#include "OpenGLShader.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "glm/gtc/type_ptr.hpp"
namespace TribleZ
{
	static GLenum ShdertypeFromString(const std::string& type)
	{
		if (type == "vertex_shader")
			return GL_VERTEX_SHADER;
		if (type == "fragment_shader" || type == "pixel_shader")
			return GL_FRAGMENT_SHADER;

		TZ_CORE_ASSERT(false, "unkown shader type!");
		return 0;
	}
								//asserts/shader/Texture_Shader.glsl
	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		TZ_PROFILE_FUNCTION_SIG();

		std::string source = ReadFile(filepath);
		auto shaderSource = PreProcess(source);
		CompileShader(shaderSource);

		/*-----------从文件路径中抓取着色器的名字-------------------*/
		size_t Last_Slash = filepath.find_last_of("/\\");	//filepath.find_last_of("/\\", 0)是错的，说明这个写0因该指的是从0处向前找
		Last_Slash = (Last_Slash == std::string::npos) ? 0 : Last_Slash + 1;
		size_t Last_Dot = filepath.rfind(".");
		size_t count = (Last_Dot == std::string::npos) ? filepath.size() - Last_Slash : Last_Dot - Last_Slash;
		m_Name = filepath.substr(Last_Slash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertex_source, const std::string& fragment_source)
		:m_Name(name)
	{
		TZ_PROFILE_FUNCTION_SIG();

		std::unordered_map<GLenum, std::string> source;
		source[GL_VERTEX_SHADER] = vertex_source;
		source[GL_FRAGMENT_SHADER] = fragment_source;
		CompileShader(source);
	}

	OpenGLShader::~OpenGLShader()
	{
		TZ_PROFILE_FUNCTION_SIG();

		glDeleteProgram(Renderer_Program_ID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		TZ_PROFILE_FUNCTION_SIG();

		std::string result;
		std::ifstream filein(filepath, std::ios::in | std::ios::binary);
		if (filein)
		{
			filein.seekg(0, std::ios::end);
			result.resize(filein.tellg());
			filein.seekg(0, std::ios::beg);
			filein.read(&result[0], result.size());
			filein.close();
		}
		else
		{
			TZ_CORE_ASSERT(filein, "FILEPath [{0}]  invalied!", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& src_code)
	{
		TZ_PROFILE_FUNCTION_SIG();

		std::unordered_map<GLenum, std::string> ShaderSrc;

		const char* typeToken = "#type";
		size_t typeTokenLenth = strlen(typeToken);
		size_t pos = src_code.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t endsymble = src_code.find_first_of("\r\n", pos);	//找到行末的指针
			TZ_CORE_ASSERT(endsymble != std::string::npos, "Syntax error!")
			size_t begain = pos + typeTokenLenth + 1;				//找到代表着色器的文字首
			std::string type = src_code.substr(begain, endsymble - begain);	//从哪一位开始取多少位
			TZ_CORE_ASSERT(type != "vertex_shader" || type != "fragment_shader" || type != "pixel_shader",
						  "Shader Type error!");
			
			size_t next_line = src_code.find_first_not_of("\r\n", endsymble);
			pos = src_code.find(typeToken, next_line);
			ShaderSrc[ShdertypeFromString(type)]
				= src_code.substr(next_line, 
								  pos - (next_line == std::string::npos ? src_code.size()-1 : next_line));
		}

		return ShaderSrc;

	}

	void OpenGLShader::CompileShader(const std::unordered_map<GLenum, std::string>& source_map)
	{
		TZ_PROFILE_FUNCTION_SIG();

		GLuint Program = glCreateProgram();
		TZ_CORE_ASSERT(source_map.size() <= 2, "We only support capacity lower than 2 or 2");
		//constexpr auto f = [&]() mutable { return (size_t)source_map.size(); };
		std::array<GLenum, 2> shaderIDs;
		int shaderIDIndex = 0;
		//std::vector<GLenum> shaderIDs;
		//shaderIDs.reserve(source_map.size());
		for (auto& ky : source_map)
		{
			GLenum type = ky.first;
			const std::string& Shader_source = ky.second;
			
			/*-----------输入着色器源码并编译---------------------------------------*/
			unsigned int shader_id = glCreateShader(type);
			const GLchar* Shader_source_char = Shader_source.c_str();
			glShaderSource(shader_id, 1, &Shader_source_char, nullptr);	//把源码送给GL
			glCompileShader(shader_id);
			/*----------------------------------------------------------------------*/

			int result;
			/*----------------------报错函数----------------------------------------*/
			glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE)
			{
				int maxLength = 0;
				glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shader_id, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader_id);

				TZ_CORE_ERROR("{0}", infoLog.data());
				TZ_CORE_ASSERT(false, "VertexShader Compiled Failed!")
					return;
			}
			/*----------------------报错函数----------------------------------------*/
			/*把着色器链接到渲染器程序上*/
			//shaderIDs.push_back(shader_id);
			shaderIDs[shaderIDIndex] = shader_id;
			glAttachShader(Program, shader_id);
		}	
			Renderer_Program_ID = Program;


			/*链接渲染器程序*/
			glLinkProgram(Program);
			//glValidateProgram(Program);     //验证程序是否能在当前opengl环境下运行（？）

			/*-------------检查渲染器和着色器的链接状态---------------------------------*/
			int isLinked = 0;
			glGetProgramiv(Program, GL_LINK_STATUS, &isLinked);
			if (isLinked == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(/*maxLength*/1000);
				glGetProgramInfoLog(Program, maxLength, &maxLength, &infoLog[0]);

				// We don't need the program anymore.
				glDeleteProgram(Program);
				// Don't leak shaders either.
				for (auto ID : shaderIDs)
				{
					glDeleteShader(ID);
				}

				TZ_CORE_ERROR("{0}", infoLog.data());
				TZ_CORE_ASSERT(false, "Shadere Linked Failed!");
				return;
			}
			/*-------------检查渲染器和着色器的链接状态---------------------------------*/


			/*----------着色器链接成功后要删除链接文件----------------------------------*/
			for (auto ID : shaderIDs)
			{
				glDetachShader(Renderer_Program_ID, ID);
			}
			/*--------------------------------------------------------------------------*/
		
		
	}

	/*----------------统一变量API------------------------------------------------------------------------*/
	void OpenGLShader::SetInt(const std::string& name, const int& num)
	{
		SetUniform1i(name, num);
	}
	void OpenGLShader::SetIntArray(const std::string& name, const int* value, const uint32_t count)
	{
		SetUniform1i_array(name, value, count);
	}
	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		SetUniform1f(name, value);
	}
	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) 
	{
		SetUniform3f(name, value);
	}
	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		SetUniform4f(name, value);
	}
	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value)
	{
		SetUniformMat3f(name, value);
	}
	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		SetUniformMat4f(name, value);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(Renderer_Program_ID);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}
	/*----------------统一变量API------------------------------------------------------------------------*/


	/*----------------统一变量实现------------------------------------------------------------------------*/
	/*----------------查找统一变量-----------------------*/
	int OpenGLShader::GetUniLocation(const std::string& uni_name)
	{
		if (UniNameMap.find(uni_name) != UniNameMap.end())
		{
			return UniNameMap[uni_name];
		}

		int location = glGetUniformLocation(Renderer_Program_ID, uni_name.c_str());

		if (location != -1)
		{
			UniNameMap.insert({ uni_name, location });
			return location;
		}

		return -1;
	}
	/*----------------设置统一变量-----------------------*/
	/*--------------------单int--------------------------*/
	void OpenGLShader::SetUniform1i(const std::string& uni_name, int value)
	{
		glUniform1i(GetUniLocation(uni_name), value);
	}
	/*--------------------int数组--------------------------*/
	void OpenGLShader::SetUniform1i_array(const std::string& uni_name, const int* value, const uint32_t count)
	{
		glUniform1iv(GetUniLocation(uni_name), count, value);
	}
	/*--------------------单float------------------------*/
	void OpenGLShader::SetUniform1f(const std::string& uni_name, float value)
	{
		glUniform1f(GetUniLocation(uni_name), value);
	}
	/*-----------------三向float向量---------------------*/
	void OpenGLShader::SetUniform3f(const std::string& uni_name, float value1, float value2, float value3)
	{
		glUniform3f(GetUniLocation(uni_name), value1, value2, value3);
	}
	void OpenGLShader::SetUniform3f(const std::string& uni_name, const glm::vec3& value)
	{
		glUniform3f(GetUniLocation(uni_name), value.x, value.y, value.z);
	}
	/*------------------四向float向量--------------------*/
	void OpenGLShader::SetUniform4f(const std::string& uni_name, float value1, float value2, float value3, float value4)
	{
		glUniform4f(GetUniLocation(uni_name), value1, value2, value3, value4);
	}
	void OpenGLShader::SetUniform4f(const std::string& uni_name, const glm::vec4& value)
	{
		glUniform4f(GetUniLocation(uni_name), value.r, value.g, value.b, value.a);
	}
	/*----------------三阶float矩阵----------------------*/
	void OpenGLShader::SetUniformMat3f(const std::string& uni_name, const glm::mat3& value)
	{
		glUniformMatrix3fv(GetUniLocation(uni_name), 1, GL_FALSE, glm::value_ptr(value));
	}
	/*----------------四阶float矩阵----------------------*/
	void OpenGLShader::SetUniformMat4f(const std::string& uni_name, const glm::mat4& value)
	{
												/*是否转置，OpenGL列主排列所以不用*/
		glUniformMatrix4fv(GetUniLocation(uni_name), 1, GL_FALSE, glm::value_ptr(value));
	}
	/*-----------------------------------------------------------------------------------------------------------*/

}