#include "pch.h"
#include "JsonParserVec.h"

using namespace FIEAGameEngine;
namespace LibraryDesktopTests
{
#pragma region SHARED_DATA
	RTTI_DEFINITIONS(JsonParserVec::SharedData, FIEAGameEngine::JsonParseManager::SharedData);

	void JsonParserVec::SharedData::Initialize()
	{
		JsonParseManager::SharedData::Initialize();
		Data.Clear();
		_maxdepth = 0;
	}

	gsl::owner<JsonParserVec::SharedData*> JsonParserVec::SharedData::Create() const
	{
		return new JsonParserVec::SharedData();
	}

	JsonParseManager::SharedData::size_type JsonParserVec::SharedData::Depth() const { return _depth; }
#pragma endregion

	RTTI_DEFINITIONS(JsonParserVec, IJsonParser);

	gsl::owner<IJsonParser*> JsonParserVec::Create() const
	{
		return new JsonParserVec();
	}

	void JsonParserVec::Initialize()
	{
		IJsonParser::Initialize();
		_isParsing = false;
	}

	bool JsonParserVec::StartHandler(FIEAGameEngine::JsonParseManager::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		if (key.empty()) return true;
		JsonParserVec::SharedData* sdata = sharedData.As<JsonParserVec::SharedData>();
		if (sdata == nullptr || std::strcmp(key.c_str(), VecKey.c_str()) != 0) return false;

		if (_isParsing) throw std::runtime_error("Integer Parser Already Running");
		
		sdata->_maxdepth = std::max(sdata->Depth(), sdata->_maxdepth);

		if (!value.isString()) return false;

		_isParsing = true;
		return true;
	}

	bool JsonParserVec::EndHandler(FIEAGameEngine::JsonParseManager::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		if (key.empty()) return true;
		JsonParserVec::SharedData* sdata = sharedData.As<JsonParserVec::SharedData>();
		if (sdata == nullptr || std::strcmp(key.c_str(), VecKey.c_str()) != 0 || _isParsing == false) return false;

		float v[4];
		bool gotallvalues = (sscanf_s(value.asCString(), "vec4(%f, %f, %f, %f)", &v[0], &v[1], &v[2], &v[3]) == 4);
		if (gotallvalues) sdata->Data.PushBack(glm::vec4{ v[0], v[1], v[2], v[3] });

		_isParsing = false;
		return gotallvalues;
	}
}