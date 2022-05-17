#pragma once

#include "IJsonParser.h"
#include "JsonParseManager.h"
#include "Stack.h"
#include "Vector.h"
#include "HashMap.h"
#include "Scope.h"
#include "GameObject.h"

using namespace std::string_literals;
namespace FIEAGameEngine
{
	class JsonParserScope final : public FIEAGameEngine::IJsonParser
	{
		RTTI_DECLARATIONS(JsonParserInteger);

	public:

		struct ContextPair { std::string key; Scope& scope; };

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
			/// Vector that stores all parsed GameObjects
			/// </summary>
			FIEAGameEngine::Vector<std::shared_ptr<GameObject>> Data;
		};

		/// <summary>
		/// Create a new instance of this Parser
		/// </summary>
		/// <returns>Raw pointer to the newly created instance</returns>
		gsl::owner<IJsonParser*> Create() const override;

		/// <summary>
		/// Initialize Parser to fresh default values
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// Parse Scope from given json object
		/// </summary>
		/// <param name="sharedData">Reference to shared data to parse into</param>
		/// <param name="key">Reference to key of json pair</param>
		/// <param name="value"></param>
		/// <returns>TRUE if this is able to handle the json grammar | FALSE if not able to handle</returns>
		bool StartHandler(FIEAGameEngine::JsonParseManager::SharedData& sharedData, const std::string& key, const Json::Value& value) override;

		/// <summary>
		/// End Parsing the given Scope
		/// </summary>
		/// <param name="sharedData">Reference to shared data to parse into</param>
		/// <param name="key">Reference to key of json pair</param>
		/// <param name="value">Reference to JSON value object to parse</param>
		/// <returns>TRUE if succesfully parsed | FALSE if nothing was added to shared data</returns>
		bool EndHandler(FIEAGameEngine::JsonParseManager::SharedData& sharedData, const std::string& key, const Json::Value& value) override;

	private:
		DatumTypes GetDatumType(const std::string& key);

	private:
		Stack<ContextPair> _stack;
		size_type array_index{ 0 };
	};
}