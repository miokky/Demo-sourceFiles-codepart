#ifndef SEQUENCE_FRAME_ANIMATION_H
#define SEQUENCE_FRAME_ANIMATION_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "FrameAnimationConfig.h"
#include <unordered_map>
#include <memory>

class SequenceFrameAnimation
{
private:
    std::unordered_map<std::string, cocos2d::Vector<cocos2d::SpriteFrame*>> _framesMap;
    std::unordered_map<std::string, float> _frameDelayMap;
    cocos2d::Animation* _animation = nullptr;
    cocos2d::Action* _animate = nullptr;
    cocos2d::Sprite* _target = nullptr;

public:
    SequenceFrameAnimation(cocos2d::Sprite* target = nullptr);
    ~SequenceFrameAnimation();
    static std::unique_ptr<SequenceFrameAnimation> create(const std::string& atlasName, const std::vector<FrameAnimationInfo>& animations, cocos2d::Sprite* target);
    bool initWithAtlasAndAnimations(const std::string& atlasName, const std::vector<FrameAnimationInfo>& animations);
    void play(const std::string& actionName, bool isLoop = false);
    void setFacingDirection(int direction);
    void setPosition(cocos2d::Vec2 pos);
    void setZorder(int value);
};

#endif // SEQUENCE_FRAME_ANIMATION_H