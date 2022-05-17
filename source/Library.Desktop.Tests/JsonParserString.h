#pragma once

#include "IJsonParser.h"
#include "JsonParseManager.h"
#include "Vector.h"

namespace LibraryDesktopTests
{
	class JsonParserString final : public FIEAGameEngine::IJsonParser
	{
		RTTI_DECLARATIONS(JsonParserString);

	public:

		class SharedData final : public FIEAGameEngine::JsonParseManager::SharedData
		{
			RTTI_DECLARATIONS(SharedData);

		public:

			/// <summary>
			/// Initialize Shared Data to fresh default values
			/// </summary>
			virtual void Initialize() override;

			/// <summary>
			/// Create a new instance of this Shared Data
			/// </summary>
			/// <returns>Raw Pointer to new instance</returns>
			virtual gsl::owner<SharedData*> Create() const override;

			/// <summary>
			/// Get Depth of Current Parsing State
			/// </summary>
			/// <returns>Depth of json object</returns>
			size_type Depth() const;

			/// <summary>
			/// Vector that stores all parsed Data
			/// </summary>
			FIEAGameEngine::Vector<std::string> Data;

			/// <summary>
			/// Max Depth Reached by this Shared Data's Parser
			/// </summary>
			size_type _maxdepth{ 0 };
		};

		/// <summary>
		/// Create a new instance of this Parser
		/// </summary>
		/// <returns>Raw pointer to the newly created instance</returns>
		virtual gsl::owner<IJsonParser*> Create() const override;

		/// <summary>
		/// Initialize Parser to fresh default values
		/// </summary>
		virtual void Initialize() override;

		/// <summary>
		/// Check if this Parser is able to Parse the given object
		/// </summary>
		/// <param name="sharedData">Reference to shared data to parse into</param>
		/// <param name="key">Reference to key of json pair</param>
		/// <returns>TRUE if this is able to handle the json grammar | FALSE if not able to handle</returns>
		virtual bool StartHandler(FIEAGameEngine::JsonParseManager::SharedData& sharedData, const std::string& key, const Json::Value& value) override;

		/// <summary>
		/// Parse String data corresponding to previous Start Handler call
		/// </summary>
		/// <param name="sharedData">Reference to shared data to parse into</param>
		/// <param name="key">Reference to key of json pair</param>
		/// <param name="value">Reference to JSON value object to parse</param>
		/// <returns>TRUE if succesfully parsed | FALSE if nothing was added to shared data</returns>
		virtual bool EndHandler(FIEAGameEngine::JsonParseManager::SharedData& sharedData, const std::string& key, const Json::Value& value) override;

	private:
		inline static const std::string StringKey{ "string" };
		bool _isParsing{ false };
	};
}