#include "tzpch.h"

#include "Renderer.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace TribleZ
{
	Ref<Shader> Shader::Create(const std::string& filepath )
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::NONE:
			TZ_CORE_ASSERT(false, "RendererAPI::NONE is not surpported!");
			break;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(filepath);
		case RendererAPI::API::DirectX:
			TZ_CORE_ASSERT(false, "RendererAPI::DirectX is not surpported yet!");
			break;
		}


		TZ_CORE_ASSERT(false, "Unkonw Renderer API!");
		return nullptr;

	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertex_source, const std::string& fragment_source)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::NONE:
			TZ_CORE_ASSERT(false, "RendererAPI::NONE is not surpported!");
			break;
		case RendererAPI::API::OpenGL:
			return make_shared<OpenGLShader>(name, vertex_source, fragment_source);
		case RendererAPI::API::DirectX:
			TZ_CORE_ASSERT(false, "RendererAPI::DirectX is not surpported yet!");
			break;
		}


		TZ_CORE_ASSERT(false, "Unkonw Renderer API!");
		return nullptr;

	}

	void ShaderLibrary::Add(const std::string& name,const Ref<Shader>& shader)
	{
		TZ_CORE_ASSERT(IsExist(name), " shader exists!");
		ShaderLib[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto name = shader->GetName();
		Add(name, shader);
	}

	TribleZ::Ref<TribleZ::Shader> ShaderLibrary::LoadShader(const std::string& name, const std::string& Filepath)
	{
		auto shader = Shader::Create(Filepath);
		Add(name, shader);
		return shader;
	}

	TribleZ::Ref<TribleZ::Shader> ShaderLibrary::LoadShader(const std::string& Filepath)
	{
		auto shader = Shader::Create(Filepath);
		Add(shader);
		return shader;
	}
	TribleZ::Ref<TribleZ::Shader> ShaderLibrary::GetShader(const std::string& name)
	{
		TZ_CORE_ASSERT(!IsExist(name), " shader not exists!");
		return ShaderLib[name];
	}

	bool ShaderLibrary::IsExist(const std::string& name)
	{
		return ShaderLib.find(name) == ShaderLib.end();
	}
}