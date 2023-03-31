#pragma once
#include "Component.h"
#include "Math.h"
#include <vector>
#include <unordered_map>

struct NodeInfo {
    // The parent is the node at "before" current node
    class PathNode* parent = nullptr;
    // f(x) = g(x) + h(x)
    float f = 0.0f;
    // g(x) true cost from start to this node
    float g = 0.0f;
    // h(x) heuristic estimate from this node to goal
    float h = 0.0f;
    // Whether or not this node is "closed"
    bool IsClosed = false;
    PathNode* unusable = nullptr;
};

class GhostAI : public Component
{
public:
	// Used to track the four different GhostAI states
	enum State
	{
		Scatter,
		Chase,
		Frightened,
		Dead
	};
	
	GhostAI(class Actor* owner);

	void Update(float deltaTime) override;
	
	// Called when the Ghost starts at the beginning
	// (or when the ghosts should respawn)
	void Start(class PathNode* startNode);
	
	// Get the current state
	State GetState() const { return mState; }
	
	// Called when the ghost should switch to the "Frightened" state
	void Frighten();
	
	// Called when the ghost should switch to the "Dead" state
	void Die();

	//  Helper function to draw GhostAI's current path
	void DebugDrawPath(struct SDL_Renderer* render);
private:
	// Member data for pathfinding

	// TargetNode is our current goal node
	class PathNode* mTargetNode = nullptr;
	// PrevNode is the last node we intersected
	// with prior to the current position
	class PathNode* mPrevNode = nullptr;
	// NextNode is the next node we're trying
	// to get to
	class PathNode* mNextNode = nullptr;

	// This vector always contains the path
	// from "next node" to "target node"
	// (if there is still such a path)
	std::vector<class PathNode*> mPath;

	// Current state of the Ghost AI
	State mState = Scatter;

	// Save the owning actor (cast to a Ghost*)
	class Ghost* mGhost;

	// TODO: Add any member data/helper functions here!
    float stateTimer = 0.0f;
    Vector2 dir = Vector2::Zero;
    float forwardSpeed = 90.0f;
    void AStar(class PathNode* startNode, class PathNode* scatterNode);
    float EdgeCost(class PathNode* from, class PathNode* to);
    class PathNode* GetClosestNode(Vector2 pos);
    void SetAnimations();
    void SetDirection(Vector2 prev, Vector2 next);
    bool UpdatePath();
    bool dead = false;
    bool frightened = false;
    float chaseTimer = 5.0f;
    const float SCATTER = 5.0f;
    const float CHASE = 20.0f;
    const float FRIGHTENED = 65.0f;
    const float SCATTERSPEED = 90.0f;
    const float DEADSPEED = 125.0f;
    const float FRIGHTENTIME = 7.0f;
    const float ENDFRIGHTENTIME = 2.0f;
};
