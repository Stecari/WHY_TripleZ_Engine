#pragma once
/*
* ����yaml�����л��������Ǳ���һ������
* ��Ȼ���h�ļ���cpp�ļ����ǳ���
* �����Ͽ��ܾ���fstream�İ�װ��
* ��fstream������������YAML�ļ��ĸ�ʽ���л�(��ʽ��)
* ��¼��һ���ļ��Ȼ�󽫸��ֲ���������װ�����YAML::��
* ֱ�ӵ��ú�����¼���ݾͿ�����
*/
#include "Scene.h"


namespace TribleZ
{
	class SceneSerializer
	{
	private:
		Ref<Scene> m_Scene;

	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serializer(const std::string& filepath);
		void SerializerRuntime(const std::string& filepath);

		bool Deserializer(const std::string& filepath);
		bool DeserializerRuntime(const std::string& filepath);

	};
}