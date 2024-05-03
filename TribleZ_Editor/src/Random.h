#pragma once

#include "random"

class Random
{
private:
	static std::mt19937 s_RandomEngine;	//�Դ������������
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distruibute;		//��������ɺ������ṩһ������������������һ��<>��ָ�������͸�������
public:
	static void Init()
	{					/*���������*/
		s_RandomEngine.seed(std::random_device()());
		/*�����Ǿ��ͬ������һ��*/
		//std::random_device F_random;  /*  -> F_random() */
		//unsigned int random = F_random();
		//s_RandomEngine.seed(random);
	}

	static float Float()	//����һ��float�͵��������С��һ��
	{
																/*����<>��ָ�������͵���(��)ֵ*/
		return (float)s_Distruibute(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

};