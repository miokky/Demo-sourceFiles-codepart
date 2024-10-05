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
    // ������Ӹ����ɫ������·��

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

    // ��ȡ�����ļ��еĲ���
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
    
    // ���� SequenceFrameAnimation ����
    std::string atlasName = configMap["initial_action"];
    std::vector<FrameAnimationInfo> animations = FrameAnimationConfig::getInstance().getFrameAnimationInfos(atlasName);
    auto sequenceAnimation = SequenceFrameAnimation::create(atlasName, animations);

    // ��������ʼ�� GameCharacter ����
    CharacterInitData initData{ mapPositionX, mapPositionY, currentAction, health, maxHealth, mana, maxMana, manaRecoveryRate, attackPower };
    auto character = new GameCharacter(sequenceAnimation.get(), initData);

    // ���� sprite ��������ê��
    auto sprite = cocos2d::Sprite::create();
    sprite->setAnchorPoint(cocos2d::Vec2(anchorX, anchorY));
    sprite->setPosition(mapPositionX, mapPositionY);

    // ���� initial_action ���������� SequenceFrameAnimation �� play ����
    sequenceAnimation->play(sprite, currentActionStr, false);

    // �� sprite ��ӵ����ڵ�
    if (parent)
    {
        parent->addChild(sprite);
    }

    // �����ɫ����
    _characterCache[characterName] = character;

    return character;
}