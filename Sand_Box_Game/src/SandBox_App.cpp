
#include <vector>
#include "TribleZ.h"
/*--------------�����----------------------------------*/

#include "TribleZ_Core/EntryPoint.h"

/*--------------�����----------------------------------*/
//#include <TribleZ.h>  //���Ҳ���ԣ�������Ϊ�˷ֵ����һ���������Ǹ�

#include "SandBox2D.h"




class SandBox : public TribleZ::Application
{
private:


public:			//����SandBox��Application�����࣬�����ڹ���֮ǰ���ȹ���Application
	SandBox()	//ʵ��Ӧ�õĳ�ʼ������Ҫ�õ��Ĳ�ȫ���ƽ���׼����
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new SandBox2D_layer());
		//PushOverlayer(new TribleZ::ImGuiLayer());
	}

	~SandBox()
	{

	}


};

TribleZ::Application* TribleZ::CreatApplication()
{
	return new SandBox;
}
