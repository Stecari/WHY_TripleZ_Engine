#pragma once
#include "TribleZ_Core/Core.h"
#include "entt.hpp"

#include "TribleZ_Core/TimeStep.h"
#include "TribleZ_Core/Render/Editor_Camera.h"

struct b2WorldId;
#ifdef BOX2D_V2_4_CODE_REFERENCE
class b2World;	//����������namespace TribleZ���棬��Ȼ�ͱ����TribleZ::b2World()�ˣ��Ͳ���Box2D�Ķ�����
#endif

namespace TribleZ
{
	class Entity;
	class Scene
	{
	private:
		friend class Entity;	//��Ԫ������ʹ���Լ���private��Ա���Դ�extern
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
		entt::registry m_Registry;  //ע��������ռ�ʵ����������ݺ�ʵ��ID

		uint32_t m_ViewWidth, m_ViewHeight;

		b2WorldId* m_PhysicsWorldID = {}/*b2_nullWorldId*/;

#ifdef BOX2D_V2_4_CODE_REFERENCE
		b2World* m_PhysicsWorld = nullptr;		//���ﴴ��һ��b2World����ᱻ��Ϊ��������ʵ�������Ǵ���һ��b2Worldָ��Ϳ���
#endif


	public:
		Scene();
		~Scene();

		/*OnUpdata���º�����Ϊ�༭ʱ������ʱ������ʱ�����൱�ڰ����˱༭���ϵġ����桱���ߡ����С���ť��
		* �����Ǳ༭ʱ��Ҫ�������Ϊһ�����Ա���Ⱦʵ�壬�����ӾͿ����ڱ༭�����϶��������������������Ҫ��λ�ã�
		* Ȼ���ٵ��"����"����"����"��������ʱ���ӽǽ���ָ�������������Ⱦ�������������ʵ��*/
		void OnUpdataEditor(TimeStep tiemstep, Editor_Camera Edi_Camera);
		void OnUpdataRuntime(TimeStep tiemstep);
		void ResizeView(uint32_t width, uint32_t height);

		void OnRuntimeStop();
		void OnRuntimeStart();

		Entity CreateEntity(const std::string& name = std::string());	//ÿ��ʵ�����һ��entt::entity,��Scene*
		void DeleteEntity(Entity entity);

		//z�����ҷ�����һֱ����Щä�����������ر�ϲ����&������������ü�&�����ҼӲ��˵İ�
		Entity GetPrimaryCameraEntity();

	private:
		/*�������ֶν����������cpp�ļ������÷��Ƚ��٣����Թ�עһ��*/
		template<typename T>
		void OnComponentAdding(Entity entity, T& component);
	};
}