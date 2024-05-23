#pragma once
#include "tzpch.h"

#include "TribleZ_Core/Render/Shader.h"
#include "glm/glm.hpp"
//#include "Camera.h"

//����Ҫ��취����
typedef unsigned int GLenum;
typedef GLenum GLShaderType;

namespace TribleZ
{

	struct Shader_Program_source
	{
		std::string VERTEX_SOURCE;
		std::string FRAGMENT_SOURCE;

	};

	class OpenGLShader : public Shader
	{
	private:
		unsigned int Renderer_Program_ID;

		std::unordered_map<std::string, int> UniNameMap;

		std::string m_Name;
		std::string m_FilePath;

		//���������ƻ���
		std::unordered_map<GLenum, std::vector<uint32_t>> m_VulkanSPIRVCache;
		std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRVCache;	

		//����ɵ�OpenGLԴ��
		std::unordered_map<GLenum, std::string>	m_OpenGLSourceCode;

	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertex_source, const std::string& fragment_source);
		~OpenGLShader();

		void CompileShader(const std::unordered_map<GLenum, std::string>& source_map);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& src_code);

		void CompiledOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& source_map);				  
		void CompiledOrGetOpenGLBinaries();																			  
		void CreatePrograme();																						  
		void Reflect(/*GLShadertype*/ GLenum stage, std::vector<uint32_t>& shaderData);								  

	public:
		virtual const std::string& GetName() const override { return m_Name; }

		/*----------------ͳһ����API------------------------------------------------------------------------*/
		virtual void SetInt(const std::string& name, const int& num) override;
		virtual void SetIntArray(const std::string& name, const int* value, const uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) override;
		/*----------------ͳһ����API------------------------------------------------------------------------*/

		/*----------------ͳһ����ʵ��------------------------------------------------------------------------*/
		int GetUniLocation(const std::string& uni_name);

		void SetUniform1i(const std::string& uni_name, int value);
		void SetUniform1i_array(const std::string& name, const int* value, const uint32_t count);
		void SetUniform1f(const std::string& uni_name, float value);
		void SetUniform3f(const std::string& uni_name, float value1, float value2, float value3);
		void SetUniform3f(const std::string& uni_name, const glm::vec3& value);
		void SetUniform4f(const std::string& uni_name, float value1, float value2, float value3, float value4);
		void SetUniform4f(const std::string& uni_name, const glm::vec4& value);
		void SetUniformMat3f(const std::string& uni_name, const glm::mat3& value);
		void SetUniformMat4f(const std::string& uni_name, const glm::mat4& value);
		/*----------------ͳһ����ʵ��------------------------------------------------------------------------*/

		void Bind() const override;
		void UnBind() const override;


	};
}
