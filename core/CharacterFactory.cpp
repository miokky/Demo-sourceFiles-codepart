#include "CharacterFactory.h"

CharacterFactory* CharacterFactory::_instance = nullptr;

CharacterFactory::CharacterFactory()
{
}

CharacterFactory::~CharacterFactory()
{
    for (const auto& pair : _characterCache)
    {
        delete pair.second;
    }
}

CharacterFactory* CharacterFactory::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new CharacterFactory();
    }
    return _instance;
}

void CharacterFactory::loadConfigurations()
{
    _configPaths["player"] = "PlayerConfig.txt";
    // 可以添加更多角色的配置路径

    for (const auto& pair : _configPaths)
    {
        std::string configPath = pair.second;
        std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(configPath);
        std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath);

        std::unordered_map<std::string, std::string> configMap;
        std::istringstream iss(content);
        std::string line;
        while (std::getline(iss, line))
        {
            std::istringstream lineStream(line);
            std::string key, value;
            lineStream >> key >> value;
            configMap[key] = value;
        }

        _cachedConfigs[pair.first] = configMap;
    }
}

GameCharacter* CharacterFactory::createCharacter(const std::string& characterName, cocos2d::Node* parent)
{
    if (_characterCache.find(characterName) != _characterCache.end())
    {
        return _characterCache[characterName];
    }

    if (_cachedConfigs.find(characterName) == _cachedConfigs.end())
    {
        return nullptr;
    }

    std::unordered_map<std::string, std::string> configMap = _cachedConfigs[characterName];

    // 读取配置文件中的参数
    float anchorX = std::stoi(configMap["anchor_x"]);
    float anchorY = std::stoi(configMap["anchor_y"]);
    int mapPositionX = std::stoi(configMap["initial_x"]);
    int mapPositionY = std::stoi(configMap["initial_y"]);
    std::string currentActionStr = configMap["initial_action"];
    CharacterAction currentAction = currentActionStr == "Stand" ? CharacterAction::Stand : CharacterAction::Moving;
    int health = std::stoi(configMap["health"]);
    int maxHealth = std::stoi(configMap["max_health"]);
    int mana = std::stoi(configMap["mana"]);
    int maxMana = std::stoi(configMap["max_mana"]);
    float manaRecoveryRate = std::stof(configMap["mana_recovery_rate"]);
    int attackPower = std::stoi(configMap["attack_power"]);
    
    // 创建 SequenceFrameAnimation 对象
    std::string atlasName = configMap["initial_action"];
    std::vector<FrameAnimationInfo> animations = FrameAnimationConfig::getInstance().getFrameAnimationInfos(atlasName);
    auto sequenceAnimation = SequenceFrameAnimation::create(atlasName, animations);

    // 创建并初始化 GameCharacter 对象
    CharacterInitData initData{ mapPositionX, mapPositionY, currentAction, health, maxHealth, mana, maxMana, manaRecoveryRate, attackPower };
    auto character = new GameCharacter(sequenceAnimation.get(), initData);

    // 创建 sprite 对象并设置锚点
    auto sprite = cocos2d::Sprite::create();
    sprite->setAnchorPoint(cocos2d::Vec2(anchorX, anchorY));
    sprite->setPosition(mapPositionX, mapPositionY);

    // 根据 initial_action 参数，调用 SequenceFrameAnimation 的 play 方法
    sequenceAnimation->play(sprite, currentActionStr, false);

    // 将 sprite 添加到父节点
    if (parent)
    {
        parent->addChild(sprite);
    }

    // 缓存角色对象
    _characterCache[characterName] = character;

    return character;
}