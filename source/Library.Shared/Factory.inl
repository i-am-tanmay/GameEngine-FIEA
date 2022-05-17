#pragma once
#include "Factory.h"

namespace FIEAGameEngine
{
	template <typename ClassT>
	void Factory<ClassT>::Add(const Factory<ClassT>& factory)
	{
		if (!((_factoryMap.Insert(factory.ClassName(), factory)).second))
			throw std::runtime_error("Factory Instance already exists");
	}

	template <typename ClassT>
	void Factory<ClassT>::Remove(const Factory<ClassT>& factory)
	{
		_factoryMap.Remove(factory.ClassName());
	}

	template <typename ClassT>
	const Factory<ClassT>* Factory<ClassT>::Find(const std::string& className)
	{
		typename _HashMapT::Iterator itr = _factoryMap.Find(className);
		if (itr != _factoryMap.end())
		{
			const Factory<ClassT>& returnref = itr->second;
			return &returnref;
		}
		return nullptr;
	}

	template <typename ClassT>
	gsl::owner<ClassT*> Factory<ClassT>::Create(const std::string& className)
	{
		const Factory<ClassT>* foundclass = Find(className);
		if (foundclass) return foundclass->Create();
		return nullptr;
	}
}