#include "Datum.h"

namespace FIEAGameEngine
{
	inline DatumTypes Datum::Type() const { return _type; }
	inline bool Datum::IsExternalStorage() const { return _isExternalStorage; }

	inline bool Datum::SetType(DatumTypes type)
	{
		if (_type == DatumTypes::Unknown || _type == type)
		{
			_type = type;
			return true;
		}

		return false;
	}

	inline typename Datum::size_type Datum::Size() const { return _size; }
	inline typename Datum::size_type Datum::Capacity() const { return _capacity; }
}