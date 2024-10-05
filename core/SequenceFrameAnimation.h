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
    std::unique_ptr<cocos2d::Animation> _animation;
    std::unique_ptr<cocos2d::Animate> _animate;

public:
    SequenceFrameAnimation();
    ~SequenceFrameAnimation();
    static std::unique_ptr<SequenceFrameAnimation> create(const std::string& atlasName, const std::vector<FrameAnimationInfo>& animations);
    bool initWithAtlasAndAnimations(const std::string& atlasName, const std::vector<FrameAnimationInfo>& animations);
    void play(cocos2d::Node* target, const std::string& actionName, bool isLoop = false);
};

#endif // SEQUENCE_FRAME_ANIMATION_H