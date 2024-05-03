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

		void PushLayer(Layer* layer);							/*         ��ͨ�� -> * ->  ******  <- * <-���ǲ�     */
		void PushOverLayer(Layer* overlayer);
		void PopLayer(Layer* layer);
		void PopOverLayer(Layer* overlayer);

				
		//inline std::vector<Layer*>::iterator Beginiter() { return m_Layers.begin(); }
		inline std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }/*ָ��ջ������һ��Ԫ�ص�λ��*/
								/*��������������������Ϊһ��classҪ��for�������Ļ�,������һ��begin()��һ��end()������*/
		inline std::vector<Layer*>::iterator end() { return m_Layers.end(); }/*ָ��ջ�ף����һ��Ԫ�ص� ��һλ λ��*/
		//inline std::vector<Layer*>::iterator Enditer() { return m_Layers.end(); }

	};
}