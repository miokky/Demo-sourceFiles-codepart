#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"
#include "../core/SequenceFrameAnimation.h"
#include "../core/KeyboardEventHandler.h"
#include "../core/CharacterFactory.h"
#include "../core/BattleDataModel.h"
#include <string>

class BattleScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void update(float deltaTime);
    CREATE_FUNC(BattleScene);

    cocos2d::Node* playerHPHead;
    cocos2d::Node* playerHPBody;
    cocos2d::Node* playerHPEnd;

    cocos2d::Node* playerManaHead;
    cocos2d::Node* playerManaBody;
    cocos2d::Node* playerManaEnd;

    cocos2d::Node* bossHPHead;
    cocos2d::Node* bossHPBody;
    cocos2d::Node* bossHPEnd;

    cocos2d::Node* bossManaHead;
    cocos2d::Node* bossManaBody;
    cocos2d::Node* bossManaEnd;

private:
    KeyboardEventHandler _keyboardHandler;

    cocos2d::EventListenerCustom* _customListener;

    cocos2d::EventListenerCustom* _characterHurtListener;

    cocos2d::EventListenerCustom* _characterDataListener;

    void keyboardActionEventHandler(cocos2d::EventCustom* event);

    void onGameCharacterHurt(cocos2d::EventCustom* event);

    void onGameCharacterDataUpdate(cocos2d::EventCustom* event);

    void bindingHudNode(cocos2d::Node* rootNode);

    void setPlayerHP(float v);
    void setPlayerMana(float v);
    void setBossHP(float v);
    void setBossMana(float v);

    virtual void onExit();
};

#endif // BATTLESCENE_H