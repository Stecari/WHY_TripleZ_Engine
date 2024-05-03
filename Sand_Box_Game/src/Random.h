#pragma once

#include "random"

class Random
{
private:
	static std::mt19937 s_RandomEngine;	//自带的随机数引擎
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distruibute;		//随机数生成函数，提供一个随机数引擎就能生成一个<>里指定的类型搞的随机数
public:
	static void Init()
	{					/*随机数赋种*/
		s_RandomEngine.seed(std::random_device()());
		/*上面那句等同下面这一段*/
		//std::random_device F_random;  /*  -> F_random() */
		//unsigned int random = F_random();
		//s_RandomEngine.seed(random);
	}

	static float Float()	//生成一个float型的随机数，小于一？
	{
																/*返回<>中指定的类型的最(大)值*/
		return (float)s_Distruibute(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

};