#include "pch.h"
#include "JsonParserScope.h"
#include "GameObject.h"
#include "ActionList.h"

namespace FIEAGameEngine
{
#pragma region SHARED_DATA
	RTTI_DEFINITIONS(JsonParserScope::SharedData, FIEAGameEngine::JsonParseManager::SharedData);

	void JsonParserScope::SharedData::Initialize()
	{
		JsonParseManager::SharedData::Initialize();
		Data.Clear();
	}

	gsl::owner<JsonParserScope::SharedData*> JsonParserScope::SharedData::Create() const
	{
		return new JsonParserScope::SharedData();
	}
#pragma endregion

	RTTI_DEFINITIONS(JsonParserScope, IJsonParser);

	gsl::owner<IJsonParser*> JsonParserScope::Create() const
	{
		return new JsonParserScope();
		
	}

	void JsonParserScope::Initialize()
	{
		IJsonParser::Initialize();
	}

	bool JsonParserScope::StartHandler(FIEAGameEngine::JsonParseManager::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		JsonParserScope::SharedData* sdata = sharedData.As<JsonParserScope::SharedData>();
		if (sdata == nullptr) return false;

		if (key.empty()) _stack.Push({ "root"s, **(sdata->Data.PushBack(std::make_shared<GameObject>())) });
		else
		{
			ContextPair& TOP = _stack.Top();

			if (value.isObject())
			{
				if (key == "value"s)
				{
					const std::string& top_key = TOP.key;
					if (top_key == "Children"s && TOP.scope.Is(GameObject::TypeIdClass()))
					{
						GameObject* go{ nullptr };

						const Factory<Scope>* foundclass = Factory<Scope>::Find(value["class"s].asString());
						if (foundclass != nullptr && foundclass->IsGameObject()) go = reinterpret_cast<GameObject*>(Factory<Scope>::Create(value["class"s].asString()));
						else go = reinterpret_cast<GameObject*>(Factory<Scope>::Create("GameObject"s));

						reinterpret_cast<GameObject&>(TOP.scope).AddChild(*go);
						_stack.Push({ top_key, *go });
					}
					else if (top_key == "Actions"s && (TOP.scope.Is(GameObject::TypeIdClass()) || TOP.scope.Is(ActionList::TypeIdClass())))
					{
						Action* action = Factory<Action>::Create(value["class"s].asString());
						if (action == nullptr) throw std::runtime_error("Action Class Not Found!");

						if (TOP.scope.Is(GameObject::TypeIdClass())) reinterpret_cast<GameObject&>(TOP.scope).AddAction(*action);
						else reinterpret_cast<ActionList&>(TOP.scope).AddAction(*action);
						_stack.Push({ top_key, *action });
					}
					else
					{
						Scope* scope = Factory<Scope>::Create(value["class"s].asString());
						if (scope == nullptr) scope = new Scope;

						TOP.scope.Adopt(*scope, top_key);
						_stack.Push({ top_key, *scope });
					}
				}
				else
				{
					if (key == "Transform"s) _stack.Push({ key, *(TOP.scope.Find(key)->Get<Scope*>()) });
					else _stack.Push({ key, TOP.scope });
					array_index = 0;
				}
			}
			else
			{
				if (key == "type"s)
				{
					TOP.scope.Append(TOP.key).SetType(GetDatumType(value.asString()));
				}
				else if (key != "class"s)
				{
					Datum& datum = TOP.scope.Append(TOP.key);
					if (!datum.IsExternalStorage()) datum.PushBackFromString(value.asString());
					else datum.SetFromString(value.asString(), array_index++);
				}
			}
		}

		return true;
	}

	bool JsonParserScope::EndHandler(FIEAGameEngine::JsonParseManager::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		JsonParserScope::SharedData* sdata = sharedData.As<JsonParserScope::SharedData>();
		if (sdata == nullptr) return false;

		if (!key.empty() && value.isObject()) _stack.Pop();

		return true;
	}

	DatumTypes JsonParserScope::GetDatumType(const std::string& key)
	{
		DatumTypes type = DatumTypes::Unknown;
		HashMap<std::string, DatumTypes>::Const_Iterator itr = Datum::DatumStringToType.Find(key);
		if (itr != Datum::DatumStringToType.cend()) type = itr->second;
		return type;
	}
}