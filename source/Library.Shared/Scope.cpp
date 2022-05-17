#include "pch.h"
#include "Scope.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(Scope, RTTI);

	Scope::Scope(size_type capacity, size_type hashmap_size, HashFunct hashFunct, EqualityFunct equalityFunct) :
		_hashmap{ hashmap_size, hashFunct, equalityFunct }, _orderTable{ capacity }, _parent{ nullptr }
	{

	}

	Scope::~Scope()
	{
		RemoveParent();
		Clear();
	}

	void Scope::Clear()
	{
		for (_TPair* pair : _orderTable)
		{
			Datum& datum = pair->second;
			if (datum.Type() == DatumTypes::ScopeTable && !datum.IsExternalStorage())
			{
				for (size_type i = 0; i < datum.Size(); ++i)
				{
					datum[i]._parent = nullptr;
					delete& datum[i];
				}
			}
		}
	}

	Scope::Scope(const Scope& rhs) :
		_hashmap{ rhs._hashmap.Capacity(), rhs._hashmap.GetHashFunct(), rhs._hashmap.GetEqualityFunct() }, _orderTable{ rhs._orderTable.Size() }, _parent{ nullptr }
	{
		for (_TPair* pair : rhs._orderTable)
		{
			auto& [key, datum] = *pair;
			if (datum.Type() == DatumTypes::ScopeTable && !datum.IsExternalStorage())
			{
				Datum& appendatum = Append(key, DatumTypes::ScopeTable);
				for (size_type i = 0; i < datum.Size(); ++i)
				{
					appendatum.PushBack(*(datum[i].Clone()));
					appendatum[i]._parent = this;
				}
			}
			else Append(key, datum);
		}
	}

	Scope& Scope::operator=(const Scope& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			_hashmap.Clear();
			_orderTable.Clear();

			for (_TPair* pair : rhs._orderTable)
			{
				auto& [key, datum] = *pair;
				if (datum.Type() == DatumTypes::ScopeTable && !datum.IsExternalStorage())
				{
					Datum& appendatum = Append(key, DatumTypes::ScopeTable);
					for (size_type i = 0; i < datum.Size(); ++i)
					{
						appendatum.PushBack(*(datum[i].Clone()));
						appendatum[i]._parent = this;
					}
				}
				else Append(key, datum);
			}
		}

		return *this;
	}

	Scope::Scope(Scope&& rhs) noexcept :
		_orderTable{ std::move(rhs._orderTable) }, _hashmap{ std::move(rhs._hashmap) }, _parent{ rhs._parent }
	{
		if (_parent != nullptr)
		{
			auto [datum, index] = _parent->FindContainedScope(rhs);
			if (datum != nullptr) datum->Set(*this, index);
		}

		for (_TPair* pair : _orderTable)
		{
			auto& [key, datum] = *pair;
			if (datum.Type() == DatumTypes::ScopeTable)
			{
				for (size_type i = 0; i < datum.Size(); ++i)
				{
					datum[i]._parent = this;
				}
			}
		}
	}

	Scope& Scope::operator=(Scope&& rhs) noexcept
	{
		if (this != &rhs)
		{
			RemoveParent();
			Clear();
			_hashmap.Clear();
			_orderTable.Clear();

			_orderTable = std::move(rhs._orderTable);
			_hashmap = std::move(rhs._hashmap);
			_parent = rhs._parent;

			if (_parent != nullptr)
			{
				auto [datum, index] = _parent->FindContainedScope(rhs);
				if (datum != nullptr) datum->Set(*this, index);
			}

			for (_TPair* pair : _orderTable)
			{
				auto& [key, datum] = *pair;
				if (datum.Type() == DatumTypes::ScopeTable)
				{
					for (size_type i = 0; i < datum.Size(); ++i)
					{
						datum[i]._parent = this;
					}
				}
			}
		}

		return *this;
	}


	gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope(*this);
	}

	Datum& Scope::Append(const std::string& name, DatumTypes type, size_type capacity)
	{
		if (name.empty()) throw std::runtime_error("String is empty!");

		auto [iterator, wasInserted] = _hashmap.Insert(std::move(_TPair(name, Datum{ type, capacity })));
		if (wasInserted) _orderTable.PushBack(&(*iterator));

		return iterator->second;
	}

	Datum& Scope::Append(const std::string& name, const Datum& datum)
	{
		if (name.empty()) throw std::runtime_error("String is empty!");

		auto [iterator, wasInserted] = _hashmap.Insert(std::move(_TPair(name, Datum{ datum })));
		if (wasInserted) _orderTable.PushBack(&(*iterator));

		return iterator->second;
	}

	Scope& Scope::AppendScope(const std::string& name, size_type capacity, size_type hashmap_size, HashFunct hashFunct, EqualityFunct equalityFunct)
	{
		if (name.empty()) throw std::runtime_error("String is empty!");

		Datum* foundDatum{ nullptr };
		Scope* scope{ nullptr };
		if (_hashmap.ContainsKey(name, foundDatum))
		{
			if (foundDatum->IsExternalStorage()) throw std::runtime_error("Cannot Add to External Storage");

			if (foundDatum->Type() == DatumTypes::Unknown || foundDatum->Type() == DatumTypes::ScopeTable)
			{
				scope = new Scope{ capacity, hashmap_size, hashFunct, equalityFunct };
				scope->_parent = this;
				foundDatum->PushBack(*scope);
			}
			else throw std::runtime_error("Invalid Type");
		}
		else
		{
			scope = new Scope{ capacity, hashmap_size, hashFunct, equalityFunct };
			scope->_parent = this;
			auto [iterator, wasInserted] = _hashmap.Insert(std::move(_TPair(name, Datum{ *scope })));
			_orderTable.PushBack(&(*iterator));
		}

		return *scope;
	}

	void Scope::Adopt(Scope& scope, const std::string& name)
	{
		if (name.empty()) throw std::runtime_error("String is empty!");

		if (this != &scope)
		{
			if (IsRecursiveChildOf(scope)) throw std::runtime_error("Scope Reference already found in up hierarchy! Cannot adopt to avoid cyclical structure");

			const auto& [iterator, wasInserted] = _hashmap.Insert(std::move(_TPair(name, Datum{ scope })));
			if (wasInserted) _orderTable.PushBack(&(*iterator));
			else iterator->second.PushBack(scope);

			scope.RemoveParent();
			scope._parent = this;
		}
	}

	void Scope::RemoveParent()
	{
		if (_parent != nullptr)
		{
			auto [datum, index] = _parent->FindContainedScope(*this);
			if (datum != nullptr && !datum->IsExternalStorage()) datum->RemoveAt(index);
			_parent = nullptr;
		}
	}

	bool Scope::IsRecursiveParentOf(const Scope& scope) const
	{
		Scope* parent{ scope._parent };
		while (parent != nullptr)
		{
			if (parent == this) return true;
			parent = parent->_parent;
		}
		return false;
	}

	bool Scope::IsRecursiveChildOf(const Scope& scope) const
	{
		return scope.IsRecursiveParentOf(*this);
	}

	Scope* Scope::GetParent() { return _parent; }
	const Scope* Scope::GetParent() const { return _parent; }

	Datum& Scope::operator[](std::string name) { return Append(name); }

	Datum& Scope::operator[](size_type index) { return _orderTable[index]->second; }
	const Datum& Scope::operator[](size_type index) const { return _orderTable[index]->second; }

	bool Scope::operator==(const Scope& rhs) const
	{
		return ((this == &rhs) || (_orderTable.ComparePointerData(rhs._orderTable)));
	}

	bool Scope::operator!=(const Scope& rhs) const
	{
		return !operator==(rhs);
	}

	Scope::size_type Scope::Size() const
	{
		return _orderTable.Size();
	}

	Datum& Scope::At(const std::string& attr_key)
	{
		Datum* attr = Find(attr_key);
		if (attr == nullptr) throw std::runtime_error("Attribute Doesn't Exist");
		return *attr;
	}

	const Datum& Scope::At(const std::string& attr_key) const
	{
		const Datum* attr = Find(attr_key);
		if (attr == nullptr) throw std::runtime_error("Attribute Doesn't Exist");
		return *attr;
	}

	Datum* Scope::Find(const std::string& name, bool findInChildren, Scope** foundScope)
	{
		if (name.empty()) throw std::runtime_error("String is empty!");

		Datum* foundDatum{ nullptr };
		Scope* _foundScope{ nullptr };

		_HashMapSignature::Iterator foundItr = _hashmap.Find(name);
		if (foundItr != _hashmap.end())
		{
			foundDatum = &(foundItr->second);
			_foundScope = this;
		}

		if (foundDatum == nullptr && findInChildren)
		{
			for (_TPair* pair : _orderTable)
			{
				Datum& datum = pair->second;
				if (datum.Type() == DatumTypes::ScopeTable)
				{
					for (size_type i = 0; i < datum.Size(); ++i)
					{
						foundDatum = datum[i].Find(name, true);
						if (foundDatum)
						{
							_foundScope = &(datum[i]);
							break;
						}
					}

					if (foundDatum) break;
				}
			}
		}

		if (foundScope != nullptr) *foundScope = _foundScope;
		return foundDatum;
	}

	const Datum* Scope::Find(const std::string& name, bool findInChildren, const Scope** foundScope) const
	{
		if (name.empty()) throw std::runtime_error("String is empty!");

		const Datum* foundDatum{ nullptr };
		const Scope* _foundScope{ nullptr };

		_HashMapSignature::Const_Iterator foundItr = _hashmap.Find(name);
		if (foundItr != _hashmap.end())
		{
			foundDatum = &(foundItr->second);
			_foundScope = this;
		}

		if (foundDatum == nullptr && findInChildren)
		{
			for (_TPair* pair : _orderTable)
			{
				Datum& datum = pair->second;
				if (datum.Type() == DatumTypes::ScopeTable)
				{
					for (size_type i = 0; i < datum.Size(); ++i)
					{
						foundDatum = datum[i].Find(name, true);
						if (foundDatum)
						{
							_foundScope = &(datum[i]);
							break;
						}
					}

					if (foundDatum) break;
				}
			}
		}

		if (foundScope != nullptr) *foundScope = _foundScope;
		return foundDatum;
	}

	std::pair<Datum*, std::size_t> Scope::FindContainedScope(const Scope& scope) const
	{
		Datum* foundDatum{ nullptr };
		size_type foundIndex = 0;

		for (_TPair* const& pair : _orderTable)
		{
			Datum& datum = pair->second;
			if (datum.Type() == DatumTypes::ScopeTable)
			{
				for (size_type i = 0; i < datum.Size(); ++i)
				{
					if (&datum[i] == &scope)
					{
						foundIndex = i;
						foundDatum = &datum;
						break;
					}
				}
			}

			if (foundDatum) break;
		}

		return std::pair<Datum*, size_type>{foundDatum, foundIndex};
	}

	Datum* Scope::Search(const std::string& name)
	{
		Scope* foundScope{ nullptr };
		return Search(name, foundScope);
	}

	const Datum* Scope::Search(const std::string& name) const
	{
		const Scope* foundScope{ nullptr };
		return Search(name, foundScope);
	}

	Datum* Scope::Search(const std::string& name, Scope*& foundScope)
	{
		if (name.empty()) throw std::runtime_error("String is empty!");

		Datum* foundDatum = Find(name, false, &foundScope);

		if (foundDatum == nullptr && _parent != nullptr) foundDatum = _parent->Search(name, foundScope);

		return foundDatum;
	}

	const Datum* Scope::Search(const std::string& name, const Scope*& foundScope) const
	{
		if (name.empty()) throw std::runtime_error("String is empty!");

		const Datum* foundDatum = Find(name, false, &foundScope);

		if (foundDatum == nullptr && _parent != nullptr) foundDatum = _parent->Search(name, foundScope);

		return foundDatum;
	}

	std::string Scope::ToString() const
	{
		return "Scope";
	}
}