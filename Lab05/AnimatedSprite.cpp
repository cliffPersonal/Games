#include "AnimatedSprite.h"
#include "Actor.h"
#include "Game.h"

AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
{
}

void AnimatedSprite::Update(float deltaTime)
{
    if(!mAnims.empty()){
        if(!mIsPaused){
            mAnimTimer += mAnimFPS * deltaTime;
            while(mAnimTimer >= mAnims[mAnimName].size()){
                mAnimTimer -= mAnims[mAnimName].size();
            }
        }
        this->SetTexture(mAnims[mAnimName][(int)mAnimTimer]);
    }
}

void AnimatedSprite::SetAnimation(const std::string& name, bool resetTimer)
{
	if (mAnimName != name)
	{
		mAnimName = name;
	}
	
	if (resetTimer)
	{
		mAnimTimer = 0.0f;
	}
}

void AnimatedSprite::AddAnimation(const std::string& name,
								  const std::vector<SDL_Texture*>& images)
{
	mAnims.emplace(name, images);
}
