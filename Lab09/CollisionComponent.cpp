#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
,mDepth(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	Vector3 aMin = GetMin();
	Vector3 aMax = GetMax();
	Vector3 bMin = other->GetMin();
	Vector3 bMax = other->GetMax();

	bool no = aMax.x < bMin.x ||
		aMax.y < bMin.y ||
		aMax.z < bMin.z ||
		bMax.x < aMin.x ||
		bMax.y < aMin.y ||
		bMax.z < aMin.z;

	return !no;
}

Vector3 CollisionComponent::GetMin() const
{
	Vector3 v = mOwner->GetPosition();
	v.x -= mDepth * mOwner->GetScale() / 2.0f;
	v.y -= mWidth * mOwner->GetScale() / 2.0f;
	v.z -= mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetMax() const
{
	Vector3 v = mOwner->GetPosition();
	v.x += mDepth * mOwner->GetScale() / 2.0f;
	v.y += mWidth * mOwner->GetScale() / 2.0f;
	v.z += mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

const Vector3& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector3& offset)
{
	offset = Vector3::Zero;
    if(!(this->Intersect(other))){
        return CollSide::None;
    }
    
    float otherMinYDiff = Math::Abs(other->GetMin().y - this->GetMax().y);
    float otherMaxYDiff = Math::Abs(other->GetMax().y - this->GetMin().y);
    float otherMinXDiff = Math::Abs(other->GetMin().x - this->GetMax().x);
    float otherMaxXDiff = Math::Abs(other->GetMax().x - this->GetMin().x);
    float otherMinZDiff = Math::Abs(other->GetMin().z - this->GetMax().z);
    float otherMaxZDiff = Math::Abs(other->GetMax().z - this->GetMin().z);
    
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
    if(otherMinZDiff < smallest){
        smallest = otherMinZDiff;
    }
    if(otherMaxZDiff < smallest){
        smallest = otherMaxZDiff;
    }
    
    if(smallest == otherMinYDiff){
        offset.y = -smallest;
        return CollSide::Side;
    }
    else if(smallest == otherMaxYDiff){
        offset.y = smallest;
        return CollSide::Side;
    }
    else if(smallest == otherMinXDiff){
        offset.x = -smallest;
        return CollSide::Side;
        
    }
    else if(smallest == otherMaxXDiff){
        offset.x = smallest;
        return CollSide::Side;
        
    }
    else if(smallest == otherMinZDiff){
        offset.z = -smallest;
        return CollSide::Bottom;
        
    }
    else if(smallest == otherMaxZDiff){
        offset.z = smallest;
        return CollSide::Top;
        
    }
	return CollSide::None;
}
