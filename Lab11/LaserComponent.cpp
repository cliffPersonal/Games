//
//  LaserComponent.cpp
//  Lab10
//
//  Created by Clifford Lin on 11/8/21.
//

#include "LaserComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Player.h"
#include "Block.h"
#include "PlayerMove.h"

LaserComponent::LaserComponent(Actor* owner)
:MeshComponent(owner, true)
{
    SetMesh(owner->GetGame()->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
}

void LaserComponent::Update(float deltaTime){
    Vector3 pos = mOwner->GetWorldPos();
    Vector3 forward = mOwner->GetWorldForward();
    lsVector.clear();
    bool reflect = true;
    Actor* ignore = mOwner->GetParent();
    
    while(reflect){
        LineSegment ls;
        ls.mStart = pos;
        ls.mEnd = pos + forward * 500.0f;
        CastInfo outInfo;
        Actor* a = mOwner->GetGame()->GetPlayer();
        if(SegmentCast(a, ls, outInfo)){
            ls.mEnd = outInfo.mPoint;
            reflect = false;
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Laser.wav"), 0);
            mOwner->GetGame()->GetPlayer()->GetComponent<PlayerMove>()->Respawn();
        }
        else if(SegmentCast(mOwner->GetGame()->GetBlocks(), ls, outInfo, ignore)){
            ls.mEnd = outInfo.mPoint;
            Block* b = static_cast<Block*>(outInfo.mActor);
            if(b->GetIsMirror()){
                ignore = outInfo.mActor;
                pos = outInfo.mPoint;
                forward = Vector3::Reflect(forward, outInfo.mNormal);
            }
            else{
                reflect = false;
            }
        }
        else{
            reflect = false;
        }
        lsVector.push_back(ls);
    }
}

void LaserComponent::Draw(Shader *shader){
    if (mMesh)
    {
        // Set the active texture
        Texture* t = mMesh->GetTexture(mTextureIndex);
        if (t)
        {
            t->SetActive();
        }
        // Set the mesh's vertex array as active
        VertexArray* va = mMesh->GetVertexArray();
        va->SetActive();
        // Draw
        for(LineSegment l: lsVector){
            // Set the world transform
            shader->SetMatrixUniform("uWorldTransform",
                SegToMat(l));
            glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
        }
    }
}

Matrix4 LaserComponent::SegToMat(LineSegment seg){
    Matrix4 mWorldTransform;
    Matrix4 scaleMatrix = Matrix4::CreateScale(seg.Length(), 1.0f, 1.0f);
    Matrix4 rotationMatrix;
    Vector3 forwardNorm = seg.mEnd - seg.mStart;
    forwardNorm.Normalize();
    float dot = Vector3::Dot(Vector3::UnitX, forwardNorm);
    if(dot == 1.0f){
        rotationMatrix = Matrix4::CreateFromQuaternion(Quaternion::Identity);
    }
    else if(dot == -1.0f){
        rotationMatrix = Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ, Math::Pi));
    }
    else{
        float theta = Math::Acos(dot);
        Vector3 cross = Vector3::Cross(Vector3::UnitX, forwardNorm);
        float crossLength = cross.Length();
        Vector3 axis;
        axis = cross * (1.0f / crossLength);
        rotationMatrix = Matrix4::CreateFromQuaternion(Quaternion(axis, theta));
    }
    Matrix4 positionMatrix = Matrix4::CreateTranslation(seg.PointOnSegment(0.5f));
    mWorldTransform = scaleMatrix * rotationMatrix * positionMatrix;
    return mWorldTransform;
}
