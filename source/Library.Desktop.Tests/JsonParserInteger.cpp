#include "pch.h"
#include "JsonParserInteger.h"

using namespace FIEAGameEngine;
namespace LibraryDesktopTests
{
#pragma region SHARED_DATA
	RTTI_DEFINITIONS(JsonParserInteger::SharedData, FIEAGameEngine::JsonParseManager::SharedData);

	void JsonParserInteger::SharedData::Initialize()
	{
		JsonParseManager::SharedData::Initialize();
		Data.Clear();
		_maxdepth = 0;
	}

	gsl::owner<JsonParserInteger::SharedData*> JsonParserInteger::SharedData::Create() const
	{
		return new JsonParserInteger::SharedData();
	}

	JsonParseManager::SharedData::size_type JsonParserInteger::SharedData::Depth() const { return _depth; }
#pragma endregion

	RTTI_DEFINITIONS(JsonParserInteger, IJsonParser);

	gsl::owner<IJsonParser*> JsonParserInteger::Create() const
	{
		return new JsonParserInteger();
	}

	void JsonParserInteger::Initialize()
	{
		IJsonParser::Initialize();
		_isParsing = false;
	}

	bool JsonParserInteger::StartHandler(FIEAGameEngine::JsonParseManager::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		if (key.empty()) return true;
		JsonParserInteger::SharedData* sdata = sharedData.As<JsonParserInteger::SharedData>();
		if (sdata == nullptr || std::strcmp(key.c_str(), IntegerKey.c_str()) != 0) return false;

		if (_isParsing) throw std::runtime_error("Integer Parser Already Running");

		sdata->_maxdepth = std::max(sdata->Depth(), sdata->_maxdepth);

		if (!value.isInt()) return false;

		_isParsing = true;
		return true;
	}

	bool JsonParserInteger::EndHandler(FIEAGameEngine::JsonParseManager::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		if (key.empty()) return true;
		JsonParserInteger::SharedData* sdata = sharedData.As<JsonParserInteger::SharedData>();
		if (sdata == nullptr || std::strcmp(key.c_str(), IntegerKey.c_str()) != 0 || _isParsing == false) return false;
		
		sdata->Data.PushBack(value.asInt());

		_isParsing = false;
		return true;
	}
}