#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
    Vector2 otherMax = other->GetMax();
    Vector2 otherMin = other->GetMin();
    Vector2 thisMax = this->GetMax();
    Vector2 thisMin = this->GetMin();
    
    bool intersect = false;
    if(thisMax.x < otherMin.x){
        
    }
    else if(otherMax.x < thisMin.x){
        
    }
    else if(thisMax.y < otherMin.y){
        
    }
    else if(otherMax.y < thisMin.y){
        
    }
    else{
        intersect = true;
    }
	return intersect;
}

Vector2 CollisionComponent::GetMin() const
{
    Vector2 min;
    float x = mOwner->GetPosition().x;
    float y = mOwner->GetPosition().y;
    float scale = mOwner->GetScale();
    min.x = x - ((mWidth * scale) / 2.0f);
    min.y = y - ((mHeight * scale) / 2.0f);
    
	return min;
}

Vector2 CollisionComponent::GetMax() const
{
    Vector2 max;
    float x = mOwner->GetPosition().x;
    float y = mOwner->GetPosition().y;
    float scale = mOwner->GetScale();
    max.x = x + ((mWidth * scale) / 2.0f);
    max.y = y + ((mHeight * scale) / 2.0f);
    
    return max;
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector2& offset)
{
	offset = Vector2::Zero;
    if(!(this->Intersect(other))){
        return CollSide::None;
    }
	
    float otherMinYDiff = Math::Abs(other->GetMin().y - this->GetMax().y);
    float otherMaxYDiff = Math::Abs(other->GetMax().y - this->GetMin().y);
    float otherMinXDiff = Math::Abs(other->GetMin().x - this->GetMax().x);
    float otherMaxXDiff = Math::Abs(other->GetMax().x - this->GetMin().x);
    
    float smallest = otherMinYDiff;
    if(otherMaxYDiff < smallest){
        smallest = otherMaxYDiff;
    }
    if(otherMinXDiff < smallest){
        smallest = otherMinXDiff;
    }
    if(otherMaxXDiff < smallest){
        smallest = otherMaxXDiff;
    }
    
    if(smallest == otherMinYDiff){
        offset.y = -smallest;
        return CollSide::Top;
    }
    else if(smallest == otherMaxYDiff){
        offset.y = smallest;
        return CollSide::Bottom;
    }
    else if(smallest == otherMinXDiff){
        offset.x = -smallest;
        return CollSide::Left;
    }
    else{
        offset.x = smallest;
        return CollSide::Right;
    }
}
