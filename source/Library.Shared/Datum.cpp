#include "pch.h"
#include "RTTI.h"
#include "Datum.h"
#include "Scope.h"

namespace FIEAGameEngine
{
	const typename Datum::size_type Datum::DatumTypeSizes[] =
	{
		0,							// UNKNOWN
		sizeof(std::int32_t),		// Integer
		sizeof(float),				// Float
		sizeof(glm::vec4),			// Vector
		sizeof(glm::mat4),			// Matrix
		sizeof(std::string),		// String
		sizeof(RTTI**),				// Pointer
		sizeof(Scope**),			// Scope
	};

	const typename HashMap<std::string, DatumTypes> Datum::DatumStringToType =
	{
		{"integer"s, DatumTypes::Integer},
		{"float"s, DatumTypes::Float},
		{"vec4"s, DatumTypes::Vector},
		{"mat4x4"s, DatumTypes::Matrix},
		{"string"s, DatumTypes::String},
		{"pointer"s, DatumTypes::Pointer},
		{"scope"s, DatumTypes::ScopeTable}
	};

	Datum::Datum(DatumTypes type, size_type capacity) :
		_type{ type }
	{
		if (capacity > 0) Reserve(capacity);
	}

	Datum::~Datum()
	{
		if (_isExternalStorage) return;

		if (_type == DatumTypes::String)
		{
			while (_size > 0u) _data._string[--_size].~basic_string();
		}

		free(_data._voidp);
	}

	Datum::Datum(const Datum& other) :
		_type{ other._type }, _data{ nullptr }, _incrementFunc{ other._incrementFunc }, _isExternalStorage{ other._isExternalStorage }
	{
		if (_isExternalStorage)
		{
			SetStorageExternal(other._data._voidp, _type, other._size);
		}
		else if (_type != DatumTypes::Unknown)
		{
			Reserve(other._capacity);

			if (_type == DatumTypes::String)
			{
				for (size_type i = 0; i < other._size; ++i)
				{
					new(_data._string + i) std::string{ other._data._string[i] };
				}
			}
			else
			{
				std::memcpy(_data._voidp, other._data._voidp, other._size * DatumTypeSizes[static_cast<size_type>(_type)]);
			}

			_size = other._size;

		}
	}

	Datum::Datum(const std::int32_t data, size_type capacity) :
		_type{ DatumTypes::Integer }, _data{ nullptr }, _size{ 1u }
	{
		Reserve(std::max<size_type>(capacity, 1u));
		_data._int[0] = data;
	}

	Datum::Datum(const float data, size_type capacity) :
		_type{ DatumTypes::Float }, _data{ nullptr }, _size{ 1u }
	{
		Reserve(std::max<size_type>(capacity, 1u));
		_data._float[0] = data;
	}

	Datum::Datum(const glm::vec4& data, size_type capacity) :
		_type{ DatumTypes::Vector }, _data{ nullptr }, _size{ 1u }
	{
		Reserve(std::max<size_type>(capacity, 1u));
		_data._vec[0] = data;
	}

	Datum::Datum(const glm::mat4& data, size_type capacity) :
		_type{ DatumTypes::Matrix }, _data{ nullptr }, _size{ 1u }
	{
		Reserve(std::max<size_type>(capacity, 1u));
		_data._mat[0] = data;
	}

	Datum::Datum(const std::string& data, size_type capacity) :
		_type{ DatumTypes::String }, _data{ nullptr }, _size{ 1u }
	{
		Reserve(std::max<size_type>(capacity, 1u));
		new(_data._string)std::string{ data };
	}

	Datum::Datum(std::string&& data, size_type capacity) :
		_type{ DatumTypes::String }, _data{ nullptr }, _size{ 1u }
	{
		Reserve(std::max<size_type>(capacity, 1u));
		new(_data._string)std::string{ std::move(data) };
	}

	Datum::Datum(RTTI* data, size_type capacity) :
		_type{ DatumTypes::Pointer }, _data{ nullptr }, _size{ 1u }
	{
		Reserve(std::max<size_type>(capacity, 1u));
		_data._rtti[0] = data;
	}

	Datum::Datum(Scope& data, size_type capacity) :
		_type{ DatumTypes::ScopeTable }, _data{ nullptr }, _size{ 1u }
	{
		Reserve(std::max<size_type>(capacity, 1u));
		_data._scope[0] = &data;
	}

	Datum::Datum(Datum&& rhs) noexcept :
		_isExternalStorage{ rhs._isExternalStorage }, _type{ rhs._type }, _data{ rhs._data }, _size{ rhs._size }, _capacity{ rhs._capacity }, _incrementFunc{ rhs._incrementFunc }
	{
		rhs._data._voidp = nullptr;
		rhs._size = rhs._capacity = 0;
		rhs._type = DatumTypes::Unknown;
		rhs._incrementFunc = DefaultIncrementFunct{};
		rhs._isExternalStorage = false;
	}

	Datum& Datum::operator=(const Datum& other)
	{
		if (this != &other)
		{
			_type = other._type;
			_incrementFunc = other._incrementFunc;

			if (other._isExternalStorage)
			{
				SetStorageExternal(other._data._voidp, other._type, other._size);
			}
			else
			{
				if (_isExternalStorage)
				{
					_data._voidp = nullptr;
					void* alloc = std::realloc(_data._voidp, other._size * DatumTypeSizes[static_cast<size_type>(_type)]);
					if (alloc)
					{
						_data._voidp = alloc;
						_capacity = other._size;
						_size = other._size;

						if (_type == DatumTypes::String)
						{
							for (size_type i = 0; i < _size; ++i) new(_data._string + i)std::string{ other._data._string[i] };
						}
						else
						{
							std::memcpy(_data._voidp, other._data._voidp, _size * DatumTypeSizes[static_cast<size_type>(_type)]);
						}

						_isExternalStorage = false;
					}
				}
				else
				{
					if (_capacity >= other._size) ReSize(other._size);
					else Reserve(other._size);

					if (_type == DatumTypes::String)
					{
						for (size_type i = 0; i < _size; ++i) _data._string[i] = other._data._string[i];
						for (size_type i = _size; i < other._size; ++i) new(_data._string + i)std::string{ other._data._string[i] };
					}
					else
					{
						std::memcpy(_data._voidp, other._data._voidp, other._size * DatumTypeSizes[static_cast<size_type>(_type)]);
					}

					_size = other._size;
				}
			}
		}

		return *this;
	}

	Datum& Datum::operator=(Datum&& rhs) noexcept
	{
		if (this != &rhs)
		{
			if (!_isExternalStorage)
			{
				Clear();
				free(_data._voidp);
			}

			_isExternalStorage = rhs._isExternalStorage;
			_type = rhs._type;
			_data = rhs._data;
			_size = rhs._size;
			_capacity = rhs._capacity;
			_incrementFunc = rhs._incrementFunc;

			rhs._data._voidp = nullptr;
			rhs._size = rhs._capacity = 0;
			rhs._type = DatumTypes::Unknown;
			rhs._incrementFunc = DefaultIncrementFunct{};
			rhs._isExternalStorage = false;
		}

		return *this;
	}

	Datum& Datum::operator=(const std::int32_t data)
	{
		if (!_isExternalStorage && (_type == DatumTypes::Integer || _type == DatumTypes::Unknown))
		{
			_type = DatumTypes::Integer;

			if (_size > 0)
			{
				_data._int[0] = data;
				_size = 1;
			}
			else
			{
				_size = 1;
				if (_capacity > 0) ShrinkToFit();
				else Reserve(1u);
				_data._int[0] = data;
			}
		}
		else if (_isExternalStorage && _type == DatumTypes::Integer)
		{
			_data._int[0] = data;
		}
		else
		{
			throw std::runtime_error("Invalid Assignment Type");
		}

		return *this;
	}

	Datum& Datum::operator=(const float data)
	{
		if (!_isExternalStorage && (_type == DatumTypes::Float || _type == DatumTypes::Unknown))
		{
			_type = DatumTypes::Float;

			if (_size > 0)
			{
				_data._float[0] = data;
				_size = 1;
			}
			else
			{
				_size = 1;
				if (_capacity > 0) ShrinkToFit();
				else Reserve(1u);
				_data._float[0] = data;
			}
		}
		else if (_isExternalStorage && _type == DatumTypes::Float)
		{
			_data._float[0] = data;
		}
		else
		{
			throw std::runtime_error("Invalid Assignment Type");
		}

		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& data)
	{
		if (!_isExternalStorage && (_type == DatumTypes::Vector || _type == DatumTypes::Unknown))
		{
			_type = DatumTypes::Vector;

			if (_size > 0)
			{
				_data._vec[0] = data;
				_size = 1;
			}
			else
			{
				_size = 1;
				if (_capacity > 0) ShrinkToFit();
				else Reserve(1u);
				_data._vec[0] = data;
			}
		}
		else if (_isExternalStorage && _type == DatumTypes::Vector)
		{
			_data._vec[0] = data;
		}
		else
		{
			throw std::runtime_error("Invalid Assignment Type");
		}

		return *this;
	}

	Datum& Datum::operator=(const glm::mat4& data)
	{
		if (!_isExternalStorage && (_type == DatumTypes::Matrix || _type == DatumTypes::Unknown))
		{
			_type = DatumTypes::Matrix;

			if (_size > 0)
			{
				_data._mat[0] = data;
				_size = 1;
			}
			else
			{
				_size = 1;
				if (_capacity > 0) ShrinkToFit();
				else Reserve(1u);
				_data._mat[0] = data;
			}
		}
		else if (_isExternalStorage && _type == DatumTypes::Matrix)
		{
			_data._mat[0] = data;
		}
		else
		{
			throw std::runtime_error("Invalid Assignment Type");
		}

		return *this;
	}

	Datum& Datum::operator=(const std::string& data)
	{
		if (!_isExternalStorage && (_type == DatumTypes::String || _type == DatumTypes::Unknown))
		{
			_type = DatumTypes::String;

			if (_size > 0)
			{
				while (--_size >= 1) _data._string[_size].~basic_string();
				_size = 1;
				_data._string[0] = data;
			}
			else
			{
				_size = 1;
				if (_capacity > 0) ShrinkToFit();
				else Reserve(1u);
				new(_data._string)std::string{ data };
			}
		}
		else if (_isExternalStorage && _type == DatumTypes::String)
		{
			_data._string[0] = data;
		}
		else
		{
			throw std::runtime_error("Invalid Assignment Type");
		}

		return *this;
	}

	Datum& Datum::operator=(std::string&& data)
	{
		if (!_isExternalStorage && (_type == DatumTypes::String || _type == DatumTypes::Unknown))
		{
			_type = DatumTypes::String;

			if (_size > 0)
			{
				while (--_size >= 1) _data._string[_size].~basic_string();
				_size = 1;
				_data._string[0] = std::move(data);
			}
			else
			{
				_size = 1;
				if (_capacity > 0) ShrinkToFit();
				else Reserve(1u);
				new(_data._string)std::string{ std::move(data) };
			}
		}
		else if (_isExternalStorage && _type == DatumTypes::String)
		{
			_data._string[0] = std::move(data);
		}
		else
		{
			throw std::runtime_error("Invalid Assignment Type");
		}

		return *this;
	}

	Datum& Datum::operator=(RTTI* data)
	{
		if (!_isExternalStorage && (_type == DatumTypes::Pointer || _type == DatumTypes::Unknown))
		{
			_type = DatumTypes::Pointer;

			if (_size > 0)
			{
				_data._rtti[0] = data;
				_size = 1;
			}
			else
			{
				_size = 1;
				if (_capacity > 0) ShrinkToFit();
				else Reserve(1u);
				_data._rtti[0] = data;
			}
		}
		else if (_isExternalStorage && _type == DatumTypes::Pointer)
		{
			_data._rtti[0] = data;
		}
		else
		{
			throw std::runtime_error("Invalid Assignment Type");
		}

		return *this;
	}

	bool Datum::operator==(const Datum& other) const
	{
		if (this == &other) return true;

		bool isEqual = false;

		if (_type == other._type && _size == other._size)
		{
			if (_type == DatumTypes::String)
			{
				isEqual = true;

				for (size_type i = 0; i < _size; ++i)
				{
					if (std::strcmp(_data._string[i].c_str(), other._data._string[i].c_str()) != 0)
					{
						isEqual = false;
						break;
					}
				}
			}
			else if (_type == DatumTypes::Pointer)
			{
				isEqual = true;

				for (size_type i = 0; i < _size; ++i)
				{
					if (!_data._rtti[i]->Equals(other._data._rtti[i]))
					{
						isEqual = false;
						break;
					}
				}
			}
			else if (_type == DatumTypes::ScopeTable)
			{
				isEqual = true;

				for (size_type i = 0; i < _size; ++i)
				{
					if (!_data._scope[i]->Equals(other._data._scope[i]))
					{
						isEqual = false;
						break;
					}
				}
			}
			else
			{
				isEqual = (std::memcmp(_data._voidp, other._data._voidp, _size * DatumTypeSizes[static_cast<size_type>(_type)]) == 0);
			}
		}

		return isEqual;
	}

	bool Datum::operator==(const std::int32_t value) const
	{
		return ((_size == 1) && (_type == DatumTypes::Integer) && (_data._int[0] == value));
	}

	bool Datum::operator==(const float value) const
	{
		return ((_size == 1) && (_type == DatumTypes::Float) && (_data._float[0] == value));
	}

	bool Datum::operator==(const glm::vec4& value) const
	{
		return ((_size == 1) && (_type == DatumTypes::Vector) && (_data._vec[0] == value));
	}

	bool Datum::operator==(const glm::mat4& value) const
	{
		return ((_size == 1) && (_type == DatumTypes::Matrix) && (_data._mat[0] == value));
	}

	bool Datum::operator==(const std::string& value) const
	{
		return ((_size == 1) && (_type == DatumTypes::String) && (std::strcmp(_data._string[0].c_str(), value.c_str()) == 0));
	}

	bool Datum::operator==(RTTI* value) const
	{
		return ((_size == 1) && (_type == DatumTypes::Pointer) && (_data._rtti[0]->Equals(value)));
	}

	bool Datum::operator==(Scope* value) const
	{
		return ((_size == 1) && (_type == DatumTypes::Pointer) && (_data._scope[0]->Equals(value)));
	}

	bool Datum::operator!=(const Datum& other) const
	{
		return !operator==(other);
	}

	bool Datum::operator!=(const std::int32_t value) const
	{
		return !operator==(value);
	}

	bool Datum::operator!=(const float value) const
	{
		return !operator==(value);
	}

	bool Datum::operator!=(const glm::vec4& value) const
	{
		return !operator==(value);
	}

	bool Datum::operator!=(const glm::mat4& value) const
	{
		return !operator==(value);
	}

	bool Datum::operator!=(const std::string& value) const
	{
		return !operator==(value);
	}

	bool Datum::operator!=(RTTI* value) const
	{
		return !operator==(value);
	}

	bool Datum::operator!=(Scope* value) const
	{
		return !operator==(value);
	}

	void Datum::Reserve(size_type capacity)
	{
		if (_type == DatumTypes::Unknown) throw std::runtime_error("Invalid Type");
		if (_isExternalStorage) throw std::runtime_error("Cannot Modify External Storage");

		if (capacity > _capacity)
		{
			void* alloc = std::realloc(_data._voidp, capacity * DatumTypeSizes[static_cast<size_type>(_type)]);
			if (alloc)
			{
				_data._voidp = alloc;
				_capacity = capacity;
			}
			else
			{
				throw std::runtime_error("Failed to Allocate Memory");
			}
		}
	}

	void Datum::ReSize(size_type capacity)
	{
		if (_type == DatumTypes::Unknown) throw std::runtime_error("Invalid Type");
		if (_isExternalStorage) throw std::runtime_error("Cannot Modify External Storage");

		if (capacity >= _capacity)
		{
			size_type old_cap = _capacity;
			Reserve(capacity);
			_size = capacity;

			if (_type == DatumTypes::String)
			{
				while (capacity > old_cap) new(_data._string + (--capacity))std::string();
			}
		}
		else
		{
			_size = capacity;

			if (_type == DatumTypes::String)
			{
				while (_capacity > capacity) _data._string[capacity++].~basic_string();
			}

			ShrinkToFit();
		}
	}

	void Datum::Clear()
	{
		if (_isExternalStorage) throw std::runtime_error("Cannot Modify External Storage");

		if (_type == DatumTypes::String)
		{
			while (_size > 0u) _data._string[--_size].~basic_string();
		}

		_size = 0u;
	}

	void Datum::ShrinkToFit()
	{
		if (_isExternalStorage) throw std::runtime_error("Cannot Modify External Storage");

		if (_size == 0u)
		{
			free(_data._voidp);
			_data._voidp = nullptr;
			_capacity = 0u;
		}
		else
		{
			void* alloc = std::realloc(_data._voidp, _size * DatumTypeSizes[static_cast<size_type>(_type)]);
			if (alloc)
			{
				_data._voidp = alloc;
				_capacity = _size;
			}
			else
			{
				throw std::runtime_error("Failed to Allocate Memory");
			}
		}
	}

	void Datum::Set(const std::int32_t data, size_type index)
	{
		if (_type == DatumTypes::Unknown && !_isExternalStorage && _capacity == 0 && _size == 0 && index == 0)
		{
			_type = DatumTypes::Integer;
			Reserve(std::max<size_type>(_incrementFunc(_size, _capacity), _capacity + 1));
			++_size;
		}
		else if (index >= _size) throw std::out_of_range("Index Out of Range");
		else if (_type != DatumTypes::Integer) throw std::runtime_error("Invalid Type");
		
		_data._int[index] = data;
	}

	void Datum::Set(const float data, size_type index)
	{
		if (_type == DatumTypes::Unknown && !_isExternalStorage && _capacity == 0 && _size == 0 && index == 0)
		{
			_type = DatumTypes::Float;
			Reserve(std::max<size_type>(_incrementFunc(_size, _capacity), _capacity + 1));
			++_size;
		}
		else if (index >= _size) throw std::out_of_range("Index Out of Range");
		else if (_type != DatumTypes::Float) throw std::runtime_error("Invalid Type");

		_data._float[index] = data;
	}

	void Datum::Set(const glm::vec4& data, size_type index)
	{
		if (_type == DatumTypes::Unknown && !_isExternalStorage && _capacity == 0 && _size == 0 && index == 0)
		{
			_type = DatumTypes::Vector;
			Reserve(std::max<size_type>(_incrementFunc(_size, _capacity), _capacity + 1));
			++_size;
		}
		else if (index >= _size) throw std::out_of_range("Index Out of Range");
		else if (_type != DatumTypes::Vector) throw std::runtime_error("Invalid Type");

		_data._vec[index] = data;
	}

	void Datum::Set(const glm::mat4& data, size_type index)
	{
		if (_type == DatumTypes::Unknown && !_isExternalStorage && _capacity == 0 && _size == 0 && index == 0)
		{
			_type = DatumTypes::Matrix;
			Reserve(std::max<size_type>(_incrementFunc(_size, _capacity), _capacity + 1));
			++_size;
		}
		else if (index >= _size) throw std::out_of_range("Index Out of Range");
		else if (_type != DatumTypes::Matrix) throw std::runtime_error("Invalid Type");

		_data._mat[index] = data;
	}

	void Datum::Set(const std::string& data, size_type index)
	{
		if (_type == DatumTypes::Unknown && !_isExternalStorage && _capacity == 0 && _size == 0 && index == 0)
		{
			_type = DatumTypes::String;
			Reserve(std::max<size_type>(_incrementFunc(_size, _capacity), _capacity + 1));
			new(_data._string)std::string();
			++_size;
		}
		else if (index >= _size) throw std::out_of_range("Index Out of Range");
		else if (_type != DatumTypes::String) throw std::runtime_error("Invalid Type");

		_data._string[index] = data;
	}

	void Datum::Set(RTTI* data, size_type index)
	{
		if (_type == DatumTypes::Unknown && !_isExternalStorage && _capacity == 0 && _size == 0 && index == 0)
		{
			_type = DatumTypes::Pointer;
			Reserve(std::max<size_type>(_incrementFunc(_size, _capacity), _capacity + 1));
			++_size;
		}
		else if (index >= _size) throw std::out_of_range("Index Out of Range");
		else if (_type != DatumTypes::Pointer) throw std::runtime_error("Invalid Type");

		_data._rtti[index] = data;
	}

	void Datum::Set(Scope& data, size_type index)
	{
		if (_type == DatumTypes::Unknown && !_isExternalStorage && _capacity == 0 && _size == 0 && index == 0)
		{
			_type = DatumTypes::ScopeTable;
			Reserve(std::max<size_type>(_incrementFunc(_size, _capacity), _capacity + 1));
			++_size;
		}
		else if (index >= _size) throw std::out_of_range("Index Out of Range");
		else if (_type != DatumTypes::ScopeTable) throw std::runtime_error("Invalid Type");

		_data._scope[index] = &data;
	}

	bool Datum::PushBackFromString(const std::string& str)
	{
		bool success = false;
		try
		{
			switch (_type)
			{
			case DatumTypes::Integer:

				PushBack(std::stoi(str));
				success = true;

				break;

			case DatumTypes::Float:

				PushBack(std::stof(str));
				success = true;

				break;

			case DatumTypes::Vector:
			{
				float v[4];
				bool gotallvalues = (sscanf_s(str.c_str(), "vec4(%f, %f, %f, %f)", &v[0], &v[1], &v[2], &v[3]) == 4);
				if (gotallvalues) PushBack(glm::vec4{ v[0], v[1], v[2], v[3] });
				success = gotallvalues;
			}
			break;

			case DatumTypes::Matrix:
			{
				float m[16];
				bool gotallvalues = (sscanf_s(str.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))", &m[0], &m[1], &m[2], &m[3], &m[4], &m[5], &m[6], &m[7], &m[8], &m[9], &m[10], &m[11], &m[12], &m[13], &m[14], &m[15]) == 16);
				if (gotallvalues) PushBack(glm::mat4{ m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15] });
				success = gotallvalues;
			}
			break;

			case DatumTypes::String:

				PushBack(str);
				success = true;

				break;

			default:

				break;
			}
		}
		catch (...)
		{
			success = false;
		}

		return success;
	}

	bool Datum::SetFromString(const std::string& str, size_type index)
	{
		bool success = false;
		try
		{
			switch (_type)
			{
			case DatumTypes::Integer:

				Set(std::stoi(str), index);
				success = true;

				break;

			case DatumTypes::Float:

				Set(std::stof(str), index);
				success = true;

				break;

			case DatumTypes::Vector:
			{
				float v[4];
				bool gotallvalues = (sscanf_s(str.c_str(), "vec4(%f, %f, %f, %f)", &v[0], &v[1], &v[2], &v[3]) == 4);
				if (gotallvalues) Set(glm::vec4{ v[0], v[1], v[2], v[3] }, index);
				success = gotallvalues;
			}
				break;

			case DatumTypes::Matrix:
			{
				float m[16];
				bool gotallvalues = (sscanf_s(str.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))", &m[0], &m[1], &m[2], &m[3], &m[4], &m[5], &m[6], &m[7], &m[8], &m[9], &m[10], &m[11], &m[12], &m[13], &m[14], &m[15]) == 16);
				if (gotallvalues) Set(glm::mat4{ m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15] }, index);
				success = gotallvalues;
			}
				break;

			case DatumTypes::String:

				Set(str, index);
				success = true;

				break;

			default:

				break;
			}
		}
		catch (...) 
		{
			success = false;
		}

		return success;
	}

	std::string Datum::ToString(size_type index) const
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range");

		switch (_type)
		{
		case DatumTypes::Integer:

			return std::to_string(_data._int[index]);

			break;

		case DatumTypes::Float:

			return std::to_string(_data._float[index]);

			break;

		case DatumTypes::Vector:

			return glm::to_string(_data._vec[index]);

			break;

		case DatumTypes::Matrix:

			return glm::to_string(_data._mat[index]);

			break;

		case DatumTypes::String:

			return _data._string[index];

			break;

		case DatumTypes::Pointer:

			return _data._rtti[index]->ToString();

			break;

		case DatumTypes::ScopeTable:

			return _data._scope[index]->ToString();

			break;

		default:
			throw std::runtime_error("Invalid Type");
			break;
		}
	}

	template<>
	std::int32_t& Datum::Get<std::int32_t>(size_type index)
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range");
		if (_type != DatumTypes::Integer)throw std::runtime_error("Invalid Type");

		return _data._int[index];
	}

	template<>
	const std::int32_t& Datum::Get<std::int32_t>(size_type index) const
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range");
		if (_type != DatumTypes::Integer)throw std::runtime_error("Invalid Type");

		return _data._int[index];
	}

	template<>
	float& Datum::Get<float>(size_type index)
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range");
		if (_type != DatumTypes::Float)throw std::runtime_error("Invalid Type");

		return _data._float[index];
	}

	template<>
	const float& Datum::Get<float>(size_type index) const
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range");
		if (_type != DatumTypes::Float)throw std::runtime_error("Invalid Type");

		return _data._float[index];
	}

	template<>
	glm::vec4& Datum::Get<glm::vec4>(size_type index)
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range");
		if (_type != DatumTypes::Vector)throw std::runtime_error("Invalid Type");

		return _data._vec[index];
	}

	template<>
	const glm::vec4& Datum::Get<glm::vec4>(size_type index) const
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range");
		if (_type != DatumTypes::Vector)throw std::runtime_error("Invalid Type");

		return _data._vec[index];
	}

	template<>
	glm::mat4& Datum::Get<glm::mat4>(size_type index)
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range");
		if (_type != DatumTypes::Matrix)throw std::runtime_error("Invalid Type");

		return _data._mat[index];
	}

	template<>
	const glm::mat4& Datum::Get<glm::mat4>(size_type index) const
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range");
		if (_type != DatumTypes::Matrix)throw std::runtime_error("Invalid Type");

		return _data._mat[index];
	}

	template<>
	std::string& Datum::Get<std::string>(size_type index)
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range");
		if (_type != DatumTypes::String)throw std::runtime_error("Invalid Type");

		return _data._string[index];
	}

	template<>
	const std::string& Datum::Get<std::string>(size_type index) const
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range");
		if (_type != DatumTypes::String)throw std::runtime_error("Invalid Type");

		return _data._string[index];
	}

	template<>
	RTTI*& Datum::Get<RTTI*>(size_type index)
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range");
		if (_type != DatumTypes::Pointer)throw std::runtime_error("Invalid Type");

		return _data._rtti[index];
	}

	template<>
	RTTI* const& Datum::Get<RTTI*>(size_type index) const
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range");
		if (_type != DatumTypes::Pointer)throw std::runtime_error("Invalid Type");

		return _data._rtti[index];
	}

	template<>
	Scope*& Datum::Get<Scope*>(size_type index)
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range");
		if (_type != DatumTypes::ScopeTable)throw std::runtime_error("Invalid Type");

		return _data._scope[index];
	}

	template<>
	Scope* const& Datum::Get<Scope*>(size_type index) const
	{
		if (index >= _size) throw std::out_of_range("Index Out of Range");
		if (_type != DatumTypes::ScopeTable)throw std::runtime_error("Invalid Type");

		return _data._scope[index];
	}

	Scope& Datum::operator[](size_type index)
	{
		return *Get<Scope*>(index);
	}

	const Scope& Datum::operator[](size_type index) const
	{
		return *Get<Scope*>(index);
	}

	template<>
	std::int32_t& Datum::Front<std::int32_t>()
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Integer)throw std::runtime_error("Invalid Type");

		return _data._int[0];
	}

	template<>
	const std::int32_t& Datum::Front<std::int32_t>() const
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Integer)throw std::runtime_error("Invalid Type");

		return _data._int[0];
	}

	template<>
	float& Datum::Front<float>()
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Float)throw std::runtime_error("Invalid Type");

		return _data._float[0];
	}

	template<>
	const float& Datum::Front<float>() const
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Float)throw std::runtime_error("Invalid Type");

		return _data._float[0];
	}

	template<>
	glm::vec4& Datum::Front<glm::vec4>()
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Vector)throw std::runtime_error("Invalid Type");

		return _data._vec[0];
	}

	template<>
	const glm::vec4& Datum::Front<glm::vec4>() const
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Vector)throw std::runtime_error("Invalid Type");

		return _data._vec[0];
	}

	template<>
	glm::mat4& Datum::Front<glm::mat4>()
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Matrix)throw std::runtime_error("Invalid Type");

		return _data._mat[0];
	}

	template<>
	const glm::mat4& Datum::Front<glm::mat4>() const
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Matrix)throw std::runtime_error("Invalid Type");

		return _data._mat[0];
	}

	template<>
	std::string& Datum::Front<std::string>()
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::String)throw std::runtime_error("Invalid Type");

		return _data._string[0];
	}

	template<>
	const std::string& Datum::Front<std::string>() const
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::String)throw std::runtime_error("Invalid Type");

		return _data._string[0];
	}

	template<>
	RTTI*& Datum::Front<RTTI*>()
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Pointer)throw std::runtime_error("Invalid Type");

		return _data._rtti[0];
	}

	template<>
	RTTI* const& Datum::Front<RTTI*>() const
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Pointer)throw std::runtime_error("Invalid Type");

		return _data._rtti[0];
	}

	template<>
	Scope*& Datum::Front<Scope*>()
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::ScopeTable)throw std::runtime_error("Invalid Type");

		return _data._scope[0];
	}

	template<>
	Scope* const& Datum::Front<Scope*>() const
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::ScopeTable)throw std::runtime_error("Invalid Type");

		return _data._scope[0];
	}

	template<>
	std::int32_t& Datum::Back<std::int32_t>()
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Integer)throw std::runtime_error("Invalid Type");

		return _data._int[_size - 1];
	}

	template<>
	const std::int32_t& Datum::Back<std::int32_t>() const
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Integer)throw std::runtime_error("Invalid Type");

		return _data._int[_size - 1];
	}

	template<>
	float& Datum::Back<float>()
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Float)throw std::runtime_error("Invalid Type");

		return _data._float[_size - 1];
	}

	template<>
	const float& Datum::Back<float>() const
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Float)throw std::runtime_error("Invalid Type");

		return _data._float[_size - 1];
	}

	template<>
	glm::vec4& Datum::Back<glm::vec4>()
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Vector)throw std::runtime_error("Invalid Type");

		return _data._vec[_size - 1];
	}

	template<>
	const glm::vec4& Datum::Back<glm::vec4>() const
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Vector)throw std::runtime_error("Invalid Type");

		return _data._vec[_size - 1];
	}

	template<>
	glm::mat4& Datum::Back<glm::mat4>()
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Matrix)throw std::runtime_error("Invalid Type");

		return _data._mat[_size - 1];
	}

	template<>
	const glm::mat4& Datum::Back<glm::mat4>() const
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Matrix)throw std::runtime_error("Invalid Type");

		return _data._mat[_size - 1];
	}

	template<>
	std::string& Datum::Back<std::string>()
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::String)throw std::runtime_error("Invalid Type");

		return _data._string[_size - 1];
	}

	template<>
	const std::string& Datum::Back<std::string>() const
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::String)throw std::runtime_error("Invalid Type");

		return _data._string[_size - 1];
	}

	template<>
	RTTI*& Datum::Back<RTTI*>()
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Pointer)throw std::runtime_error("Invalid Type");

		return _data._rtti[_size - 1];
	}

	template<>
	RTTI* const& Datum::Back<RTTI*>() const
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::Pointer)throw std::runtime_error("Invalid Type");

		return _data._rtti[_size - 1];
	}

	template<>
	Scope*& Datum::Back<Scope*>()
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::ScopeTable)throw std::runtime_error("Invalid Type");

		return _data._scope[_size - 1];
	}

	template<>
	Scope* const& Datum::Back<Scope*>() const
	{
		if (_size == 0) throw std::out_of_range("Object is Empty");
		if (_type != DatumTypes::ScopeTable)throw std::runtime_error("Invalid Type");

		return _data._scope[_size - 1];
	}

	void Datum::PushBack(const std::int32_t data)
	{
		if (_isExternalStorage) throw std::runtime_error("Cannot Modify External Storage");
		if (_type == DatumTypes::Unknown || _type == DatumTypes::Integer)_type = DatumTypes::Integer;
		else throw std::runtime_error("Invalid Type");

		if (_size >= _capacity) Reserve(std::max<size_type>(_incrementFunc(_size, _capacity), _capacity + 1));

		_data._int[_size] = data;
		++_size;
	}

	void Datum::PushBack(const float data)
	{
		if (_isExternalStorage) throw std::runtime_error("Cannot Modify External Storage");
		if (_type == DatumTypes::Unknown || _type == DatumTypes::Float)_type = DatumTypes::Float;
		else throw std::runtime_error("Invalid Type");

		if (_size >= _capacity) Reserve(std::max<size_type>(_incrementFunc(_size, _capacity), _capacity + 1));

		_data._float[_size] = data;
		++_size;
	}

	void Datum::PushBack(const glm::vec4& data)
	{
		if (_isExternalStorage) throw std::runtime_error("Cannot Modify External Storage");
		if (_type == DatumTypes::Unknown || _type == DatumTypes::Vector)_type = DatumTypes::Vector;
		else throw std::runtime_error("Invalid Type");

		if (_size >= _capacity) Reserve(std::max<size_type>(_incrementFunc(_size, _capacity), _capacity + 1));

		_data._vec[_size] = data;
		++_size;
	}

	void Datum::PushBack(const glm::mat4& data)
	{
		if (_isExternalStorage) throw std::runtime_error("Cannot Modify External Storage");
		if (_type == DatumTypes::Unknown || _type == DatumTypes::Matrix)_type = DatumTypes::Matrix;
		else throw std::runtime_error("Invalid Type");

		if (_size >= _capacity) Reserve(std::max<size_type>(_incrementFunc(_size, _capacity), _capacity + 1));

		_data._mat[_size] = data;
		++_size;
	}

	void Datum::PushBack(const std::string& data)
	{
		if (_isExternalStorage) throw std::runtime_error("Cannot Modify External Storage");
		if (_type == DatumTypes::Unknown || _type == DatumTypes::String)_type = DatumTypes::String;
		else throw std::runtime_error("Invalid Type");

		if (_size >= _capacity) Reserve(std::max<size_type>(_incrementFunc(_size, _capacity), _capacity + 1));

		new(_data._string + _size) std::string{ data };
		++_size;
	}

	void Datum::PushBack(std::string&& data)
	{
		if (_isExternalStorage) throw std::runtime_error("Cannot Modify External Storage");
		if (_type == DatumTypes::Unknown || _type == DatumTypes::String)_type = DatumTypes::String;
		else throw std::runtime_error("Invalid Type");

		if (_size >= _capacity) Reserve(std::max<size_type>(_incrementFunc(_size, _capacity), _capacity + 1));

		new(_data._string + _size) std::string{ std::move(data) };
		++_size;
	}

	void Datum::PushBack(RTTI* data)
	{
		if (_isExternalStorage) throw std::runtime_error("Cannot Modify External Storage");
		if (_type == DatumTypes::Unknown || _type == DatumTypes::Pointer)_type = DatumTypes::Pointer;
		else throw std::runtime_error("Invalid Type");

		if (_size >= _capacity) Reserve(std::max<size_type>(_incrementFunc(_size, _capacity), _capacity + 1));

		_data._rtti[_size] = data;
		++_size;
	}

	void Datum::PushBack(Scope& data)
	{
		if (_isExternalStorage) throw std::runtime_error("Cannot Modify External Storage");
		if (_type == DatumTypes::Unknown || _type == DatumTypes::ScopeTable)_type = DatumTypes::ScopeTable;
		else throw std::runtime_error("Invalid Type");

		if (_size >= _capacity) Reserve(std::max<size_type>(_incrementFunc(_size, _capacity), _capacity + 1));

		_data._scope[_size] = &data;
		++_size;
	}

	void Datum::PopBack()
	{
		if (_isExternalStorage) throw std::runtime_error("Cannot Modify External Storage");

		if (_size > 0)
		{
			if (_type == DatumTypes::String)
			{
				_data._string[_size - 1].~basic_string();
			}

			--_size;
		}
	}

	typename Datum::size_type Datum::Find(const std::int32_t value) const
	{
		if (_type != DatumTypes::Integer)throw std::runtime_error("Invalid Type");

		for (size_type i = 0; i < _size; ++i)
		{
			if (_data._int[i] == value)
			{
				return i;
			}
		}

		return _size;
	}

	typename Datum::size_type Datum::Find(const float value) const
	{
		if (_type != DatumTypes::Float)throw std::runtime_error("Invalid Type");

		for (size_type i = 0; i < _size; ++i)
		{
			if (_data._float[i] == value)
			{
				return i;
			}
		}

		return _size;
	}

	typename Datum::size_type Datum::Find(const glm::vec4& value) const
	{
		if (_type != DatumTypes::Vector)throw std::runtime_error("Invalid Type");

		for (size_type i = 0; i < _size; ++i)
		{
			if (_data._vec[i] == value)
			{
				return i;
			}
		}

		return _size;
	}

	typename Datum::size_type Datum::Find(const glm::mat4& value) const
	{
		if (_type != DatumTypes::Matrix)throw std::runtime_error("Invalid Type");

		for (size_type i = 0; i < _size; ++i)
		{
			if (_data._mat[i] == value)
			{
				return i;
			}
		}

		return _size;
	}

	typename Datum::size_type Datum::Find(const std::string& value) const
	{
		if (_type != DatumTypes::String)throw std::runtime_error("Invalid Type");

		for (size_type i = 0; i < _size; ++i)
		{
			if (std::strcmp(_data._string[i].c_str(), value.c_str()) == 0)
			{
				return i;
			}
		}

		return _size;
	}

	typename Datum::size_type Datum::Find(RTTI* value) const
	{
		if (_type != DatumTypes::Pointer)throw std::runtime_error("Invalid Type");

		for (size_type i = 0; i < _size; ++i)
		{
			if (_data._rtti[i] != nullptr && _data._rtti[i]->Equals(value))
			{
				return i;
			}
		}

		return _size;
	}

	typename Datum::size_type Datum::Find(Scope& value)
	{
		if (_type != DatumTypes::ScopeTable)throw std::runtime_error("Invalid Type");

		for (size_type i = 0; i < _size; ++i)
		{
			if (_data._scope[i]->Equals(&value))
			{
				return i;
			}
		}

		return _size;
	}

	typename Datum::size_type Datum::Find(const Scope& value) const
	{
		if (_type != DatumTypes::ScopeTable)throw std::runtime_error("Invalid Type");

		for (size_type i = 0; i < _size; ++i)
		{
			if (_data._scope[i]->Equals(&value))
			{
				return i;
			}
		}

		return _size;
	}

	bool Datum::Remove(const std::int32_t value)
	{
		return RemoveAt(Find(value));
	}

	bool Datum::Remove(const float value)
	{
		return RemoveAt(Find(value));
	}

	bool Datum::Remove(const glm::vec4& value)
	{
		return RemoveAt(Find(value));
	}

	bool Datum::Remove(const glm::mat4& value)
	{
		return RemoveAt(Find(value));
	}

	bool Datum::Remove(const std::string& value)
	{
		return RemoveAt(Find(value));
	}

	bool Datum::Remove(RTTI* value)
	{
		return RemoveAt(Find(value));
	}

	bool Datum::RemoveAt(size_type index)
	{
		if (_isExternalStorage) throw std::runtime_error("Cannot Modify External Storage");

		if (index < _size)
		{
			if (_type == DatumTypes::String)
			{
				_data._string[index].~basic_string();
			}

			size_type typesize = DatumTypeSizes[static_cast<size_type>(_type)];
			std::memmove(_data._byte + typesize * index, _data._byte + typesize * (index + 1), (_size - index - 1) * typesize);

			--_size;
			return true;
		}

		return false;
	}

	void Datum::SetStorageExternal(void* const list, DatumTypes type, size_type size)
	{
		if (list == nullptr || size == 0 || type == DatumTypes::Unknown) throw std::runtime_error("External Storage is Invalid");
		if (!_isExternalStorage)
		{
			if (_size > 0 && _type == DatumTypes::String) while (_size > 0u) _data._string[--_size].~basic_string();
			if (_capacity > 0) free(_data._voidp);
		}

		_type = type;
		_data._voidp = list;
		_size = _capacity = size;
		_isExternalStorage = true;
	}

	void Datum::SetStorageExternal(std::int32_t* const list, size_type size)
	{
		SetStorageExternal(list, DatumTypes::Integer, size);
	}

	void Datum::SetStorageExternal(float* const list, size_type size)
	{
		SetStorageExternal(list, DatumTypes::Float, size);
	}

	void Datum::SetStorageExternal(glm::vec4* const list, size_type size)
	{
		SetStorageExternal(list, DatumTypes::Vector, size);
	}

	void Datum::SetStorageExternal(glm::mat4* const list, size_type size)
	{
		SetStorageExternal(list, DatumTypes::Matrix, size);
	}

	void Datum::SetStorageExternal(std::string* const list, size_type size)
	{
		SetStorageExternal(list, DatumTypes::String, size);
	}

	void Datum::SetStorageExternal(RTTI** const list, size_type size)
	{
		SetStorageExternal(list, DatumTypes::Pointer, size);
	}

	void Datum::SetIncrementFunctor(std::function<size_type(size_type, size_type)> incrementFunc)
	{
		_incrementFunc = incrementFunc;
	}
}