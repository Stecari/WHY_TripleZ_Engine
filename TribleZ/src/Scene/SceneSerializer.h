#pragma once
/*
* 关于yaml的序列化，作用是保存一个场景
* 显然这个h文件和cpp文件都是抄的
* 本质上可能就是fstream的包装类
* 用fstream将各种数据以YAML文件的格式序列化(格式化)
* 记录到一个文件里，然后将各种操作函数包装成这个YAML::类
* 直接调用函数记录数据就可以了
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