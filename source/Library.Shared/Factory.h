#pragma once
#include <gsl/gsl>
#include "HashMap.h"
#include <type_traits>

namespace FIEAGameEngine
{
	template <typename ClassT>
	class Factory
	{
	public:
		using _HashMapT = HashMap<std::string, std::reference_wrapper<const Factory<ClassT>>>;

	public:

		/// <summary>
		/// Constructor for Factory of Class
		/// </summary>
		Factory() = default;
		virtual ~Factory() = default;
		Factory(const Factory& other) = delete;
		Factory& operator=(const Factory& other) = delete;
		Factory(Factory&& rhs) = delete;
		Factory& operator=(Factory&& rhs) = delete;

	public:

		/// <summary>
		/// Find Concrete Factory of given name
		/// </summary>
		/// <param name="className">Name of class to find</param>
		/// <returns>Pointer to constant Factory of given class | nullptr if Factory not found</returns>
		static const Factory<ClassT>* Find(const std::string& className);

		/// <summary>
		/// Create a new Object of given classname
		/// </summary>
		/// <param name="className">Name of class to create</param>
		/// <returns>Raw Pointer to newly allocated object</returns>
		static gsl::owner<ClassT*> Create(const std::string& className);

		/// <summary>
		/// Create a new object of this class
		/// </summary>
		/// <returns>Raw Pointer to newly allocated object</returns>
		virtual gsl::owner<ClassT*> Create() const = 0;

		/// <summary>
		/// Get Class Name of this object
		/// </summary>
		/// <returns>String containing name of this object</returns>
		virtual const std::string& ClassName() const = 0;

		virtual bool IsGameObject() const = 0;

	protected:
		static void Add(const Factory<ClassT>& factory);
		static void Remove(const Factory<ClassT>& factory);

	private:
		inline static _HashMapT _factoryMap;
	};
}

#include "Factory.inl"

#define DefineFactory(ConcreteT, AbstractT)																					\
class ConcreteT##Factory : public FIEAGameEngine::Factory<AbstractT>														\
{																															\
    public:																													\
        ConcreteT##Factory() { Add(*this); }																				\
        virtual ~ConcreteT##Factory() { Remove(*this); }																	\
        virtual const std::string& ClassName() const override { return _className; }										\
        virtual gsl::owner<ConcreteT*> Create() const override { return new ConcreteT; }									\
		virtual bool IsGameObject() const override { return std::is_base_of<FIEAGameEngine::GameObject, ConcreteT>(); }		\
	private:																												\
		const std::string _className{ #ConcreteT };																			\
}