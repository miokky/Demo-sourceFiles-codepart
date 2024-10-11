#include "SequenceFrameAnimation.h"

SequenceFrameAnimation::SequenceFrameAnimation(cocos2d::Sprite* target) : _target(target) {}
SequenceFrameAnimation::~SequenceFrameAnimation() {}

std::unique_ptr<SequenceFrameAnimation> SequenceFrameAnimation::create(const std::string& atlasName, const std::vector<FrameAnimationInfo>& animations, cocos2d::Sprite* target)
{
    std::unique_ptr<SequenceFrameAnimation> animation = std::make_unique<SequenceFrameAnimation>(target);
    if (animation && animation->initWithAtlasAndAnimations(atlasName, animations))
    {
        return animation;
    }
    return nullptr;
}

bool SequenceFrameAnimation::initWithAtlasAndAnimations(const std::string& atlasName, const std::vector<FrameAnimationInfo>& animations)
{
    cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(atlasName + ".plist");

    for (const auto& info : animations)
    {
        cocos2d::Vector<cocos2d::SpriteFrame*> frames;
        for (int i = 1; i <= info.frameCount; ++i)
        {
            std::string spriteFrameName;
            if (i < 10)
            {
                spriteFrameName = info.framePrefix + "0" + std::to_string(i);
            }
            else
            {
                spriteFrameName = info.framePrefix + std::to_string(i);
            }
            auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName + ".png");
            if (!frame)
            {
                CCLOG("Error loading sprite frame: %s", spriteFrameName.c_str());
                return false;
            }
            frames.pushBack(frame);
        }
        _framesMap[info.actionName] = frames;
        _frameDelayMap[info.actionName] = info.frameDelay;
    }

    return true;
}

void SequenceFrameAnimation::play(const std::string& actionName, bool isLoop)
{
    if (!_target) return;
    if (_animate != nullptr)
    {
        _target->stopAction(_animate);
    }
    _animate = nullptr;

    auto framesIt = _framesMap.find(actionName);
    auto delayIt = _frameDelayMap.find(actionName);
    if (framesIt == _framesMap.end() || delayIt == _frameDelayMap.end())
    {
        CCLOG("Action not found: %s", actionName.c_str());
        return;
    }

    
    
    _animation = cocos2d::Animation::createWithSpriteFrames(framesIt->second, delayIt->second);
    if (isLoop)
    {
        _animate = cocos2d::RepeatForever::create(cocos2d::Animate::create(_animation));
    }
    else
    {
        _animate = cocos2d::Animate::create(_animation);
    }
    _target->runAction(_animate);
}

void SequenceFrameAnimation::setFacingDirection(int direction)
{
    _target->setScaleX(static_cast<float>(direction));
}

void SequenceFrameAnimation::setPosition(cocos2d::Vec2 pos)
{
    _target->setPosition(pos);
}

void SequenceFrameAnimation::setZorder(int value)
{
    _target->setGlobalZOrder(value);
}
