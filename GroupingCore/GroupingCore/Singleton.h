#pragma once

#include "framework.h"
#include <cstddef>


template <typename T>
class GROUPINGCORE_API Singleton
{
protected:
	static T* m_Instance;

public:
	static T* GetInstance()
	{
		if(m_Instance == NULL)
		{
			m_Instance = new T();
		}
		
		return m_Instance;
	}
};

template <typename T>
T* Singleton<T>::m_Instance = NULL;	
