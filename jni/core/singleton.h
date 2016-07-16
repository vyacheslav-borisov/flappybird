#ifndef PEGAS_SINGLETON_H
#define PEGAS_SINGLETON_H
#pragma once

namespace pegas
{
	template <class T>
	class Singleton
	{
	public:
		Singleton(T& obj)
		{
			if(instance) throw;
			instance = &obj;
		};
		
		~Singleton()
		{
			if(!instance) throw;
			instance = 0;
		};
		
		static T& getInstance()
		{
			if(!instance) throw;
			return *instance;
		};
	private:
		static T* instance;
		
		Singleton(const Singleton& src);
		Singleton& operator=(const Singleton& src);
	};

	template <class T>  T* Singleton<T>::instance = NULL;

}

#endif //PEGAS_SINGLETON_H



