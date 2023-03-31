#include "GhostAI.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include "Ghost.h"
#include "PacMan.h"
#include "Random.h"

GhostAI::GhostAI(class Actor* owner)
:Component(owner, 50)
{
	mGhost = static_cast<Ghost*>(owner);
}

void GhostAI::Update(float deltaTime)
{
    if(stateTimer > 0.0f){
        stateTimer -= deltaTime;
    }
    if(chaseTimer > 0.0f && !frightened && !dead){
        chaseTimer -= deltaTime;
    }
    
    Vector2 prevPos = mPrevNode->GetPosition();
    Vector2 nextPos = mNextNode->GetPosition();
    SetDirection(prevPos, nextPos);
    
    Vector2 ghostPos = mGhost->GetPosition();
    ghostPos += forwardSpeed * deltaTime * dir;
    mGhost->SetPosition(ghostPos);
    
    CollisionComponent* gcc = mGhost->GetComponent<CollisionComponent>();
    CollisionComponent* ncc = mNextNode->GetComponent<CollisionComponent>();
    bool needUpdatePath = false;
    if(gcc->Intersect(ncc)){
        ghostPos = mNextNode->GetPosition();
        mGhost->SetPosition(ghostPos);
        if(mNextNode == mGhost->GetGame()->mGhostPen && dead){
            dead = false;
            forwardSpeed = SCATTERSPEED;
            Start(mGhost->GetGame()->mGhostPen);
        }
        else{
            needUpdatePath = UpdatePath();
            if(!needUpdatePath){
                mPrevNode = mNextNode;
                mNextNode = mPath.back();
                mPath.pop_back();
            }
        }
        
    }
    SetAnimations();
}


void GhostAI::Frighten()
{
    frightened = true;
    if(mState == Dead){
        return;
    }
    stateTimer = FRIGHTENTIME;
    mState = Frightened;
    PathNode* temp = mNextNode;
    mNextNode = mPrevNode;
    mPrevNode = temp;
    mPath.clear();
    
    UpdatePath();

}

void GhostAI::Start(PathNode* startNode)
{
    mOwner->SetPosition(startNode->GetPosition());
    mState = State::Scatter;
    mPrevNode = nullptr;
    mNextNode = nullptr;
    mTargetNode = nullptr;
    mPath.clear();
    AStar(startNode, mGhost->GetScatterNode());
    stateTimer = 0.0f;
    chaseTimer = SCATTER;
}

void GhostAI::Die()
{
    mState = Dead;
    stateTimer = 0.0f;
    forwardSpeed = DEADSPEED;
    dead = true;
    frightened = false;
}

void GhostAI::DebugDrawPath(SDL_Renderer* render)
{
	// Draw a rectangle at the target node
	if (mTargetNode != nullptr)
	{
		const int SIZE = 16;
		SDL_Rect r;
		r.x = static_cast<int>(mTargetNode->GetPosition().x) - SIZE / 2;
		r.y = static_cast<int>(mTargetNode->GetPosition().y) - SIZE / 2;
		r.w = SIZE;
		r.h = SIZE;
		SDL_RenderDrawRect(render, &r);
	}

	// Line from ghost to next node
	if (mNextNode != nullptr)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mOwner->GetPosition().x),
			static_cast<int>(mOwner->GetPosition().y),
			static_cast<int>(mNextNode->GetPosition().x),
			static_cast<int>(mNextNode->GetPosition().y));
	}

	// Exit if no path
	if (mPath.empty())
	{
		return;
	}

	if (mNextNode)
	{
		// Line from next node to subsequent on path
		SDL_RenderDrawLine(render,
			static_cast<int>(mNextNode->GetPosition().x),
			static_cast<int>(mNextNode->GetPosition().y),
			static_cast<int>(mPath.back()->GetPosition().x),
			static_cast<int>(mPath.back()->GetPosition().y));
	}

	// Lines for rest of path
	for (size_t i = 0; i < mPath.size() - 1; i++)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mPath[i]->GetPosition().x),
			static_cast<int>(mPath[i]->GetPosition().y),
			static_cast<int>(mPath[i + 1]->GetPosition().x),
			static_cast<int>(mPath[i + 1]->GetPosition().y));
	}
}

void GhostAI::AStar(PathNode* startNode, PathNode* scatterNode){
    mPath.clear();
    std::unordered_map<PathNode*, NodeInfo> info;
    PathNode* currentNode = startNode;
    std::vector<PathNode*> openset;
    if(startNode != scatterNode){
        info[currentNode].IsClosed = true;
    }
    info[startNode].unusable = mPrevNode;
    for(PathNode* n:currentNode->mAdjacent){
        if(n != mPrevNode){
            info[n].unusable = startNode;
        }
    }
    do{
        for(PathNode* n:currentNode->mAdjacent){
            if(n->GetType() != PathNode::Type::Tunnel){
                if(!info[n].IsClosed && info[currentNode].unusable != n){
                    if(openset.end() != std::find(openset.begin(), openset.end(), n)){
                        float new_g = info[currentNode].g + EdgeCost(currentNode, n);
                        if(new_g < info[n].g){
                            info[n].parent = currentNode;
                            info[n].g = new_g;
                            info[n].f = info[n].g + info[n].h;
                        }
                    }
                    else{
                        info[n].parent = currentNode;
                        info[n].h = EdgeCost(n, scatterNode);
                        info[n].g = info[currentNode].g + EdgeCost(currentNode, n);
                        info[n].f = info[n].g + info[n].h;
                        openset.push_back(n);
                    }
                }
            }
        }
        
        if(openset.empty()){
            break;
        }
        
        float smallest = info[openset[0]].f;
        currentNode = openset[0];
        for(PathNode* n: openset){
            if(info[n].f < smallest){
                smallest = info[n].f;
                currentNode = n;
            }
        }
        auto iter = std::find(openset.begin(), openset.end(), currentNode);
        openset.erase(iter);
        info[currentNode].IsClosed = true;
    }
    while(currentNode != scatterNode);
    
    PathNode* pth = scatterNode;
    do{
        mPath.push_back(pth);
        pth = info[pth].parent;
        
    }
    while(pth != startNode);
    
    mNextNode = mPath.back();
    mPath.pop_back();
    mTargetNode = scatterNode;
    mPrevNode = startNode;
    
}

float GhostAI::EdgeCost(class PathNode* from, class PathNode* to){
    float cost;
    cost = (to->GetPosition() - from->GetPosition()).Length();
    return cost;
}

bool GhostAI::UpdatePath(){
    if(chaseTimer < 0.0f && mState == Scatter){
        mState = Chase;
        chaseTimer = CHASE;
    }
    if(chaseTimer < 0.0f && mState == Chase){
        mState = Scatter;
        chaseTimer = SCATTER;
    }
    if(stateTimer < 0.0f){
        mState = State::Scatter;
        stateTimer = 0.0f;
        forwardSpeed = SCATTERSPEED;
    }
    
    if(mState == Scatter || mState == Dead){
        frightened = false;
        if(mPath.empty()){
            if(mState == Scatter){
                AStar(mNextNode, mGhost->GetScatterNode());
            }
            else{
                AStar(mNextNode, mGhost->GetGame()->mGhostPen);
            }
            return true;
        }
        else{
            return false;
        }
    }
    else if(mState == Frightened){
        if(stateTimer < FRIGHTENTIME){
            forwardSpeed = FRIGHTENED;
            bool defaultExists = false;
            std::unordered_map<PathNode*, NodeInfo> info;
            for(PathNode* n:mNextNode->mAdjacent){
                if(n != mPrevNode && n->GetType() != PathNode::Tunnel){
                    info[n].unusable = mNextNode;
                    if(n->GetType() == PathNode::Default){
                        defaultExists = true;
                    }
                }
            }
            
            if(defaultExists){
                int size = 0;
                for(auto i: info){
                    if(i.first->GetType() == PathNode::Default){
                        size++;
                    }
                }
                int rand = Random::GetIntRange(0, size - 1);
                int counter = 0;
                for(auto i: info){
                    if(i.first->GetType() == PathNode::Default){
                        if(counter == rand){
                            mPrevNode = mNextNode;
                            mNextNode = i.first;
                            break;
                        }
                        counter++;
                    }
                }
                
            }
            else{
                int rand = Random::GetIntRange(0, info.size());
                int counter = 0;
                for(auto i: info){
                    if(counter == rand){
                        mPrevNode = mNextNode;
                        mNextNode = i.first;
                        break;
                    }
                    counter++;
                }
            }
        }
        return true;
    }
    else if(mState == Chase){
        frightened = false;
        Ghost::Type t = mGhost->GetType();
        Vector2 pacpos = mGhost->GetGame()->mPlayer->GetPosition();
        if(t == Ghost::Blinky){
            PathNode* target = mGhost->GetGame()->mPlayer->GetPrevNode();
            if(target->GetType() == PathNode::Tunnel){
                target = GetClosestNode(pacpos);
            }
            mPath.clear();
            AStar(mNextNode, target);
        }
        else if(t == Ghost::Pinky){
            Vector2 targetv = mGhost->GetGame()->mPlayer->GetPointInFrontOf(80.0f);
            PathNode* target = GetClosestNode(targetv);
            mPath.clear();
            AStar(mNextNode, target);
        }
        else if(t == Ghost::Inky){
            Vector2 targetv = mGhost->GetGame()->mPlayer->GetPointInFrontOf(40.0f);
            Vector2 blinkyPos = mGhost->GetGame()->mGhosts[0]->GetPosition();
            Vector2 blinkyToTarget = 2 * (targetv - blinkyPos);
            blinkyToTarget += blinkyPos;
            
            PathNode* target = GetClosestNode(blinkyToTarget);
            mPath.clear();
            AStar(mNextNode, target);
        }
        else if(t == Ghost::Clyde){
            float playerToClyde = (pacpos - mGhost->GetPosition()).Length();
            if(playerToClyde > 150.0f){
                PathNode* target = mGhost->GetGame()->mPlayer->GetPrevNode();
                if(target->GetType() == PathNode::Tunnel){
                    target = GetClosestNode(pacpos);
                }
                mPath.clear();
                AStar(mNextNode, target);
            }
            else{
                mPath.clear();
                AStar(mNextNode, mGhost->GetScatterNode());
            }
        }
        return true;
    }
    return false;
}

PathNode* GhostAI::GetClosestNode(Vector2 pos){
    PathNode* closest = mNextNode;
    float smallestDist = (mNextNode->GetPosition() - pos).Length();
    for(PathNode* n: mGhost->GetGame()->mPathNodes){
        if(n->GetType() != PathNode::Default){
            continue;
        }
        if((n->GetPosition() - pos).Length() < smallestDist){
            smallestDist = (n->GetPosition() - pos).Length();
            closest = n;
        }
    }
    return closest;
}

void GhostAI::SetAnimations(){
    if(dir.y > 0){
        mGhost->GetComponent<AnimatedSprite>()->SetAnimation("down");
    }
    else if(dir.y < 0){
        mGhost->GetComponent<AnimatedSprite>()->SetAnimation("up");
    }
    else if(dir.x > 0){
        mGhost->GetComponent<AnimatedSprite>()->SetAnimation("right");
    }
    else if(dir.x < 0){
        mGhost->GetComponent<AnimatedSprite>()->SetAnimation("left");
    }
    if(dead){
        if(dir.y > 0){
            mGhost->GetComponent<AnimatedSprite>()->SetAnimation("deaddown");
        }
        else if(dir.y < 0){
            mGhost->GetComponent<AnimatedSprite>()->SetAnimation("deadup");
        }
        else if(dir.x > 0){
            mGhost->GetComponent<AnimatedSprite>()->SetAnimation("deadright");
        }
        else if(dir.x < 0){
            mGhost->GetComponent<AnimatedSprite>()->SetAnimation("deadleft");
        }
    }
    if(stateTimer < ENDFRIGHTENTIME && stateTimer > 0.0f){
        mGhost->GetComponent<AnimatedSprite>()->SetAnimation("scared1");
    }
    else if(stateTimer >= ENDFRIGHTENTIME){
        mGhost->GetComponent<AnimatedSprite>()->SetAnimation("scared0");
    }
}

void GhostAI::SetDirection(Vector2 prevPos, Vector2 nextPos){
    if(nextPos.y < prevPos.y){
        dir.x = 0.0f;
        dir.y = -1.0f;
    }
    else if(nextPos.y > prevPos.y){
        dir.x = 0.0f;
        dir.y = 1.0f;
    }
    else if(nextPos.x > prevPos.x){
        dir.x = 1.0f;
        dir.y = 0.0f;
    }
    else if(nextPos.x < prevPos.x){
        dir.x = -1.0f;
        dir.y = 0.0f;
    }
}
