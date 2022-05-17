#pragma once
#include <cstdint>
#include <stdexcept>
#include "Datum.h"

namespace FIEAGameEngine
{
	struct AttributeSignature final
	{
		std::string _name;
		DatumTypes _type;
		std::size_t _size;
		std::size_t _offset;

		/// <summary>
		/// Constructor for making a signature
		/// </summary>
		/// <param name="name">Key for entry</param>
		/// <param name="type">Type of entry</param>
		/// <param name="size">Size of data</param>
		/// <param name="offset">Offset memory location from start</param>
		AttributeSignature(std::string&& name, DatumTypes type, std::size_t size, std::size_t offset) : _name{ std::move(name) }, _type{ type }, _size{ size }, _offset{ offset } {}
	};
}