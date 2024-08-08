#pragma once

#include "ImpulseCore.h"
#include "Containers/Set.h"

class FSingletonBase
{
	friend extern CORE_API void CleanupSingletons();

protected:
	virtual void OnSDestroy() {}
};

template<typename T>
class TSingleton : public FSingletonBase
{
public:

	template<typename SingletonType>
	friend void DestroySingleton();

	template<typename SingletonType, typename CreationType, typename... ParamTypes>
	friend SingletonType* CreateSingleton(ParamTypes&&... Params);

	static T* Get()
	{
		return s_Instance;
	}

protected:
	TSingleton() = default;
	~TSingleton() = default;
	
private:
	inline static T* s_Instance = nullptr;
	static void Set(T* Instance) { s_Instance = Instance; }
};

namespace IE
{
	namespace Templates
	{
		namespace Private
		{
			extern CORE_API TSet<FSingletonBase*> GSingletonInstances;
		}
	}
}

template<typename SingletonType>
void DestroySingleton()
{
	SingletonType* instance = TSingleton<SingletonType>::Get();
	if (instance != nullptr)
	{
		IE::Templates::Private::GSingletonInstances.Remove(instance);
		delete instance;
	}
	
	TSingleton<SingletonType>::Set(nullptr);
}

template<typename SingletonType, typename CreationType, typename... ParamTypes>
SingletonType* CreateSingleton(ParamTypes&&... Params)
{
	if (SingletonType::Get() == nullptr)
	{
		SingletonType* singleton = new CreationType(Forward<ParamTypes>(Params)...);

		SingletonType::Set(singleton);
		IE::Templates::Private::GSingletonInstances.PushBack(singleton);
	}

	return SingletonType::Get();
}

extern CORE_API void CleanupSingletons();

template<typename T, typename CreationType = T>
class TSSingleton
{
public:
	
	static T* Get()
	{
		static CreationType instance;
		return (T*)&instance;
	}

protected:
	TSSingleton() = default;
	~TSSingleton() = default;
};