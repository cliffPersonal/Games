//
//  HUD.cpp
//  Lab12
//
//  Created by Clifford Lin on 11/26/21.
//

#include "HUD.h"
#include "Actor.h"
#include "Font.h"
#include "Shader.h"
#include "Texture.h"
#include "Game.h"
#include "Checkpoint.h"
#include "Renderer.h"
#include "SecurityCamera.h"
#include "Player.h"
#include <sstream>
#include <iomanip>

HUD::HUD(Actor* owner):
UIComponent(owner)
{
    mFont = new Font();
    mFont->Load("Assets/Inconsolata-Regular.ttf");
    
    float time = mOwner->GetGame()->GetTimer();
    int minutes = (int)(time / 60);
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << minutes;
    std::string min = ss.str();
    
    int endDigits = (int)(time * 100.0f) % 100;
    std::stringstream ss2;
    ss2 << std::setw(2) << std::setfill('0') << endDigits;
    std::string ed = ss2.str();
    
    int seconds = (int)(time - (minutes * 60));
    std::stringstream ss3;
    ss3 << std::setw(2) << std::setfill('0') << seconds;
    std::string sec = ss3.str();
    
    std::string strTime = min + ":" + sec + "." + ed;
    mTimerText = mFont->RenderText(strTime);

    std::string fin = std::to_string(owner->GetGame()->GetCoin()) + "/55";
    mCoinText = mFont->RenderText(fin);
    
    radar = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Radar.png");
    arrow = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/RadarArrow.png");
    blip = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Blip.png");
}

HUD::~HUD(){
    mFont->Unload();
    delete mFont;
}

void HUD::Update(float deltaTime){
    mOwner->GetGame()->IncrementTimer(deltaTime);
    float time = mOwner->GetGame()->GetTimer();
    
    mTimerText->Unload();
    delete mTimerText;
    
    int minutes = (int)(time / 60);
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << minutes;
    std::string min = ss.str();
    
    int endDigits = (int)(time * 100.0f) % 100;
    std::stringstream ss2;
    ss2 << std::setw(2) << std::setfill('0') << endDigits;
    std::string ed = ss2.str();
    
    int seconds = (int)(time - (minutes * 60));
    std::stringstream ss3;
    ss3 << std::setw(2) << std::setfill('0') << seconds;
    std::string sec = ss3.str();
    
    std::string strTime = min + ":" + sec + "." + ed;
    mTimerText = mFont->RenderText(strTime);
    
    if(textTimerRestart){
        textTimer = 0.0f;
        textTimerRestart = false;
        textShowing = true;
    }
    
    if(textShowing){
        textTimer += deltaTime;
    }
    
    if(textTimer >= 5.0f){
        mOwner->GetGame()->SetTexture(NULL);
        textShowing = false;
    }
    
    
    Vector2 player2D;
    player2D.x = mOwner->GetGame()->GetPlayer()->GetWorldPos().y;
    player2D.y = mOwner->GetGame()->GetPlayer()->GetWorldPos().x;
    Vector2 playerForward;
    playerForward.x = mOwner->GetGame()->GetPlayer()->GetWorldForward().x;
    playerForward.y = mOwner->GetGame()->GetPlayer()->GetWorldForward().y;
    float ang = Math::Atan2(playerForward.y, playerForward.x);
    Matrix3 rot = Matrix3::CreateRotation(ang);
    
    std::vector<SecurityCamera*> cameras =  mOwner->GetGame()->GetCameras();
    for(SecurityCamera* sc: cameras){
        Vector3 pto = sc->GetWorldPos() - mOwner->GetGame()->GetPlayer()->GetWorldPos();
        Vector2 playerToObject;
        playerToObject.x = pto.y;
        playerToObject.y = pto.x;
        if(playerToObject.Length() <= 1500.0f){
            playerToObject.x /= 1500.0f;
            playerToObject.y /= 1500.0f;
            playerToObject *= 92.0f;
            Vector2 fin = Vector2::Transform(playerToObject, rot);
            blips.push_back(fin);
            
            Vector2 objectForward;
            objectForward.x = sc->GetWorldForward().y;
            objectForward.y = sc->GetWorldForward().x;
            Vector2 finAngle = Vector2::Transform(objectForward, rot);
            float fAng = Math::Atan2(finAngle.y, finAngle.x);
            angles.push_back(fAng);
        }
    }
}

void HUD::Draw(Shader *shader){
    DrawTexture(shader, mTimerText, Vector2(-420.0f, -325.0f));
    DrawTexture(shader, mCoinText, Vector2(-435.0f, -295.0f));
    if(mOwner->GetGame()->GetTexture() != NULL){
        DrawTexture(shader, mOwner->GetGame()->GetTexture(), Vector2::Zero);
    }
    DrawTexture(shader, radar, Vector2(BOTPOSX, BOTPOSY));
    DrawTexture(shader, arrow, Vector2(BOTPOSX, BOTPOSY));
    
    for(int i = 0; i < (int)blips.size(); i++){
        DrawTexture(shader, blip, Vector2(BOTPOSX, BOTPOSY) + blips[i], 1.0f, angles[i]);
    }
    blips.clear();
    angles.clear();
}

void HUD::CoinUpdate(){
    int cCount = mOwner->GetGame()->IncrementCoin();
    mCoinText->Unload();
    delete mCoinText;
    std::string fin = std::to_string(cCount) + "/55";
    mCoinText = mFont->RenderText(fin);
}

void HUD::CheckpointText(){
    Actor* a = mOwner->GetGame()->GetFront();
    Checkpoint* c = static_cast<Checkpoint*>(a);
    std::string cText = c->GetText();
    if(cText.length() > 0){
        if(mOwner->GetGame()->GetTexture() != NULL){
            mOwner->GetGame()->GetTexture()->Unload();
            delete mOwner->GetGame()->GetTexture();
        }
        Texture* mCheckpointText = mFont->RenderText(cText);
        mOwner->GetGame()->SetTexture(mCheckpointText);
    }
}
