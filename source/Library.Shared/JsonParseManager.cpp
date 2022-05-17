#include "pch.h"
#include "JsonParseManager.h"
#include "IJsonParser.h"
#include <fstream>

namespace FIEAGameEngine
{
#pragma region SHARED_DATA
	RTTI_DEFINITIONS(JsonParseManager::SharedData, RTTI);

	void JsonParseManager::SharedData::Initialize()
	{
		_depth = 0;
	}

	JsonParseManager* JsonParseManager::SharedData::GetJsonParseManager() { return _manager; }
	const JsonParseManager* JsonParseManager::SharedData::GetJsonParseManager() const { return _manager; }

	void JsonParseManager::SharedData::IncreaseDepth() { ++_depth; }
	void JsonParseManager::SharedData::DecreaseDepth() { if (_depth > 0) --_depth; }
#pragma endregion

	JsonParseManager::JsonParseManager(SharedData& data) : _sharedData{ &data }
	{
		data._manager = this;
	}

	JsonParseManager::JsonParseManager(JsonParseManager&& rhs) noexcept : _isClone{ rhs._isClone }, _clonedSD{ rhs._clonedSD }, _sharedData{ rhs._sharedData }, _parsers{ std::move(rhs._parsers) }
	{
		rhs._isClone = false;
		rhs._clonedSD = nullptr;
		rhs._sharedData = nullptr;
		if (_sharedData != nullptr) _sharedData->_manager = this;
	}

	JsonParseManager& JsonParseManager::operator=(JsonParseManager&& rhs) noexcept
	{
		if (this != &rhs)
		{
			CleanClonedData();

			_isClone = rhs._isClone;
			_clonedSD = rhs._clonedSD;
			_sharedData = rhs._sharedData;
			_parsers = std::move(rhs._parsers);

			rhs._isClone = false;
			rhs._clonedSD = nullptr;
			rhs._sharedData = nullptr;
			if (_sharedData != nullptr) _sharedData->_manager = this;
		}

		return *this;
	}

	JsonParseManager::~JsonParseManager()
	{
		CleanClonedData();
	}

	void JsonParseManager::CleanClonedData()
	{
		if (_isClone)
		{
			for (size_type i = 0; i < _parsers.Size(); ++i)
			{
				delete _parsers[i];
			}
		}

		delete _clonedSD;
	}

	JsonParseManager::SharedData* JsonParseManager::GetSharedData() const
	{
		return _sharedData;
	}

	void JsonParseManager::SetSharedData(SharedData& data)
	{
		if (_sharedData) _sharedData->_manager = nullptr;

		_sharedData = &data;
		data._manager = this;
	}

	bool JsonParseManager::AddParser(IJsonParser& parser)
	{
		if (_isClone) return false;

		for (const IJsonParser* _parser : _parsers)
		{
			if (_parser->TypeIdInstance() == parser.TypeIdInstance())
			{
				return false;
			}
		}

		Vector<IJsonParser*>::Iterator itr = _parsers.PushBack(&parser);
		return (itr != _parsers.end());
	}

	bool JsonParseManager::RemoveParser(IJsonParser& parser)
	{
		if (_parsers.Remove(&parser))
		{
			if (_isClone) delete& parser;
			return true;
		}

		return false;
	}

	JsonParseManager::size_type JsonParseManager::GetParserCount() const { return _parsers.Size(); }
	IJsonParser& JsonParseManager::GetParser(size_type index) const { return *(_parsers.At(index)); }

	gsl::owner<JsonParseManager*> JsonParseManager::Clone() const
	{
		SharedData* newSData{ nullptr };
		if (_sharedData) newSData = _sharedData->Create();

		JsonParseManager* clone = new JsonParseManager(*newSData);
		clone->_isClone = true;
		clone->_clonedSD = newSData;

		for (IJsonParser* parser : _parsers) clone->_parsers.PushBack(parser->Create());

		return clone;
	}

	void JsonParseManager::Initialize()
	{
		_sharedData->Initialize();
		for (IJsonParser* parser : _parsers) parser->Initialize();
	}

	void JsonParseManager::Parse(const std::string& jsonstring)
	{
		std::istringstream jsonstream(jsonstring);
		return Parse(jsonstream);
	}

	void JsonParseManager::ParseFile(const std::string& filename)
	{
		std::ifstream filestream(filename, std::ifstream::binary);
		if (!filestream.good()) throw std::runtime_error("Could not open File");
		return Parse(filestream);
	}

	void JsonParseManager::Parse(std::istream& jsonstream)
	{
		if (_sharedData)
		{
			Initialize();
			Json::Value root;
			jsonstream >> root;
			ParseMembers(root);
		}
	}

	void JsonParseManager::ParseMembers(const Json::Value& objson)
	{
		if (objson.isObject()) TryParsers(std::string{}, objson);
		else if (objson.isArray()) for (const Json::Value& element : objson) TryParsers(std::string{}, element);
	}

	void JsonParseManager::Parse(const Json::Value& root)
	{
		if (root.size() > 0)
		{
			_sharedData->IncreaseDepth();
			for (const Json::String& key : root.getMemberNames()) Parse(key, root[key]);
			_sharedData->DecreaseDepth();
		}
	}

	void JsonParseManager::Parse(const std::string& key, const Json::Value& value)
	{
		if (value.isArray()) for (const Json::Value& element : value) TryParsers(key, element);
		else TryParsers(key, value);
	}

	void JsonParseManager::TryParsers(const std::string& key, const Json::Value& value)
	{
		for (size_type i = 0; i < _parsers.Size(); ++i)
		{
			if (_parsers[i]->StartHandler(*_sharedData, key, value))
			{
				if (value.isObject()) Parse(value);
				if (_parsers[i]->EndHandler(*_sharedData, key, value) == false) throw std::runtime_error("This EndHandler Should Have DEFINITELY Worked!");
				return;
			}
		}
	}

}