#pragma once
#include "SList.h"
#include "Vector.h"
#include "HashMap.h"
#include "Datum.h"
#include "Scope.h"
#include "Foo.h"
#include "AttributedFoo.h"
#include "AbsolutePerfectionAttributedFoo.h"
#include "JsonParseManager.h"
#include "IJsonParser.h"
#include "JsonParserInteger.h"
#include "JsonParserString.h"
#include "JsonParserVec.h"
#include "JsonParserScope.h"
#include "GameObject.h"
#include "GOFoo.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionList_IF.h"
#include "Action_CreateAction.h"
#include "Action_DestroyAction.h"
#include "GameTime.h"
#include "Event.h"
#include "IEventPublisher.h"

using namespace std::chrono_literals;
using namespace FIEAGameEngine;
using namespace LibraryDesktopTests;
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template <typename T> inline std::wstring ToString(const SList<T>& t) { RETURN_WIDE_STRING(t.Size()); }
	template <typename T> inline std::wstring ToString(const SList<T>* t) { RETURN_WIDE_STRING(t); }
	template <typename T> inline std::wstring ToString(SList<T>* t) { RETURN_WIDE_STRING(t); }

	template <typename T> inline std::wstring ToString(const Vector<T>& t) { RETURN_WIDE_STRING(t.Size()); }
	template <typename T> inline std::wstring ToString(const Vector<T>* t) { RETURN_WIDE_STRING(t); }
	template <typename T> inline std::wstring ToString(Vector<T>* t) { RETURN_WIDE_STRING(t); }

	template <typename T, typename Q> inline std::wstring ToString(const HashMap<T, Q>& t) { RETURN_WIDE_STRING(t.Size()); }
	template <typename T, typename Q> inline std::wstring ToString(const HashMap<T, Q>* t) { RETURN_WIDE_STRING(t); }
	template <typename T, typename Q> inline std::wstring ToString(HashMap<T, Q>* t) { RETURN_WIDE_STRING(t); }

	template <typename T, typename Q> inline std::wstring ToString(const std::pair<T, Q>& t) { try { return ToString(t.second); } catch (...) { return L"nullptr"; } }
	template <typename T, typename Q> inline std::wstring ToString(const std::pair<T, Q>* t) { try { return ToString(t->second); } catch (...) { return L"nullptr"; } }
	template <typename T, typename Q> inline std::wstring ToString(std::pair<T, Q>* t) { try { return ToString(t->second); } catch (...) { return L"nullptr"; } }

	template<> inline std::wstring ToString<Foo>(const Foo& t) { RETURN_WIDE_STRING(t.Data()); }
	template<> inline std::wstring ToString<Foo>(const Foo* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<Foo>(Foo* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString(const SList<Foo>::Iterator& t) { try { return ToString(*t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(const SList<Foo>::Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(SList<Foo>::Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }

	template<> inline std::wstring ToString(const SList<Foo>::Const_Iterator& t) { try { return ToString(*t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(const SList<Foo>::Const_Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(SList<Foo>::Const_Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }

	template<> inline std::wstring ToString(const Vector<Foo>::Iterator& t) { try { return ToString(*t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(const Vector<Foo>::Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(Vector<Foo>::Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }

	template<> inline std::wstring ToString(const Vector<Foo>::Const_Iterator& t) { try { return ToString(*t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(const Vector<Foo>::Const_Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(Vector<Foo>::Const_Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }

	template<> inline std::wstring ToString(const HashMap<Foo, Foo>::Iterator& t) { try { return ToString(*t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(const HashMap<Foo, Foo>::Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(HashMap<Foo, Foo>::Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }

	template<> inline std::wstring ToString(const HashMap<Foo, Foo>::Const_Iterator& t) { try { return ToString(*t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(const HashMap<Foo, Foo>::Const_Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(HashMap<Foo, Foo>::Const_Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }

	template<> inline std::wstring ToString(const HashMap<int, Foo>::Iterator& t) { try { return ToString(*t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(const HashMap<int, Foo>::Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(HashMap<int, Foo>::Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }

	template<> inline std::wstring ToString(const HashMap<int, Foo>::Const_Iterator& t) { try { return ToString(*t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(const HashMap<int, Foo>::Const_Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(HashMap<int, Foo>::Const_Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }

	template<> inline std::wstring ToString(const HashMap<int, int>::Iterator& t) { try { return ToString(*t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(const HashMap<int, int>::Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(HashMap<int, int>::Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }

	template<> inline std::wstring ToString(const HashMap<int, int>::Const_Iterator& t) { try { return ToString(*t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(const HashMap<int, int>::Const_Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }
	template<> inline std::wstring ToString(HashMap<int, int>::Const_Iterator* t) { try { return ToString(t); } catch (...) { return L"nullptr"; } }

	template<> inline std::wstring ToString<enum DatumTypes>(const enum DatumTypes& t) { return ToString(static_cast<std::size_t>(t)); }
	template<> inline std::wstring ToString<enum DatumTypes>(const enum DatumTypes* t) { return ToString(static_cast<std::size_t>(*t)); }
	template<> inline std::wstring ToString<enum DatumTypes>(enum DatumTypes* t) { return ToString(static_cast<std::size_t>(*t)); }

	template<> inline std::wstring ToString<glm::vec4>(const glm::vec4& t) { try { RETURN_WIDE_STRING(std::to_wstring(t.length())); } catch (...) { return L"VEC4"; } }
	template<> inline std::wstring ToString<glm::vec4>(const glm::vec4* t) { try { RETURN_WIDE_STRING(std::to_wstring(t->length())); } catch (...) { return L"VEC4"; } }
	template<> inline std::wstring ToString<glm::vec4>(glm::vec4* t) { try { RETURN_WIDE_STRING(std::to_wstring(t->length())); } catch (...) { return L"VEC4"; } }

	template<> inline std::wstring ToString<glm::mat4>(const glm::mat4& t) { try { RETURN_WIDE_STRING(std::to_wstring(t.length())); } catch (...) { return L"MAT4"; } }
	template<> inline std::wstring ToString<glm::mat4>(const glm::mat4* t) { try { RETURN_WIDE_STRING(std::to_wstring(t->length())); } catch (...) { return L"MAT4"; } }
	template<> inline std::wstring ToString<glm::mat4>(glm::mat4* t) { try { RETURN_WIDE_STRING(std::to_wstring(t->length())); } catch (...) { return L"MAT4"; } }

	template<> inline std::wstring ToString<Datum>(const Datum& t) { return ToString(t.Type()); }
	template<> inline std::wstring ToString<Datum>(const Datum* t) { return ToString(t->Type()); }
	template<> inline std::wstring ToString<Datum>(Datum* t) { return ToString(t->Type()); }

	template <> inline std::wstring ToString<RTTI>(RTTI* t) { try { RETURN_WIDE_STRING(std::to_wstring(t->TypeIdInstance())); } catch (...) { return L"RTTI"; } }
	template <> inline std::wstring ToString<RTTI>(const RTTI* t) { try { RETURN_WIDE_STRING(std::to_wstring(t->TypeIdInstance())); } catch (...) { return L"RTTI"; } }

	template<> inline std::wstring ToString<Scope>(const Scope& t) { try { return ToString(t.ToString()); } catch (...) { return L"Scope"; } }
	template<> inline std::wstring ToString<Scope>(const Scope* t) { try { return ToString(t->ToString()); } catch (...) { return L"Scope"; } }
	template<> inline std::wstring ToString<Scope>(Scope* t) { try { return ToString(t->ToString()); } catch (...) { return L"Scope"; } }

	template<> inline std::wstring ToString<Attributed>(const Attributed& t) { try { RETURN_WIDE_STRING(t.ToString().c_str()); } catch (...) { return L"Attributed"; } }
	template<> inline std::wstring ToString<Attributed>(const Attributed* t) { try { RETURN_WIDE_STRING(t->ToString().c_str()); } catch (...) { return L"Attributed"; } }
	template<> inline std::wstring ToString<Attributed>(Attributed* t) { try { RETURN_WIDE_STRING(t->ToString().c_str()); } catch (...) { return L"Attributed"; } }

	template<> inline std::wstring ToString<AttributedFoo>(const AttributedFoo& t) { try { RETURN_WIDE_STRING(t.ToString().c_str()); } catch (...) { return L"AttributedFoo"; } }
	template<> inline std::wstring ToString<AttributedFoo>(const AttributedFoo* t) { try { RETURN_WIDE_STRING(t->ToString().c_str()); } catch (...) { return L"AttributedFoo"; } }
	template<> inline std::wstring ToString<AttributedFoo>(AttributedFoo* t) { try { RETURN_WIDE_STRING(t->ToString().c_str()); } catch (...) { return L"AttributedFoo"; } }

	template<> inline std::wstring ToString<AbsolutePerfectionAttributedFoo>(const AbsolutePerfectionAttributedFoo& t) { try { RETURN_WIDE_STRING(t.ToString().c_str()); } catch (...) { return L"AbsolutePerfectionAttributedFoo"; } }
	template<> inline std::wstring ToString<AbsolutePerfectionAttributedFoo>(const AbsolutePerfectionAttributedFoo* t) { try { RETURN_WIDE_STRING(t->ToString().c_str()); } catch (...) { return L"AbsolutePerfectionAttributedFoo"; } }
	template<> inline std::wstring ToString<AbsolutePerfectionAttributedFoo>(AbsolutePerfectionAttributedFoo* t) { try { RETURN_WIDE_STRING(t->ToString().c_str()); } catch (...) { return L"AbsolutePerfectionAttributedFoo"; } }

	template<> inline std::wstring ToString<JsonParseManager>(const JsonParseManager& t) { RETURN_WIDE_STRING(&t); }
	template<> inline std::wstring ToString<JsonParseManager>(const JsonParseManager* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<JsonParseManager>(JsonParseManager* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<JsonParseManager::SharedData>(const JsonParseManager::SharedData& t) { RETURN_WIDE_STRING(&t); }
	template<> inline std::wstring ToString<JsonParseManager::SharedData>(const JsonParseManager::SharedData* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<JsonParseManager::SharedData>(JsonParseManager::SharedData* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<IJsonParser>(const IJsonParser* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<IJsonParser>(IJsonParser* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<JsonParserInteger>(const JsonParserInteger& t) { RETURN_WIDE_STRING(&t); }
	template<> inline std::wstring ToString<JsonParserInteger>(const JsonParserInteger* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<JsonParserInteger>(JsonParserInteger* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<JsonParserInteger::SharedData>(const JsonParserInteger::SharedData* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<JsonParserInteger::SharedData>(const JsonParserInteger::SharedData& t) { RETURN_WIDE_STRING(&t); }
	template<> inline std::wstring ToString<JsonParserInteger::SharedData>(JsonParserInteger::SharedData* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<JsonParserString>(const JsonParserString& t) { RETURN_WIDE_STRING(&t); }
	template<> inline std::wstring ToString<JsonParserString>(const JsonParserString* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<JsonParserString>(JsonParserString* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<JsonParserString::SharedData>(const JsonParserString::SharedData* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<JsonParserString::SharedData>(const JsonParserString::SharedData& t) { RETURN_WIDE_STRING(&t); }
	template<> inline std::wstring ToString<JsonParserString::SharedData>(JsonParserString::SharedData* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<JsonParserVec>(const JsonParserVec& t) { RETURN_WIDE_STRING(&t); }
	template<> inline std::wstring ToString<JsonParserVec>(const JsonParserVec* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<JsonParserVec>(JsonParserVec* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<JsonParserVec::SharedData>(const JsonParserVec::SharedData& t) { RETURN_WIDE_STRING(&t); }
	template<> inline std::wstring ToString<JsonParserVec::SharedData>(const JsonParserVec::SharedData* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<JsonParserVec::SharedData>(JsonParserVec::SharedData* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<JsonParserScope>(const JsonParserScope& t) { RETURN_WIDE_STRING(&t); }
	template<> inline std::wstring ToString<JsonParserScope>(const JsonParserScope* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<JsonParserScope>(JsonParserScope* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<JsonParserScope::SharedData>(const JsonParserScope::SharedData& t) { RETURN_WIDE_STRING(&t); }
	template<> inline std::wstring ToString<JsonParserScope::SharedData>(const JsonParserScope::SharedData* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<JsonParserScope::SharedData>(JsonParserScope::SharedData* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<FooFactory>(const FooFactory& t) { RETURN_WIDE_STRING(&t); }
	template<> inline std::wstring ToString<FooFactory>(const FooFactory* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<FooFactory>(FooFactory* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<GameObject>(const GameObject& t) { RETURN_WIDE_STRING(t.ToString().c_str()); }
	template<> inline std::wstring ToString<GameObject>(const GameObject* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<GameObject>(GameObject* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<GOFoo>(const GOFoo& t) { RETURN_WIDE_STRING(t.ToString().c_str()); }
	template<> inline std::wstring ToString<GOFoo>(const GOFoo* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<GOFoo>(GOFoo* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<Action>(const Action& t) { RETURN_WIDE_STRING(t.ToString().c_str()); }
	template<> inline std::wstring ToString<Action>(const Action* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<Action>(Action* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<ActionList>(const ActionList& t) { RETURN_WIDE_STRING(t.ToString().c_str()); }
	template<> inline std::wstring ToString<ActionList>(const ActionList* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<ActionList>(ActionList* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<ActionList_IF>(const ActionList_IF& t) { RETURN_WIDE_STRING(t.ToString().c_str()); }
	template<> inline std::wstring ToString<ActionList_IF>(const ActionList_IF* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<ActionList_IF>(ActionList_IF* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<Action_CreateAction>(const Action_CreateAction& t) { RETURN_WIDE_STRING(t.ToString().c_str()); }
	template<> inline std::wstring ToString<Action_CreateAction>(const Action_CreateAction* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<Action_CreateAction>(Action_CreateAction* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<Action_DestroyAction>(const Action_DestroyAction& t) { RETURN_WIDE_STRING(t.ToString().c_str()); }
	template<> inline std::wstring ToString<Action_DestroyAction>(const Action_DestroyAction* t) { RETURN_WIDE_STRING(t); }
	template<> inline std::wstring ToString<Action_DestroyAction>(Action_DestroyAction* t) { RETURN_WIDE_STRING(t); }

	template<> inline std::wstring ToString<std::chrono::high_resolution_clock::time_point>(const std::chrono::high_resolution_clock::time_point&) { RETURN_WIDE_STRING("TIMEPOINT"); }
	template<> inline std::wstring ToString<std::chrono::high_resolution_clock::time_point>(const std::chrono::high_resolution_clock::time_point*) { RETURN_WIDE_STRING("TIMEPOINT"); }
	template<> inline std::wstring ToString<std::chrono::high_resolution_clock::time_point>(std::chrono::high_resolution_clock::time_point*) { RETURN_WIDE_STRING("TIMEPOINT"); }

	template<> inline std::wstring ToString<std::chrono::milliseconds>(const std::chrono::milliseconds&) { RETURN_WIDE_STRING("MILISECONDS"); }
	template<> inline std::wstring ToString<std::chrono::milliseconds>(const std::chrono::milliseconds*) { RETURN_WIDE_STRING("MILISECONDS"); }
	template<> inline std::wstring ToString<std::chrono::milliseconds>(std::chrono::milliseconds*) { RETURN_WIDE_STRING("MILISECONDS"); }

	template<> inline std::wstring ToString<Event<Foo>>(const Event<Foo>& t) { RETURN_WIDE_STRING(t.ToString().c_str()); }
	template<> inline std::wstring ToString<Event<Foo>>(const Event<Foo>* t) { RETURN_WIDE_STRING(t->ToString().c_str()); }
	template<> inline std::wstring ToString<Event<Foo>>(Event<Foo>* t) { RETURN_WIDE_STRING(t->ToString().c_str()); }

	template<> inline std::wstring ToString<IEventPublisher>(const IEventPublisher& t) { RETURN_WIDE_STRING(t.ToString().c_str()); }
	template<> inline std::wstring ToString<IEventPublisher>(const IEventPublisher* t) { RETURN_WIDE_STRING(t->ToString().c_str()); }
	template<> inline std::wstring ToString<IEventPublisher>(IEventPublisher* t) { RETURN_WIDE_STRING(t->ToString().c_str()); }
}