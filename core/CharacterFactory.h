#ifndef CHARACTER_FACTORY_H
#define CHARACTER_FACTORY_H

#include "cocos2d.h"
#include "GameCharacter.h"
#include "SequenceFrameAnimation.h"
#include <unordered_map>
#include <string>
#include "FrameAnimationConfig.h"

class CharacterFactory
{
public:
    static CharacterFactory* getInstance();

    GameCharacter* createCharacter(const std::string& characterName, cocos2d::Node* parent);

private:
    CharacterFactory();
    ~CharacterFactory();

    // 加载配置文件并初始化内部数据结构
    void loadConfigurations();

    std::unordered_map<std::string, std::string> _configPaths;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> _cachedConfigs;
    std::unordered_map<std::string, GameCharacter*> _characterCache;

    static CharacterFactory* _instance;
};

#endif // CHARACTER_FACTORY_H