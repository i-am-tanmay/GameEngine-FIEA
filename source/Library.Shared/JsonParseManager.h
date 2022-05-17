#pragma once

#include "RTTI.h"
#include <gsl/gsl>
#include "Vector.h"

#pragma warning(push)
#pragma warning(disable:26812)
#pragma warning(disable:4635)
#include <json/json.h>
#pragma warning(pop)

#include <iostream>

namespace FIEAGameEngine
{
	class IJsonParser;

	class JsonParseManager final
	{
	public:
		using size_type = std::size_t;

		class SharedData : public RTTI
		{
			RTTI_DECLARATIONS(SharedData);

		public:
			using size_type = std::size_t;

			friend JsonParseManager;

		public:

			/// <summary>
			/// Initialize shared data to fresh default values
			/// </summary>
			virtual void Initialize();

			/// <summary>
			/// Create a new instance of this shared data
			/// </summary>
			/// <returns>Raw pointer to new instance</returns>
			virtual gsl::owner<SharedData*> Create() const = 0;

			/// <summary>
			/// Get Parse Manager this shared data is associated to
			/// </summary>
			/// <returns>Pointer to Parse Manager</returns>
			JsonParseManager* GetJsonParseManager();

			/// <summary>
			/// Get Const Parse Manager this shared data is associated to
			/// </summary>
			/// <returns>Pointer to Const Parse Manager</returns>
			const JsonParseManager* GetJsonParseManager() const;

			virtual ~SharedData() = default;

		protected:
			size_type _depth{ 0 };

		private:
			void IncreaseDepth();
			void DecreaseDepth();

			JsonParseManager* _manager{ nullptr };

		};

	public:

		/// <summary>
		/// Constructor to Initialize Parse Manager with Shared Data
		/// </summary>
		/// <param name="data">Reference to shared data</param>
		JsonParseManager(SharedData& data);

		JsonParseManager(const JsonParseManager& rhs) = delete;
		JsonParseManager& operator=(const JsonParseManager& rhs) = delete;

		/// <summary>
		/// Move Construct: Parse Manager by moving other parse manager
		/// </summary>
		/// <param name="rhs">Reference to other parse manager to move</param>
		JsonParseManager(JsonParseManager&& rhs) noexcept;

		/// <summary>
		/// Move Assignment: Parse Manager by moving other parse manager
		/// </summary>
		/// <param name="rhs">Reference to other parse manager to move</param>
		/// <returns>Reference to new this Parse Manager after moving</returns>
		JsonParseManager& operator=(JsonParseManager&& rhs) noexcept;

		~JsonParseManager();

		/// <summary>
		/// Get Shared Data associated with this Parse Manager
		/// </summary>
		/// <returns>Pointer to Shared Data if any</returns>
		SharedData* GetSharedData() const;

		/// <summary>
		/// Associate Shared Data to this Parse Manager
		/// </summary>
		/// <param name="data">Reference to shared data to incorporate</param>
		void SetSharedData(SharedData& data);

		/// <summary>
		/// Add Parser to List of parsers to parse from
		/// </summary>
		/// <param name="parser">Reference to Parser to add</param>
		/// <returns>TRUE if parser was inserted | FALSE if nothing was inserted</returns>
		bool AddParser(IJsonParser& parser);

		/// <summary>
		/// Remove Parser from List of parsers to parse from
		/// </summary>
		/// <param name="parser">Reference to Parser to remove</param>
		/// <returns>TRUE if parser was removed | FALSE if nothing was removed</returns>
		bool RemoveParser(IJsonParser& parser);

		/// <summary>
		/// Get Number of Parsers in the List of parsers to parse from
		/// </summary>
		/// <returns>Count of list of parsers</returns>
		size_type GetParserCount() const;

		/// <summary>
		/// Get Parser at index from List of parsers to parse from
		/// </summary>
		/// <param name="index">Index of parser</param>
		/// <returns>Reference to Parser at given index</returns>
		IJsonParser& GetParser(size_type index) const;

		/// <summary>
		/// Clone this Parse Manager creating new instances for each Parser and shared data
		/// </summary>
		/// <returns>Raw Pointer to cloned Parse Manager</returns>
		gsl::owner<JsonParseManager*> Clone() const;

		/// <summary>
		/// Parse a string of JSON
		/// </summary>
		/// <param name="jsonstring">Reference to string containing json format</param>
		void Parse(const std::string& jsonstring);

		/// <summary>
		/// Parse a JSON file
		/// </summary>
		/// <param name="filename">PATH of the file</param>
		void ParseFile(const std::string& filename);

		/// <summary>
		/// Parse a stream of JSON
		/// </summary>
		/// <param name="jsonstream">Reference to input-stream of json format</param>
		void Parse(std::istream& jsonstream);

	private:
		void Initialize();

		void ParseMembers(const Json::Value& objson);
		void Parse(const std::string& key, const Json::Value& value);
		void Parse(const Json::Value& root);
		void TryParsers(const std::string& key, const Json::Value& value);

	private:
		bool _isClone{ false };
		SharedData* _clonedSD{ nullptr };
		void CleanClonedData();

		SharedData* _sharedData{ nullptr };
		Vector<IJsonParser*> _parsers;
	};
}