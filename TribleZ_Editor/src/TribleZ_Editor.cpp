#include "TribleZ.h"
/*--------------进入点----------------------------------*/

#include "TribleZ_Core/EntryPoint.h"

/*--------------进入点----------------------------------*/
//#include <TribleZ.h>  //这个也可以，但是我为了分得清楚一点用上面那个
#include "Editor_Layer.h"



namespace TribleZ
{
	class TribleZ_Editor : public Application
	{
	private:


	public:			//由于SandBox是Application的子类，所以在构造之前会先构造Application
		TribleZ_Editor()	//实际应用的初始化，把要用到的层全部推进来准备好
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
