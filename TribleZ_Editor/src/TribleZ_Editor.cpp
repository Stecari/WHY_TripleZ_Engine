#include "TribleZ.h"
/*--------------�����----------------------------------*/

#include "TribleZ_Core/EntryPoint.h"

/*--------------�����----------------------------------*/
//#include <TribleZ.h>  //���Ҳ���ԣ�������Ϊ�˷ֵ����һ���������Ǹ�
#include "Editor_Layer.h"



namespace TribleZ
{
	class TribleZ_Editor : public Application
	{
	private:


	public:			//����SandBox��Application�����࣬�����ڹ���֮ǰ���ȹ���Application
		TribleZ_Editor()	//ʵ��Ӧ�õĳ�ʼ������Ҫ�õ��Ĳ�ȫ���ƽ���׼����
			:Application("TribleZ_Editor")
		{
			PushLayer(new Editor_Layer());
		}

		~TribleZ_Editor()
		{

		}


	};

	Application* CreatApplication(ApplicationCommandLineArgs Args)
	{
		return new TribleZ_Editor;
	}

}
