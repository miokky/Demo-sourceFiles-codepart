#include "FrameAnimationConfig.h"

FrameAnimationConfig::FrameAnimationConfig() {}
FrameAnimationConfig::~FrameAnimationConfig() {}

FrameAnimationConfig& FrameAnimationConfig::getInstance()
{
    static FrameAnimationConfig instance;
    return instance;
}

bool FrameAnimationConfig::loadConfig(const std::string& filePath)
{
    std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(filePath + ".txt");
    std::istringstream iss(content);
    std::string line;
    while (std::getline(iss, line))
    {
        std::istringstream lineStream(line);
        FrameAnimationInfo info;
        lineStream >> info.actionName >> info.framePrefix >> info.frameCount >> info.frameDelay;
        _animationMap[filePath + info.actionName] = info;
    }
    return true;
}

std::vector<FrameAnimationInfo> FrameAnimationConfig::getFrameAnimationInfos(const std::string& filePath) const
{
    std::vector<FrameAnimationInfo> infos;
    for (const auto& pair : _animationMap)
    {
        if (pair.first.find(filePath) != std::string::npos)
        {
            infos.push_back(pair.second);
        }
    }
    return infos;
}