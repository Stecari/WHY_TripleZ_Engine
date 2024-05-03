#pragma once
#include "tzpch.h"
#include "Core.h"
#include "Layer.h"

namespace TribleZ
{
	class TRIBLEZ_API LayerStack
	{
	private:
		std::vector<Layer*> m_Layers;
		//std::vector<Layer*>::iterator m_LayersInsert;
		unsigned int m_LayersInsertIndex = 0;

	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);							/*         普通层 -> * ->  ******  <- * <-覆盖层     */
		void PushOverLayer(Layer* overlayer);
		void PopLayer(Layer* layer);
		void PopOverLayer(Layer* overlayer);

				
		//inline std::vector<Layer*>::iterator Beginiter() { return m_Layers.begin(); }
		inline std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }/*指向栈顶，第一个元素的位置*/
								/*这俩函数必须叫这个，因为一个class要用for迭代器的话,必须有一个begin()和一个end()给他用*/
		inline std::vector<Layer*>::iterator end() { return m_Layers.end(); }/*指向栈底，最后一个元素的 下一位 位置*/
		//inline std::vector<Layer*>::iterator Enditer() { return m_Layers.end(); }

	};
}