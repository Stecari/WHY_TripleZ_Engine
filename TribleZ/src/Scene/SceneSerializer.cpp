#include "tzpch.h"
#include "SceneSerializer.h"

//#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#include "Entity.h"
#include "Component.h"

#include "YAMLBatchForGLM.h"	//视屏里是直接打在这里的我觉得太乱就弄了一个文件单独放出来

/*
* 关于yaml的序列化，作用是保存一个场景
* 显然这个h文件和cpp文件都是抄的
* 这个库有按照静态库和按照动态库两种方式链接
* 使用前记得要先检查宏定义是不是已经按照自己想要的链接方式定义了
*/
namespace TribleZ
{
	/*适配glm的运算符号*/
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& value)
	{
		out << YAML::Flow;		//excmple:Translation: [0, 0, 0]
		out << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		:m_Scene(scene)		{}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;	//开始对Entity键值对的映射
		out << YAML::Key << "Entity";
		uint64_t EntityID = entity.GetUniqueID();
		//out << YAML::Value << "15151515151";	//值是这个entity的唯一ID号
		out << YAML::Value << std::to_string(EntityID);	//值是这个entity的唯一ID号

		//开始挨个序列化entity
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;	//开始TagComponent附属键值对映射
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;	//结束TagComponent附属键值对映射
		}

		if (entity.HasComponent<CameraComponent>())
		{
			auto& Cameracomp = entity.GetComponent<CameraComponent>();
			auto& camera = Cameracomp.Camera;
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;/*--------------------------------------------------------------------------CameraComponent---*/
			
			out << YAML::Key << "CameraProps" << YAML::Value;
			out << YAML::BeginMap;/*--------------------------------------------------------------------------CameraProps-------*/
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjType();

			out << YAML::Key << "OrthoGraphicSize" << YAML::Value << camera.GetOrthoGraphicSize();
			out << YAML::Key << "OrthoGraphicNearClip" << YAML::Value << camera.GetOrthoGraphicNearClip();
			out << YAML::Key << "OrthoGraphicFarClip" << YAML::Value << camera.GetOrthoGraphicFarClip();

			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveFOV();
			out << YAML::Key << "PerspectiveNearClip" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFarClip" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::EndMap;/*-----------------------------------------------------------------------------CameraProps---*/

			out << YAML::Key << "Primary_flag" << YAML::Value << Cameracomp.Primary;
			out << YAML::Key << "FixedAspectRatio_flag" << YAML::Value << Cameracomp.FixedAspectRatio;
			
			out << YAML::EndMap;/*-----------------------------------------------------------------------------CameraComponent---*/
		}

		if (entity.HasComponent<TransformComponent>())
		{
			auto& TransComp = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Translation" << YAML::Value << TransComp.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << TransComp.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << TransComp.Scale;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			auto& Color = entity.GetComponent<SpriteRendererComponent>().Color;
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Color" << YAML::Value << Color;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;	//结束对Entity键值对的映射
	}

	void SceneSerializer::Serializer(const std::string& filepath)
	{
		YAML::Emitter yaml_out;
		yaml_out << YAML::BeginMap;	/*---------------------------------------------------------------------开始映射------*/
		yaml_out << YAML::Key << "Scene" << YAML::Value << "Scene's name";		//记录一个键值对
									//一个序列sequence， 本质上就是一个数组
		yaml_out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;/*------------------------------开始序列------*/
		//等价操作
		//yaml_out << YAML::Key << "Scene";
		//yaml_out << YAML::Value << "Scene's name";
		//yaml_out << YAML::Key << "Entity";
		//yaml_out << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.view<entt::entity>().each([&](auto entityID)	//遍历所有entity
		{
			Entity entity = { entityID, m_Scene.get() };
			//假如entity有效，我们就将其序列化
			if (!entity){
				return;
			}
			SerializeEntity(yaml_out, entity);
		});
		yaml_out << YAML::EndSeq;/*-------------------------------------------------------------------------结束序列------*/
		yaml_out << YAML::EndMap;/*-------------------------------------------------------------------------结束映射------*/

		std::ofstream fileout(filepath);
		fileout << yaml_out.c_str();
	}
	void SceneSerializer::SerializerRuntime(const std::string& filepath)
	{
		//未实现
		TZ_CORE_ASSERT(false);
	}
	
	bool SceneSerializer::Deserializer(const std::string& filepath)
	{
		std::fstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();	//rdbuf()将一个文件流的内容存入一个buffer中，然后再将其推入一个文字流中

		YAML::Node data = YAML::Load(strStream.str());
		/*[]用于获取Yaml中的某个节点*/
		if (!data["Scene"]){		//假如一个文件里面找不到Scene的Key，那说明这个文件有问题
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();
		TZ_CORE_TRACE("Deserializer Scene : ‘{0}’", sceneName);

		YAML::Node entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();
				std::string tagname;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent){
					tagname = tagComponent["Tag"].as<std::string>();
				}
				TZ_CORE_TRACE("Deserializer entity[{0}] with ID: {1}", tagname, uuid);

				//m_Scene->CreateEntity(uuid, name);		//后面有空做一个这个
				Entity deserializeEntity = m_Scene->CreateEntity(tagname);

				auto transformComponent= entity["TransformComponent"];
				if (transformComponent)
				{
					//因为我们的Entity在创建时就默认添加了Tag和TransformComponent,所以这里用Get,其他的都是要用Add
					TransformComponent& TransComp = deserializeEntity.GetComponent<TransformComponent>();
					TransComp.Translation = transformComponent["Translation"].as<glm::vec3>();
					TransComp.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					TransComp.Scale = transformComponent["Scale"].as<glm::vec3>();
				}
				
				/*
				* 记录一个可预见的Bug
				* 由于相机的初始化里包括利用Scene的宽和高对缩放比进行计算
				* 而刚创建的Scene
				* 也就是0和0，那会导致RecalculateProjection()出问题
				*/
				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					CameraComponent& CameraComp = deserializeEntity.AddComponent<CameraComponent>();
					
					auto&& cameraProps = cameraComponent["CameraProps"];
					CameraComp.Camera.SetProjectionType((SceneCamera::CameraProjectionType)cameraProps["ProjectionType"].as<int>());
					
					CameraComp.Camera.SetOrthoGraphicSize(cameraProps["OrthoGraphicSize"].as<float>());
					CameraComp.Camera.SetOrthoGraphicNearClip(cameraProps["OrthoGraphicNearClip"].as<float>());
					CameraComp.Camera.SetOrthoGraphicFarClip(cameraProps["OrthoGraphicFarClip"].as<float>());

					CameraComp.Camera.SetPerspectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
					CameraComp.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNearClip"].as<float>());
					CameraComp.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFarClip"].as<float>());

					CameraComp.Primary = cameraComponent["Primary_flag"].as<bool>();
					CameraComp.FixedAspectRatio = cameraComponent["FixedAspectRatio_flag"].as<bool>();

				}

				auto spriteRendererComponent= entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					SpriteRendererComponent& SpriteComp = deserializeEntity.AddComponent<SpriteRendererComponent>();
					SpriteComp.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}

			}
		}
		return true;
	}
	bool SceneSerializer::DeserializerRuntime(const std::string& filepath)
	{
		//未实现
		TZ_CORE_ASSERT(false);
		return false;
	}
}