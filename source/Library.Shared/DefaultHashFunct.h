#pragma once
#include <cstdint>
#include <stdexcept>
#include <functional>

namespace FIEAGameEngine
{
	template<typename T>
	struct DefaultHashFunct final
	{
		std::size_t ByteSum(const std::uint8_t* byteArray, std::size_t size) const
		{
			std::size_t hash = 79;

			for (std::size_t i = 0; i < size; ++i) hash += byteArray[i];

			return hash;
		}

		std::size_t operator()(const T& key) const
		{
			return ByteSum(reinterpret_cast<const std::uint8_t*>(&key), sizeof(T));
		}
	};

	template<typename T>
	struct DefaultHashFunct<T*> final
	{
		std::size_t ByteSum(const std::uint8_t* byteArray, std::size_t size) const
		{
			std::size_t hash = 79;

			for (std::size_t i = 0; i < size; ++i) hash += byteArray[i];

			return hash;
		}

		std::size_t operator()(const T* key) const
		{
			return ByteSum(reinterpret_cast<const std::uint8_t*>(key), sizeof(T));
		}
	};

	template<>
	struct DefaultHashFunct<std::string> final
	{
		std::size_t operator()(const std::string& key) const
		{
			std::size_t hash = 79;

			for (const char& c : key)
			{
				hash += c;
			}

			return hash;
		}
	};

	template<>
	struct DefaultHashFunct<char*> final
	{
		std::size_t operator()(const char* key) const
		{
			std::size_t hash = 79;

			std::size_t i = 0;
			while (key[i] != '\0')
			{
				hash += key[i++];
			}

			return hash;
		}
	};
}