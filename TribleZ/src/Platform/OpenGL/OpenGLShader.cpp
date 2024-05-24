#include "tzpch.h"
#include "OpenGLShader.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include <filesystem>
#include "glm/gtc/type_ptr.hpp"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

//#include "Tool/TZ_TimeR.h"

namespace TribleZ
{
	namespace Utils
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
		static std::string GLShaderStageToString(uint32_t stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:		return "GL_VERTEX_SHADER";
				case GL_FRAGMENT_SHADER:	return "GL_FRAGMENT_SHADER";
			}
			TZ_CORE_ASSERT(false, "unkown shader type!");
			return "";
		}

		static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:		return ".cached_vulkan.vert";
				case GL_FRAGMENT_SHADER:	return ".cached_vulkan.frag";
			}
			TZ_CORE_ASSERT( false, "ERROR Vulkan Shader Type" );
			return "";
		}

		static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:		return ".cached_opengl.vert";
				case GL_FRAGMENT_SHADER:	return ".cached_opengl.frag";
			}
			TZ_CORE_ASSERT(false, "ERROR OpenGL Shader Type");
			return "";
		}
		static const char* GetShaderCacheDirectory()
		{
			//ȷ�������Ŀ¼�ǺϷ���
			return "asserts/cache/shader/openGL";
		}

		static void CreateCacheDirectoryIfNeeded()
		{
			std::string cacheDirectory = GetShaderCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory)){
				std::filesystem::create_directory(cacheDirectory);
			}
		}

		static std::filesystem::path GetCureentDirectory()
		{

		}


		static shaderc_shader_kind GLShaderStageToShaderc(GLenum GLstage)
		{
			switch (GLstage)
			{
			case GL_VERTEX_SHADER:		return shaderc_shader_kind::shaderc_vertex_shader;
			case GL_FRAGMENT_SHADER:	return shaderc_shader_kind::shaderc_fragment_shader;
			}
			TZ_CORE_ASSERT(false, "ERROR OpenGL Shader Type");
			return (shaderc_shader_kind)0;	//���ﻹ��������ģ�0��ʵ����shaderc_vertex_shader�����ܵ���������ص�
		}
	}/*---Utils----*/
	

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////OpenGLShader////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


								//asserts/shader/Texture_Shader.glsl
	OpenGLShader::OpenGLShader(const std::string& filepath)
		:m_FilePath(filepath)
	{
		TZ_PROFILE_FUNCTION_SIG();
		Utils::CreateCacheDirectoryIfNeeded();

		std::string source = ReadFile(filepath);
		auto shaderSource = PreProcess(source);
		//CompileShader(shaderSource);
		{
			
			CompiledOrGetVulkanBinaries(shaderSource);
			CompiledOrGetOpenGLBinaries();
			CreatePrograme();

		}

		/*-----------���ļ�·����ץȡ��ɫ��������-------------------*/
		size_t Last_Slash = filepath.find_last_of("/\\");	//filepath.find_last_of("/\\", 0)�Ǵ�ģ�˵�����д0���ָ���Ǵ�0����ǰ��
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
		//CompileShader(source);
		{
			CompiledOrGetVulkanBinaries(source);
			CompiledOrGetOpenGLBinaries();
			CreatePrograme();
		}
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
		else{
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
			size_t endsymble = src_code.find_first_of("\r\n", pos);	//�ҵ���ĩ��ָ��
			TZ_CORE_ASSERT(endsymble != std::string::npos, "Syntax error!")
			size_t begain = pos + typeTokenLenth + 1;				//�ҵ�������ɫ����������
			std::string type = src_code.substr(begain, endsymble - begain);	//����һλ��ʼȡ����λ
			TZ_CORE_ASSERT(type != "vertex_shader" || type != "fragment_shader" || type != "pixel_shader",
						  "Shader Type error!");
			
			size_t next_line = src_code.find_first_not_of("\r\n", endsymble);
			pos = src_code.find(typeToken, next_line);
			ShaderSrc[Utils::ShdertypeFromString(type)]
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
			
			/*-----------������ɫ��Դ�벢����---------------------------------------*/
			unsigned int shader_id = glCreateShader(type);
			const GLchar* Shader_source_char = Shader_source.c_str();
			glShaderSource(shader_id, 1, &Shader_source_char, nullptr);	//��Դ���͸�GL
			glCompileShader(shader_id);
			/*----------------------------------------------------------------------*/

			int result;
			/*----------------------������----------------------------------------*/
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
			/*----------------------������----------------------------------------*/
			/*����ɫ�����ӵ���Ⱦ��������*/
			//shaderIDs.push_back(shader_id);
			shaderIDs[shaderIDIndex] = shader_id;
			glAttachShader(Program, shader_id);
		}
		Renderer_Program_ID = Program;


		/*������Ⱦ������*/
		glLinkProgram(Program);
		//glValidateProgram(Program);     //��֤�����Ƿ����ڵ�ǰopengl���������У�����

		/*-------------�����Ⱦ������ɫ��������״̬---------------------------------*/
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
		/*-------------�����Ⱦ������ɫ��������״̬---------------------------------*/


		/*----------��ɫ�����ӳɹ���Ҫɾ�������ļ�----------------------------------*/
		for (auto ID : shaderIDs)
		{
			glDetachShader(Renderer_Program_ID, ID);
		}
		/*--------------------------------------------------------------------------*/


	}

	//shaderSource��ÿһ��pair��Ӧһ��shader������fragmentshader���������Ϊ���Ƿֱ����ɶ�Ӧ��vulkan���͵�binary�ļ�
	void OpenGLShader::CompiledOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSource_map)
	{
		GLuint program = glCreateProgram();

		//���������� shaderc�ṩ��
		// ����������������ѡ�����
		shaderc::Compiler compiler;
		shaderc::CompileOptions compiler_options;

		compiler_options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = true;
		if (optimize) {
			compiler_options.SetOptimizationLevel(shaderc_optimization_level_performance);
		}

		std::filesystem::path cacheDirectory = Utils::GetShaderCacheDirectory();
		std::filesystem::path shaderFilePath = m_FilePath;

		auto& shaderData = m_VulkanSPIRVCache;		//���������ݣ�
		shaderData.clear();
		/*�滻�ļ���չ��Ϊ�������е����ݣ�����.**�е�**��λ�ã��յľ����ÿո���棬�൱��ɾ����չ��*/
		const std::string shaderName = shaderFilePath.replace_extension().filename().string();
		for (auto&& [stage, source] : shaderSource_map)	//Դ�����ݣ������������ļ�Դ���(string)	����������ÿ��shader������vert,frag��Ϊ������һ��������Binary�ļ�
		{
			//std::filesystem::path shaderFilePath = m_FilePath;
			//std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));
			std::filesystem::path cachedPath = cacheDirectory / (shaderName + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())	//����Ѿ����ˣ�ֱ�Ӷ�ȡ�Ϳ�����,�浽data��
			{
				in.seekg(0, std::ios::end);				//��������ļ����
				auto size = in.tellg();					//��ȡ���λ�ã�����size
				in.seekg(0, std::ios::beg);				//��������ļ���ʼ

				auto& data = shaderData[stage];			//�õ�����
				data.resize(size / sizeof(uint32_t));	//sizeΪ�ֽ�ƫ�ƣ�ת��Ϊcount�����ô�С
				in.read((char*)data.data(), size);		//��char��ʽ��ȡ�������ļ�
			}
			else				//���û�У������һ��
			{
				//��glsl�ļ������spir_v
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderc(stage),
					m_FilePath.c_str(), compiler_options);
				  /*shaderName.c_str()*/
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)	//���
				{
					TZ_CORE_CRITICAL(module.GetErrorMessage());
					TZ_CORE_ASSERT(false);
				}
														/*�������������������Ϊͷָ���βָ��*/
				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));	//���ֽڱ�����ʽд��
					out.flush();
					out.close();
				}
			}
		}

		//������ý����е�shader������ShaderData���棬����ʱ��ֻ����ʾһЩcompiler�������Ϣ
		for (auto&& [stage, data] : shaderData){
			Reflect(stage, data);
		}

		/*
		GLuint program = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();


		for (auto&& [stage, source] : shaderSources)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					TZ_CORE_ERROR(module.GetErrorMessage());
					TZ_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data] : shaderData)
			Reflect(stage, data);*/
	}

	void OpenGLShader::CompiledOrGetOpenGLBinaries()
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions compiler_option;

		compiler_option.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimized = false;
		if (optimized){
			compiler_option.SetOptimizationLevel(shaderc_optimization_level_performance);
		}

		auto& shaderData = m_OpenGLSPIRVCache;
		std::filesystem::path cacheDirectory = Utils::GetShaderCacheDirectory();
		shaderData.clear();
		m_OpenGLSourceCode.clear();

		std::filesystem::path shaderPath = m_FilePath;
		const std::string& shaderName = shaderPath.replace_extension().filename().string();

		for (auto&& [stage, spirv] : m_VulkanSPIRVCache)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachePath = cacheDirectory / (shaderName + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachePath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				//data.resize(size * sizeof(uint32_t));		���������ʬһ�¡�����
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL GLSLCompiler(spirv);		//ͨ�������������׼���õ�VulkanԴ���spirv�����opengl��Դ��
				m_OpenGLSourceCode[stage] = GLSLCompiler.compile();	//��Դ����뱾��
				auto& source = m_OpenGLSourceCode[stage];			

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source,		/*shaderName.c_str()*/
															Utils::GLShaderStageToShaderc(stage), m_FilePath.c_str());

				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					TZ_CORE_CRITICAL(module.GetErrorMessage());
					TZ_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachePath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];					/*˵ʵ���Ҳ�֪��Ϊʲôshaderc������uint32_t*/
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		/*
		auto& shaderData = m_OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = false;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		shaderData.clear();
		m_OpenGLSourceCode.clear();
		for (auto&& [stage, spirv] : m_VulkanSPIRV)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_OpenGLSourceCode[stage] = glslCompiler.compile();
				auto& source = m_OpenGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					HZ_CORE_ERROR(module.GetErrorMessage());
					HZ_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
		*/
	}
	void OpenGLShader::CreatePrograme()
	{
		GLuint program = glCreateProgram();
		std::vector<GLuint> ShaderIDs;
		for (auto&& [state, spirv] : m_OpenGLSPIRVCache)
		{
			GLuint shaderID = ShaderIDs.emplace_back(glCreateShader(state));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));	//�ö������ļ�������ɫ��
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);	//��׼�������⻯����ʽ����
			glAttachShader(program, shaderID);		//������ɫ��
		}
		glLinkProgram(program);		//���ӳ���

		/*-------------�����Ⱦ������ɫ��������״̬---------------------------------*/
		int isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(/*maxLength*/1000);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			TZ_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_FilePath, infoLog.data());


			glDeleteProgram(program);
			for (auto ID : ShaderIDs) {		// Don't leak shaders either.
				glDeleteShader(ID);
			}
		}
		for (auto shaderID : ShaderIDs) {		// ���Ǻܶ�Ϊʲô����һ��
			glDetachShader(program, shaderID);
			glDeleteShader(shaderID);
		}

		Renderer_Program_ID = program;

		/*
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_OpenGLSPIRVCache)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			TZ_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_FilePath, infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;
		*/
	}

	void OpenGLShader::Reflect(GLenum stage, std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		TZ_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_FilePath);
		TZ_CORE_TRACE("    {0} uniform buffer",	resources.uniform_buffers.size());	//uniform buffer����
		TZ_CORE_TRACE("    {0} resource",		resources.separate_images.size());	//Դ������(��С��)

		TZ_CORE_TRACE("uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			TZ_CORE_TRACE("  {0}", resource.name);
			TZ_CORE_TRACE("    Size = {0}", bufferSize);
			TZ_CORE_TRACE("    Binding = {0}", binding);
			TZ_CORE_TRACE("    Members = {0}", memberCount);
		}
	}

	/*----------------ͳһ����API------------------------------------------------------------------------*/
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
	/*----------------ͳһ����API------------------------------------------------------------------------*/


	/*----------------ͳһ����ʵ��------------------------------------------------------------------------*/
	/*----------------����ͳһ����-----------------------*/
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
	/*----------------����ͳһ����-----------------------*/
	/*--------------------��int--------------------------*/
	void OpenGLShader::SetUniform1i(const std::string& uni_name, int value)
	{
		glUniform1i(GetUniLocation(uni_name), value);
	}
	/*--------------------int����--------------------------*/
	void OpenGLShader::SetUniform1i_array(const std::string& uni_name, const int* value, const uint32_t count)
	{
		glUniform1iv(GetUniLocation(uni_name), count, value);
	}
	/*--------------------��float------------------------*/
	void OpenGLShader::SetUniform1f(const std::string& uni_name, float value)
	{
		glUniform1f(GetUniLocation(uni_name), value);
	}
	/*-----------------����float����---------------------*/
	void OpenGLShader::SetUniform3f(const std::string& uni_name, float value1, float value2, float value3)
	{
		glUniform3f(GetUniLocation(uni_name), value1, value2, value3);
	}
	void OpenGLShader::SetUniform3f(const std::string& uni_name, const glm::vec3& value)
	{
		glUniform3f(GetUniLocation(uni_name), value.x, value.y, value.z);
	}
	/*------------------����float����--------------------*/
	void OpenGLShader::SetUniform4f(const std::string& uni_name, float value1, float value2, float value3, float value4)
	{
		glUniform4f(GetUniLocation(uni_name), value1, value2, value3, value4);
	}
	void OpenGLShader::SetUniform4f(const std::string& uni_name, const glm::vec4& value)
	{
		glUniform4f(GetUniLocation(uni_name), value.r, value.g, value.b, value.a);
	}
	/*----------------����float����----------------------*/
	void OpenGLShader::SetUniformMat3f(const std::string& uni_name, const glm::mat3& value)
	{
		glUniformMatrix3fv(GetUniLocation(uni_name), 1, GL_FALSE, glm::value_ptr(value));
	}
	/*----------------�Ľ�float����----------------------*/
	void OpenGLShader::SetUniformMat4f(const std::string& uni_name, const glm::mat4& value)
	{
												/*�Ƿ�ת�ã�OpenGL�����������Բ���*/
		glUniformMatrix4fv(GetUniLocation(uni_name), 1, GL_FALSE, glm::value_ptr(value));
	}
	/*-----------------------------------------------------------------------------------------------------------*/

}