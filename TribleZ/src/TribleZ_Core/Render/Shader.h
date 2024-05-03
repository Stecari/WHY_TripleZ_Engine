#pragma once
#include "TribleZ_Core/Core.h"
#include "string"

#include "glm/glm.hpp"

namespace TribleZ
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const std::string& GetName() const = 0;

		virtual void SetInt(const std::string& name, const int& num) = 0;
		virtual void SetIntArray(const std::string& name, const int* value, const uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertex_source, const std::string& fragment_source);

	};

	class ShaderLibrary
	{
	private:
		std::unordered_map<std::string, Ref<Shader>> ShaderLib;
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);

		Ref<Shader> LoadShader(const std::string& Filepath);
		Ref<Shader> LoadShader(const std::string& name, const std::string& Filepath);

		Ref<Shader> GetShader(const std::string& name);

		bool IsExist(const std::string& name);
	};
}
