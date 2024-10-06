#include "BattleScene.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "../core/FrameAnimationConfig.h"
#include "../core/SequenceFrameAnimation.h";

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

    // 注册键盘事件处理类
    _keyboardHandler.registerKeyboardEvents(this);
 
    // 初始化 CharacterFactory
    CharacterFactory::getInstance()->loadConfigurations();

    auto CharacterNode = static_cast<Node*>(rootNode->getChildByName("map1_1")->getChildByName("CharacterNode"));
    if (CharacterNode) {
        // 创建角色
        CharacterFactory::getInstance()->createCharacter("player", CharacterNode);
    }

    // 注册 keyboard_action_event 事件监听器
    auto listener = cocos2d::EventListenerCustom::create("keyboard_action_event", [this](EventCustom* event) {
        ActionNameData* data = static_cast<ActionNameData*>(event->getUserData());
        auto character = CharacterFactory::getInstance()->getCharacter("player");
        if (!character)
        {
            return;
        }
        if (data) {
            switch (data->actionName) {
                // 根据不同的动作名称进行处理
            case ActionNameEnum::ACTION_MOVE_UP:
                // 处理向上移动事件
                break;
            case ActionNameEnum::ACTION_MOVE_DOWN:
                // 处理向下移动事件
            case ActionNameEnum::ACTION_ATTACK:
                character->getDisplayObject()->play("atk1");
                break;
                // 其他动作名称的处理
            default:
                break;
            }
        }
        });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}



void BattleScene::onExit()
{
    // 注销键盘事件监听器并释放 KeyboardEventHandler 对象
    _keyboardHandler.~KeyboardEventHandler();
    Layer::onExit();
}



