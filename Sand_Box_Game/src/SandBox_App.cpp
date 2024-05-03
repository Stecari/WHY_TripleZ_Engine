
#include <vector>
#include "TribleZ.h"
/*--------------进入点----------------------------------*/

#include "TribleZ_Core/EntryPoint.h"

/*--------------进入点----------------------------------*/
//#include <TribleZ.h>  //这个也可以，但是我为了分得清楚一点用上面那个

#include "SandBox2D.h"




class SandBox : public TribleZ::Application
{
private:


public:			//由于SandBox是Application的子类，所以在构造之前会先构造Application
	SandBox()	//实际应用的初始化，把要用到的层全部推进来准备好
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
