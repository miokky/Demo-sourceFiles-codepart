#include "BattleScene.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "../core/FrameAnimationConfig.h"
#include "../core/SequenceFrameAnimation.h"
#include "../core/BattleDataModel.h"

USING_NS_CC;

Scene* BattleScene::createScene() {
    auto scene = Scene::create();
    auto layer = BattleScene::create();
    scene->addChild(layer);
    return scene;
}

bool BattleScene::init() {
    if (!Layer::init()) {
        return false;
    }

    // 加载.csb 文件
    auto rootNode = CSLoader::createNode("BattleScene.csb");
    addChild(rootNode);

    FrameAnimationConfig& config = FrameAnimationConfig::getInstance();
    bool success = config.loadConfig("reimu_animation_frames");
    if (!success)
    {
        return false;
    }

    // 实例化 BattleDataModel
    BattleDataModel::getInstance().init();

    // 注册键盘事件处理类
    _keyboardHandler = KeyboardEventHandler();
    _keyboardHandler.registerKeyboardEvents(this);
 
    // 初始化 CharacterFactory
    CharacterFactory::getInstance()->loadConfigurations();

    auto CharacterNode = static_cast<Node*>(rootNode->getChildByName("map1_1")->getChildByName("CharacterNode"));
    if (CharacterNode) {
        // 创建角色
        CharacterFactory::getInstance()->createCharacter("player", CharacterNode);
        CharacterFactory::getInstance()->createCharacter("boss", CharacterNode);
    }

    //// 注册 keyboard_action_event 事件监听器
    _customListener = Director::getInstance()->getEventDispatcher()->addCustomEventListener("keyboard_action_event", CC_CALLBACK_1(BattleScene::keyboardActionEventHandler, this));

    _characterHurtListener = Director::getInstance()->getEventDispatcher()->addCustomEventListener("game_character_hurt", CC_CALLBACK_1(BattleScene::onGameCharacterHurt, this));

    _characterDataListener = Director::getInstance()->getEventDispatcher()->addCustomEventListener("game_character_data_change", CC_CALLBACK_1(BattleScene::onGameCharacterDataUpdate, this));
    bindingHudNode(rootNode);

    //start update
    this->scheduleUpdate();

    return true;
}

void BattleScene::update(float deltaTime) {
    // 更新场景中的其他元素

    // 更新角色
    auto character = CharacterFactory::getInstance()->getCharacter("player");
    auto boss = CharacterFactory::getInstance()->getCharacter("boss");
    if (character) {
        character->update(deltaTime);
    }
    if (boss)
    {
        boss->update(deltaTime);
    }
}

void BattleScene::keyboardActionEventHandler(EventCustom* event) {
    ActionNameData* data = static_cast<ActionNameData*>(event->getUserData());
    auto character = CharacterFactory::getInstance()->getCharacter("player");
    if (!character) {
        return;
    }
    character->processActionCommand(data->actionName);
}

void BattleScene::onGameCharacterHurt(EventCustom* event) {
    HurtData* data = static_cast<HurtData*>(event->getUserData());
    auto character = CharacterFactory::getInstance()->getCharacter(data->name);
    if (!character) {
        return;
    }
    character->hurtByOther(data->dir,data->damage);
}

void BattleScene::onGameCharacterDataUpdate(EventCustom* event) {
    const std::map<BattleDataTokens, int>& data = BattleDataModel::getInstance().getDataMap();

    float playerHPPercentage = static_cast<float>(data.at(BattleDataTokens::PlayerHP)) / data.at(BattleDataTokens::PlayerHPMAX);
    setPlayerHP(playerHPPercentage);

    float playerManaPercentage = static_cast<float>(data.at(BattleDataTokens::PlayerMP)) / data.at(BattleDataTokens::PlayerMPMAX);
    setPlayerMana(playerManaPercentage);

    float bossHPPercentage = static_cast<float>(data.at(BattleDataTokens::BossHP)) / data.at(BattleDataTokens::BossHPMAX);
    setBossHP(bossHPPercentage);

    float bossManaPercentage = static_cast<float>(data.at(BattleDataTokens::BossMP)) / data.at(BattleDataTokens::BossMPMAX);
    setBossMana(bossManaPercentage);
}

void BattleScene::bindingHudNode(cocos2d::Node* rootNode) {
    playerHPHead = static_cast<Node*>(rootNode->getChildByName("hud_node")->getChildByName("player_info")->getChildByName("hp")->getChildByName("bar_start"));
    playerHPBody = static_cast<Node*>(rootNode->getChildByName("hud_node")->getChildByName("player_info")->getChildByName("hp")->getChildByName("bar_body"));
    playerHPEnd = static_cast<Node*>(rootNode->getChildByName("hud_node")->getChildByName("player_info")->getChildByName("hp")->getChildByName("bar_end"));

    playerManaHead = static_cast<Node*>(rootNode->getChildByName("hud_node")->getChildByName("player_info")->getChildByName("mp")->getChildByName("bar_start"));
    playerManaBody = static_cast<Node*>(rootNode->getChildByName("hud_node")->getChildByName("player_info")->getChildByName("mp")->getChildByName("bar_body"));
    playerManaEnd = static_cast<Node*>(rootNode->getChildByName("hud_node")->getChildByName("player_info")->getChildByName("mp")->getChildByName("bar_end"));

    bossHPHead = static_cast<Node*>(rootNode->getChildByName("hud_node")->getChildByName("boss_info")->getChildByName("hp")->getChildByName("bar_start"));
    bossHPBody = static_cast<Node*>(rootNode->getChildByName("hud_node")->getChildByName("boss_info")->getChildByName("hp")->getChildByName("bar_body"));
    bossHPEnd = static_cast<Node*>(rootNode->getChildByName("hud_node")->getChildByName("boss_info")->getChildByName("hp")->getChildByName("bar_end"));

    bossManaHead = static_cast<Node*>(rootNode->getChildByName("hud_node")->getChildByName("boss_info")->getChildByName("mp")->getChildByName("bar_start"));
    bossManaBody = static_cast<Node*>(rootNode->getChildByName("hud_node")->getChildByName("boss_info")->getChildByName("mp")->getChildByName("bar_body"));
    bossManaEnd = static_cast<Node*>(rootNode->getChildByName("hud_node")->getChildByName("boss_info")->getChildByName("mp")->getChildByName("bar_end"));
}

void BattleScene::setPlayerHP(float v)
{
    float percent = v;
    playerHPBody->setScaleX(percent);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    float bodyWidth = playerHPBody->getContentSize().width * percent;
    playerHPEnd->setPositionX(playerHPHead->getPositionX() + bodyWidth);
}

void BattleScene::setPlayerMana(float v)
{
    float percent = v;
    playerManaBody->setScaleX(percent);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    float bodyWidth = playerManaBody->getContentSize().width * percent;
    playerManaEnd->setPositionX(playerManaHead->getPositionX() + bodyWidth);
}

void BattleScene::setBossHP(float v)
{
    float percent = v;
    bossHPBody->setScaleX(percent);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    float bodyWidth = bossHPBody->getContentSize().width * percent;
    bossHPEnd->setPositionX(bossHPHead->getPositionX() + bodyWidth);
}

void BattleScene::setBossMana(float v)
{
    float percent = v;
    bossManaBody->setScaleX(percent);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    float bodyWidth = bossManaBody->getContentSize().width * percent;
    bossManaEnd->setPositionX(bossManaHead->getPositionX() + bodyWidth);
}

void BattleScene::onExit()
{
    BattleDataModel::getInstance().clear();

    Director::getInstance()->getEventDispatcher()->removeEventListener(_customListener);
    
    Director::getInstance()->getEventDispatcher()->removeEventListener(_characterHurtListener);

    Director::getInstance()->getEventDispatcher()->removeEventListener(_characterDataListener);

    Layer::onExit();
}
