#pragma once

template <class T>
class SingletonClass
{
protected:
	SingletonClass<T>() {}
	~SingletonClass<T>() {}
	/*implicit methods exist for the copy constructor and operator= and we want to forbid calling them.*/
	SingletonClass<T>(const SingletonClass<T> &){}
	SingletonClass<T>& operator =(const SingletonClass<T> &){}
public:
	static void CreateInstance()
	{
		if (m_sInstance == nullptr)
			m_sInstance = new T;
	}
	static T * GetInstance()
	{
		return m_sInstance;
	}
	static void DestroyInstance()
	{
		if (m_sInstance)
		{
			delete m_sInstance;
			m_sInstance = nullptr;
		}
	}
	
protected:
	static T * m_sInstance;
};