#pragma once

#include "RTTI.h"
#include <gsl/gsl>
#include "JsonParseManager.h"

#pragma warning(push)
#pragma warning(disable:26812)
#pragma warning(disable:4635)
#include <json/json.h>
#pragma warning(pop)

namespace FIEAGameEngine
{
	class IJsonParser : public RTTI
	{
		RTTI_DECLARATIONS(IJsonParser);

	public:
		using size_type = std::size_t;

		/// <summary>
		/// Initialize Parser to default values
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Check if this Parser is able to Parse the given object
		/// </summary>
		/// <param name="sharedData">Reference to shared data to parse into</param>
		/// <param name="key">Reference to key of json pair</param>
		/// <param name="value">Reference to JSON value object to parse</param>
		/// <returns>TRUE if this is able to handle the json grammar | FALSE if not able to handle</returns>
		virtual bool StartHandler(JsonParseManager::SharedData& sharedData, const std::string& key, const Json::Value& value) = 0;

		/// <summary>
		/// Parse the data corresponding to previous Start Handler call
		/// </summary>
		/// <param name="sharedData">Reference to shared data to parse into</param>
		/// <param name="key">Reference to key of json pair</param>
		/// <param name="value">Reference to JSON value object to parse</param>
		/// <returns>TRUE if succesfully parsed | FALSE if nothing was added to shared data</returns>
		virtual bool EndHandler(JsonParseManager::SharedData& sharedData, const std::string& key, const Json::Value& value) = 0;

		/// <summary>
		/// Create a new instance of this Parser
		/// </summary>
		/// <returns>Raw Poitner to new instance</returns>
		virtual gsl::owner<IJsonParser*> Create() const = 0;

		virtual ~IJsonParser() = default;
	};
}