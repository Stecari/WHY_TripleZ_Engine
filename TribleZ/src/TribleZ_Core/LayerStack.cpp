#include "tzpch.h"
#include "LayerStack.h"

namespace TribleZ
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayersInsertIndex, layer);
		m_LayersInsertIndex++;
	}

	void LayerStack::PushOverLayer(Layer* overlayer)
	{
		m_Layers.emplace_back(overlayer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto pop_iter = std::find(begin(), end(), layer);
		if (pop_iter != end())
		{
			m_Layers.erase(pop_iter);
			m_LayersInsertIndex--;
		}
	}

	void LayerStack::PopOverLayer(Layer* overlayer)		//这个有问题，迭代器循环里不允许修改vector，到时候修一下
	{
		auto pop_iter = std::find(begin(), end(), overlayer);
		if (pop_iter != begin())
		{
			m_Layers.erase(pop_iter);
		}
	}


}