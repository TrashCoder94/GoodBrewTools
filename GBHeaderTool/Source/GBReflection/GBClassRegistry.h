#pragma once

#include <assert.h>
#include <map>
#include <string>

namespace GB
{
	template<typename T> void* Constructor() { return (void*)new T(); }
	typedef void* (*Constructor_T)();
	typedef std::map<std::string, Constructor_T> RegisteredClassMap;

	class ClassRegistry
	{
	public:
		const RegisteredClassMap& GetClassMap() { return m_Classes; };

		template<typename T>
		void Register(const std::string& name)
		{
			if (Get().m_Classes.find(name) == Get().m_Classes.end())
			{
				std::cout << "Registering class " << name << " with ClassRegistry" << std::endl;
				Get().m_Classes.insert(std::make_pair(name, &GB::Constructor<T>));
			}
		}

		void* Construct(const std::string name)
		{
			RegisteredClassMap::iterator it = Get().m_Classes.find(name);
			if (it == Get().m_Classes.end())
			{
				assert(false);
				return nullptr;
			}
			return it->second();
		}

		static ClassRegistry& Get()
		{
			static ClassRegistry classRegistry = ClassRegistry();
			return classRegistry;
		}

	private:
		RegisteredClassMap m_Classes;
	};
}
