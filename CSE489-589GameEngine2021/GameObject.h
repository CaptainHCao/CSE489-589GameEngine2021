#pragma once

#include <algorithm>

#include "SceneGraphNode.h"

/**
 * @enum	State
 *
 * @brief	An enumeration of the states of a game object.
 *
 * 			ACTIVE - Game object is in scene graph, and is is updated and "rendered"
 * 			PAUSED - Game object is in scene graph, but is not updated or "rendered"
 * 			DEAD - Game object will be removed from the game.
 */
enum STATE { ACTIVE, PAUSED, DEAD };

struct ReparentPair
{
	class GameObject* newParent;
	class GameObject* child;

	ReparentPair(class GameObject* newParent, class GameObject* child)
	{
		this->newParent = newParent;
		this->child = child;
	}
};


class GameObject : public SceneGraphNode
{
public:

	friend class Component;

	/**
	 * @fn	GameObject::GameObject(class Game * game);
	 *
	 * @brief	Constructor
	 *
	 * @param [in}	game that this game object will be part of
	 */
	GameObject();

	/**
	 * @fn	virtual ~GameObject();
	 *
	 * @brief	Destructor. Deletes all dynamically allocated memory.
	 */
	virtual ~GameObject();

	/**
	 * @fn	virtual void initialize();
	 *
	 * @brief	Initializes this game object and all attached components
	 * 			and any children of the game object.
	 *
	 */
	void initialize();

	/**
	 * @fn	void Update(float deltaTime);
	 *
	 * @brief	Update function called from Game (not overridable). Updates
	 * 			this game object and all attached components.
	 *
	 * @param	deltaTime	The time since the last update in seconds.
	 */
	virtual void update(const float& deltaTime);

	/**
	 * @fn	void ProcessInput();
	 *
	 * @brief	ProcessInput method called from Game (not overridable).
	 * 			Processes input for this game object and all attached.
	 */
	void processInput();

	/**
	 * @fn	void AddComponent(class Component* component);
	 *
	 * @brief	Attaches a component to this game object
	 *
	 * @param [in,out]	component	If non-null, the component to
	 * 					attached.
	 */
	void addComponent(class Component* component);

	/**
	 * @fn	void removeComponent(class Component* component);
	 *
	 * @brief	Detaches the component described by component from this
	 * 			game object
	 *
	 * @param [in,out]	component	If non-null, the component to be
	 * 					detached.
	 */
	void removeComponent(class Component* component);

	/**
	 * @fn	State getState() const
	 *
	 * @brief	Gets the state of the game object.
	 *
	 * @returns	The state of the game object.
	 */
	STATE getState() const { return gameObjectState; }

	/**
	 * @fn	void setState(State state);
	 *
	 * @brief	Sets a state of the game object. Only ACTIVE game
	 * 			objects are updated and rendered. When the state of
	 * 			an object is set to DEAD, it will be deleted on the
	 * 			next update cycle.
	 *
	 * @param	state	The state to which the object is being set
	 */
	void setState(STATE state);

	///**
	// * @fn	class Game* getOwningGame()
	// *
	// * @brief	Retrieves the owning game instance.
	// *
	// * @return	Game*	The game which owns this game object.
	// */
	//class Game* getOwningGame() { return owningGame; }

	/**
	 * @fn	std::vector<class std::Component*>getComponents()
	 *
	 * @brief	Returns the data structure containing all the
	 * 			Components attached to this GameObject.
	 *
	 * @return	vector containing the components.
	 */
	std::vector<class Component*>getComponents() { return components; }

	/**
	 * @fn	void Game::addGameObject(class GameObject* gameObject);
	 *
	 * @brief	Adds a game object the game. The game object must not already be parented.
	 *
	 * @param [in]	gameObject	If non-null, the game object to be added.
	 */
	/*static*/ void addChildGameObject(class GameObject* gameObject);

	/**
	 * @fn	void removeAndDelete();
	 *
	 * @brief	Marks the GameObject for removal from the scene graph and deletion.
	 */
	void removeAndDelete();

	static void SetOwningGame(class Game* owningGame) { GameObject::owningGame = owningGame; }

	static class Game* getOwningGame() { return owningGame; }

	/*static */std::vector<class GameObject*> GetChildren();

	/** @brief	Name of the game object */
	std::string gameObjectName = "GameObject";

	/**
	 * @fn	void reparent(class GameObject* child);
	 *
	 * @brief	Adds a child to this GameObject. Removes the child from the children of the former
	 * 			parent. Local transformation is set so that the position and orientation relative to
	 * 			the World reference frame is unchanged. Set the parent of the child to "this"
	 * 			GameObject.
	 *
	 * @param [in]	child	- child that is being added.
	 */
	void reparent(class GameObject* child);

	/**
	 * @fn	static void manageGameObjectChanges();
	 *
	 * @brief	Manage game object changes
	 */
	static void ManageGameObjectChanges();

protected:

	/**
	 * @fn	static void RemoveDeletedGameObjects();
	 *
	 * @brief	Removes the game objects that were deleted during the last
	 * 			update cycle.
	 */
	static void RemoveDeletedGameObjects();

	/**
	 * @fn	static void AddPendingGameObjects();
	 *
	 * @brief	Insert game objects into the scene graph that were added 
	 * 			during the last update cycle.
	 */
	static void AddPendingGameObjects();

	/**
	 * @fn	static void ReparentGameObjects();
	 *
	 * @brief	Complete reparenting of game objects that were
	 * 			reparented dusing the last update cycle.
	 */
	static void ReparentGameObjects();

	/**
	 * @fn	void deleteAllChildren();
	 *
	 * @brief	Deletes all the children of the GameObject.
	 */
	/*static */void deleteAllChildren();


	/**
	* @fn	virtual void GameObjectInput();
	*
	* @brief	Any GameObject-specific input code (overridable). Called
	* 			by processInput
	*/
	//virtual void gameObjectInput();

	/**
	 * @fn	void updateGameObject(float deltaTime)
	 *
	 * @brief	Game object specific update based on deltaTime
	 *
	 * @param 	deltaTime	The delta time.
	 */
	//void updateGameObject(const float & deltaTime);

	  /** @brief	Current state of the game object */
	STATE gameObjectState = ACTIVE;

	/** @brief	The components that are attached to this game object. */
	std::vector<class Component*> components;

	/**
	 * @class	Game*
	 *
	 * @brief	The game that this game object is part of.
	 */
	static class Game* owningGame;

	/** @brief	All the GameObjects in the Game */
	/*static */std::vector<class GameObject*> children;

	/** @brief	Any pending GameObjects that need to be added to the
	scene graph on the next update cycle. */
	static std::vector<class GameObject*> pendingChildren;

	/** @brief	The dead game objects that need to be removed from
	the game on the next update cycle. */
	static std::vector<class GameObject*> removedGameObjects;

	/** @brief	The game objects that are be attached to new
	parents after this update cycle.*/
	static std::vector<ReparentPair> reparentedGameObjects;

}; // end GameObject class


