#ifndef FRAME_ANIMATION_CONFIG_H
#define FRAME_ANIMATION_CONFIG_H

#include "cocos2d.h"
#include <string>
#include <unordered_map>
#include <vector>

struct FrameAnimationInfo
{
    std::string actionName;
    std::string framePrefix;
    int frameCount;
    float frameDelay;
};

class FrameAnimationConfig
{
private:
    FrameAnimationConfig();
    ~FrameAnimationConfig();

    std::unordered_map<std::string, FrameAnimationInfo> _animationMap;

public:
    static FrameAnimationConfig& getInstance();

    bool loadConfig(const std::string& filePath);
    std::vector<FrameAnimationInfo> getFrameAnimationInfos(const std::string& filePath) const;
};

#endif // FRAME_ANIMATION_CONFIG_H