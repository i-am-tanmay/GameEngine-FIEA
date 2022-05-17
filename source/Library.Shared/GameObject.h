#pragma once
#include "Attributed.h"
#include "TypeManager.h"
#include "GameTime.h"
#include "Action.h"

namespace FIEAGameEngine
{
	struct Transform final : public Attributed
	{
		RTTI_DECLARATIONS(Transform);

	public:
		glm::vec4 Position{ 0,0,0,0 };
		glm::vec4 Rotation{ 0,0,0,0 };
		glm::vec4 Scale{ 0,0,0,0 };

		Transform(Scope* parent = nullptr) :
			Attributed(Transform::TypeIdClass())
		{
			_parent = parent;
		}

		Transform(const Transform&) = default;
		Transform& operator=(const Transform&) = default;
		Transform(Transform&&) noexcept = default;
		Transform& operator=(Transform&&) noexcept = default;

		gsl::owner<Transform*> Clone() const override
		{
			return new Transform(*this);
		}

		static Vector<AttributeSignature> Signatures()
		{
			return Vector<AttributeSignature>
			{
				{ "Position", DatumTypes::Vector, 1u, offsetof(Transform, Position) },
				{ "Rotation", DatumTypes::Vector, 1u, offsetof(Transform, Rotation) },
				{ "Scale", DatumTypes::Vector, 1u, offsetof(Transform, Scale) }
			};
		}
	};

	class GameObject : public Attributed
	{
		RTTI_DECLARATIONS(GameObject);

	public:
		using size_type = std::size_t;

		

	public:

		/// <summary>
		/// Constructor: populates list of attributes for GameObject (Name, Position, Rotation, Scale, Children)
		/// </summary>
		GameObject();

		/// <summary>
		/// Copy construct: using another gameobject
		/// </summary>
		/// <param name="rhs">Reference to other gameobject to copy from</param>
		GameObject(const GameObject& rhs) = default;

		/// <summary>
		/// Copy assignment: using another gameobject
		/// </summary>
		/// <param name="rhs">Reference to other gameobject to copy from</param>
		/// <returns>Reference to new this gameobject</returns>
		GameObject& operator=(const GameObject& rhs) = default;

		/// <summary>
		/// Move construct: using other gameobject
		/// </summary>
		/// <param name="rhs">Reference to other gameobject to copy from</param>
		GameObject(GameObject&& rhs) noexcept = default;

		/// <summary>
		/// Move assignment: using other gameobject
		/// </summary>
		/// <param name="rhs">Reference to other gameobject to copy from</param>
		/// <returns>Reference to new this gameobject</returns>
		GameObject& operator=(GameObject&& rhs) noexcept = default;

		virtual ~GameObject() = default;

		/// <summary>
		/// Name for this GameObject
		/// </summary>
		std::string Name{ "GameObject" };

		Transform* transform{ &_transform };
		//std::shared_ptr<Transform> hello{ transform };

		/// <summary>
		/// Append a gameobject derived Child to this gameobject to call Update on
		/// </summary>
		/// <param name="child">Reference to child to add</param>
		void AddChild(GameObject& child);

		/// <summary>
		/// Append a action derived Action to this gameobject to call Update on
		/// </summary>
		/// <param name="action">Reference to action to add</param>
		void AddAction(Action& action);

		/// <summary>
		/// Remove an action derived Action from this gameobject Actions List
		/// </summary>
		/// <param name="action">Reference to action to remove</param>
		void RemoveAction(Action& action);

		/// <summary>
		/// Update gameobject every frame
		/// </summary>
		/// <param name="gametime">Reference to GameTime for current instance of game</param>
		virtual void Update(const GameTime& gametime);

		/// <summary>
		/// Create a new copy of this gameobject instance
		/// </summary>
		/// <returns>Pointer to new copied instance</returns>
		virtual gsl::owner<GameObject*> Clone() const override;

		/// <summary>
		/// Equality check
		/// </summary>
		/// <param name="rhs">Pointer to data to check equality with</param>
		/// <returns>TRUE if they are equal | FALSE if not</returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Convert to string
		/// </summary>
		/// <returns>Name of GameObject</returns>
		std::string ToString() const override;

		/// <summary>
		/// Get Signatures list for GameObject class
		/// </summary>
		/// <returns>Vector of Signatures of GameObject</returns>
		static Vector<AttributeSignature> Signatures();

	protected:
		GameObject(size_type _typeid);

	private:
		Vector<std::reference_wrapper<GameObject>> _children;
		Transform _transform{ this };

		Vector<std::reference_wrapper<Action>> _actions;
	};

	DefineFactory(GameObject, Scope);
}