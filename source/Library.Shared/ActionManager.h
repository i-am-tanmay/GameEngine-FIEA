#pragma once
#include "TypeManager.h"
#include "GameTime.h"
#include "Vector.h"
#include "Scope.h"

namespace FIEAGameEngine
{
	class Action;
	class ActionList;
	class GameObject;
	class ActionManager final
	{
	public:
		using size_type = std::size_t;

	public:
		ActionManager(const ActionManager&) = delete;
		ActionManager(ActionManager&&) = delete;
		ActionManager& operator=(const ActionManager&) = delete;
		ActionManager& operator=(ActionManager&&) = delete;

		/// <summary>
		/// TASK: Adopt Action into given parent
		/// </summary>
		/// <param name="parent">Reference to Gameobject to adopt into</param>
		/// <param name="child">Reference to child to adopt</param>
		static void CreateActionTask(GameObject& parent, Action& child);

		/// <summary>
		/// TASK: Adopt Action into given parent
		/// </summary>
		/// <param name="parent">Reference to ActionList to adopt into</param>
		/// <param name="child">Reference to child to adopt</param>
		static void CreateActionTask(ActionList& parent, Action& child);

		/// <summary>
		/// TASK: Destroy given action
		/// </summary>
		/// <param name="action">Reference to action to destroy</param>
		static void DestroyActionTask(Action& action);

		/// <summary>
		/// DO all the pending tasks
		/// </summary>
		static void Update();

		/// <summary>
		/// Cleanup All Vectors
		/// </summary>
		static void CleanUp();

	private:
		inline static Vector<std::pair<GameObject*, Action*>> _createQ_go;
		inline static Vector<std::pair<ActionList*, Action*>> _createQ_aclist;
		inline static Vector<Action*> _destroyQ;
	};
}