#include "pch.h"
#include "JsonParserString.h"

using namespace FIEAGameEngine;
namespace LibraryDesktopTests
{
#pragma region SHARED_DATA
	RTTI_DEFINITIONS(JsonParserString::SharedData, FIEAGameEngine::JsonParseManager::SharedData);

	void JsonParserString::SharedData::Initialize()
	{
		JsonParseManager::SharedData::Initialize();
		Data.Clear();
		_maxdepth = 0;
	}

	gsl::owner<JsonParserString::SharedData*> JsonParserString::SharedData::Create() const
	{
		return new JsonParserString::SharedData();
	}

	JsonParseManager::SharedData::size_type JsonParserString::SharedData::Depth() const { return _depth; }
#pragma endregion

	RTTI_DEFINITIONS(JsonParserString, IJsonParser);

	gsl::owner<IJsonParser*> JsonParserString::Create() const
	{
		return new JsonParserString();
	}

	void JsonParserString::Initialize()
	{
		IJsonParser::Initialize();
		_isParsing = false;
	}

	bool JsonParserString::StartHandler(FIEAGameEngine::JsonParseManager::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		if (key.empty()) return true;
		JsonParserString::SharedData* sdata = sharedData.As<JsonParserString::SharedData>();
		if (sdata == nullptr || std::strcmp(key.c_str(), StringKey.c_str()) != 0) return false;

		if (_isParsing) throw std::runtime_error("Integer Parser Already Running");
		
		sdata->_maxdepth = std::max(sdata->Depth(), sdata->_maxdepth);

		if (!value.isString()) return false;

		_isParsing = true;
		return true;
	}

	bool JsonParserString::EndHandler(FIEAGameEngine::JsonParseManager::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		if (key.empty()) return true;
		JsonParserString::SharedData* sdata = sharedData.As<JsonParserString::SharedData>();
		if (sdata == nullptr || std::strcmp(key.c_str(), StringKey.c_str()) != 0 || _isParsing == false) return false;

		sdata->Data.PushBack(std::move(value.asString()));

		_isParsing = false;
		return true;
	}
}