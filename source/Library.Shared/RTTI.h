#pragma once

#include <cstddef>
#include <string>

namespace FIEAGameEngine
{
	class RTTI
	{
	public:
		using IdType = std::size_t;

		RTTI() = default;
		RTTI(const RTTI&) = default;
		RTTI& operator=(const RTTI&) = default;
		RTTI(RTTI&&) noexcept = default;
		RTTI& operator=(RTTI&&) noexcept = default;
		virtual ~RTTI() = default;

		virtual IdType TypeIdInstance() const = 0;
		virtual bool Is(IdType) const
		{
			return false;
		}

		template <typename T>
		T* As()
		{
			return (Is(T::TypeIdClass()) ? reinterpret_cast<T*>(this) : nullptr);
		}

		template <typename T>
		const T* As() const
		{
			return (Is(T::TypeIdClass()) ? reinterpret_cast<const T*>(this) : nullptr);
		}

		virtual std::string ToString() const
		{
			return "RTTI";
		}
		virtual bool Equals(const RTTI* rhs) const
		{
			return this == rhs;
		}
	};
}

#define RTTI_DECLARATIONS(Type)																							\
	public:																												\
		static FIEAGameEngine::RTTI::IdType TypeIdClass();																\
		FIEAGameEngine::RTTI::IdType TypeIdInstance() const override;													\
		bool Is(FIEAGameEngine::RTTI::IdType id) const override;														\
	private:																											\
		static const FIEAGameEngine::RTTI::IdType _typeId;

#define RTTI_DEFINITIONS(Type, ParentType)																				\
	FIEAGameEngine::RTTI::IdType Type::TypeIdClass() { return _typeId; }												\
	FIEAGameEngine::RTTI::IdType Type::TypeIdInstance() const { return TypeIdClass(); }									\
	bool Type::Is(FIEAGameEngine::RTTI::IdType id) const { return (id == _typeId ? true : ParentType::Is(id)); }		\
	const FIEAGameEngine::RTTI::IdType Type::_typeId = reinterpret_cast<FIEAGameEngine::RTTI::IdType>(&Type::_typeId);