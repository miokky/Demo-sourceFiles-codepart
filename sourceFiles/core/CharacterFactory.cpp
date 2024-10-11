#include "CharacterFactory.h"
#include "PlayerAttackStateMachine.h"

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
    _configPaths["player"] = "playerConfig.txt";
    _configPaths["boss"] = "bossConfig.txt";
    
    // 可以添加更多角色的配置路径

    for (const auto& pair : _configPaths)
    {
        std::string configPath = pair.second;
        std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename("configs/"+configPath);
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
    float anchorX = std::stof(configMap["anchor_x"]);
    float anchorY = std::stof(configMap["anchor_y"]);
    int mapPositionX = std::stoi(configMap["initial_x"]);
    int mapPositionY = std::stoi(configMap["initial_y"]);
    std::string currentActionStr = configMap["initial_action"];
    CharacterAction currentAction = currentActionStr == "stand" ? CharacterAction::Stand : CharacterAction::Moving;
    int health = std::stoi(configMap["health"]);
    int maxHealth = std::stoi(configMap["max_health"]);
    int mana = std::stoi(configMap["mana"]);
    int maxMana = std::stoi(configMap["max_mana"]);
    float manaRecoveryRate = std::stof(configMap["mana_recovery_rate"]);
    int attackPower = std::stoi(configMap["attack_power"]);
    int moveSpeed = std::stoi(configMap["move_speed"]);
    int initialScaleX = std::stoi(configMap["initial_scaleX"]);
    
    
    // 创建 SequenceFrameAnimation 对象
    std::string atlasName = configMap["plist_name"];
    std::vector<FrameAnimationInfo> animations = FrameAnimationConfig::getInstance().getFrameAnimationInfos(atlasName);

    // 创建 sprite 对象并设置锚点
    auto sprite = cocos2d::Sprite::create();
    sprite->setAnchorPoint(cocos2d::Vec2(anchorX, anchorY));
    // 将 sprite 添加到父节点
    if (parent)
    {
        parent->addChild(sprite);

        //todo
        if (characterName == "boss")
        {
            sprite->setColor(cocos2d::Color3B(150, 150, 150));
        }
    }
    sprite->setPosition(mapPositionX, mapPositionY);

    auto sequenceAnimation = SequenceFrameAnimation::create(atlasName, animations, sprite);
    
    // 创建攻击状态机的实例 todo 根据通角色读取配置生成不同实例
    std::shared_ptr<IAttackStateMachine> attackStateMachine = std::make_shared<PlayerAttackStateMachine>();
    // 创建并初始化 GameCharacter 对象
    CharacterInitData initData{ characterName, mapPositionX, mapPositionY, currentAction, health, maxHealth, mana, maxMana, manaRecoveryRate, attackPower, moveSpeed, initialScaleX, attackStateMachine };
    auto character = new GameCharacter(std::move(sequenceAnimation), initData);

    
    // 根据 initial_action 参数，调用 SequenceFrameAnimation 的 play 方法    
    character->getDisplayObject()->play(currentActionStr, true);

    // 缓存角色对象
    _characterCache[characterName] = character;

    return character;
}

GameCharacter* CharacterFactory::getCharacter(const std::string& characterName)
{
    if (_characterCache.find(characterName) != _characterCache.end())
    {
        return _characterCache[characterName];
    }
    return nullptr;
}