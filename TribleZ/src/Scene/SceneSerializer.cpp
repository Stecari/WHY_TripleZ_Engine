#include "tzpch.h"
#include "SceneSerializer.h"

//#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#include "Entity.h"
#include "Component.h"

#include "YAMLBatchForGLM.h"	//��������ֱ�Ӵ���������Ҿ���̫�Ҿ�Ū��һ���ļ������ų���

/*
* ����yaml�����л��������Ǳ���һ������
* ��Ȼ���h�ļ���cpp�ļ����ǳ���
* ������а��վ�̬��Ͱ��ն�̬�����ַ�ʽ����
* ʹ��ǰ�ǵ�Ҫ�ȼ��궨���ǲ����Ѿ������Լ���Ҫ�����ӷ�ʽ������
*/
namespace TribleZ
{
	/*����glm���������*/
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
		out << YAML::BeginMap;	//��ʼ��Entity��ֵ�Ե�ӳ��
		out << YAML::Key << "Entity";
		uint64_t EntityID = entity.GetUniqueID();
		//out << YAML::Value << "15151515151";	//ֵ�����entity��ΨһID��
		out << YAML::Value << std::to_string(EntityID);	//ֵ�����entity��ΨһID��

		//��ʼ�������л�entity
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;	//��ʼTagComponent������ֵ��ӳ��
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;	//����TagComponent������ֵ��ӳ��
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

		out << YAML::EndMap;	//������Entity��ֵ�Ե�ӳ��
	}

	void SceneSerializer::Serializer(const std::string& filepath)
	{
		YAML::Emitter yaml_out;
		yaml_out << YAML::BeginMap;	/*---------------------------------------------------------------------��ʼӳ��------*/
		yaml_out << YAML::Key << "Scene" << YAML::Value << "Scene's name";		//��¼һ����ֵ��
									//һ������sequence�� �����Ͼ���һ������
		yaml_out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;/*------------------------------��ʼ����------*/
		//�ȼ۲���
		//yaml_out << YAML::Key << "Scene";
		//yaml_out << YAML::Value << "Scene's name";
		//yaml_out << YAML::Key << "Entity";
		//yaml_out << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.view<entt::entity>().each([&](auto entityID)	//��������entity
		{
			Entity entity = { entityID, m_Scene.get() };
			//����entity��Ч�����Ǿͽ������л�
			if (!entity){
				return;
			}
			SerializeEntity(yaml_out, entity);
		});
		yaml_out << YAML::EndSeq;/*-------------------------------------------------------------------------��������------*/
		yaml_out << YAML::EndMap;/*-------------------------------------------------------------------------����ӳ��------*/

		std::ofstream fileout(filepath);
		fileout << yaml_out.c_str();
	}
	void SceneSerializer::SerializerRuntime(const std::string& filepath)
	{
		//δʵ��
		TZ_CORE_ASSERT(false);
	}
	
	bool SceneSerializer::Deserializer(const std::string& filepath)
	{
		std::fstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();	//rdbuf()��һ���ļ��������ݴ���һ��buffer�У�Ȼ���ٽ�������һ����������

		YAML::Node data = YAML::Load(strStream.str());
		/*[]���ڻ�ȡYaml�е�ĳ���ڵ�*/
		if (!data["Scene"]){		//����һ���ļ������Ҳ���Scene��Key����˵������ļ�������
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();
		TZ_CORE_TRACE("Deserializer Scene : ��{0}��", sceneName);

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

				//m_Scene->CreateEntity(uuid, name);		//�����п���һ�����
				Entity deserializeEntity = m_Scene->CreateEntity(tagname);

				auto transformComponent= entity["TransformComponent"];
				if (transformComponent)
				{
					//��Ϊ���ǵ�Entity�ڴ���ʱ��Ĭ�������Tag��TransformComponent,����������Get,�����Ķ���Ҫ��Add
					TransformComponent& TransComp = deserializeEntity.GetComponent<TransformComponent>();
					TransComp.Translation = transformComponent["Translation"].as<glm::vec3>();
					TransComp.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					TransComp.Scale = transformComponent["Scale"].as<glm::vec3>();
				}
				
				/*
				* ��¼һ����Ԥ����Bug
				* ��������ĳ�ʼ�����������Scene�Ŀ�͸߶����űȽ��м���
				* ���մ�����Scene
				* Ҳ����0��0���ǻᵼ��RecalculateProjection()������
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
		//δʵ��
		TZ_CORE_ASSERT(false);
		return false;
	}
}