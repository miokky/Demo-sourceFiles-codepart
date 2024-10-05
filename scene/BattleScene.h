#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"
#include "../core/SequenceFrameAnimation.h"
#include "../core/KeyboardEventHandler.h"

class BattleScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(BattleScene);

private:
    std::unique_ptr<SequenceFrameAnimation> _sequenceAnimation;
    KeyboardEventHandler _keyboardHandler;

    virtual void onExit();
};

#endif // BATTLESCENE_H