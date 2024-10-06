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

    // ����.csb �ļ�
    auto rootNode = CSLoader::createNode("BattleScene.csb");
    addChild(rootNode);

    FrameAnimationConfig& config = FrameAnimationConfig::getInstance();
    bool success = config.loadConfig("reimu_animation_frames");
    if (!success)
    {
        return false;
    }

    // ע������¼�������
    _keyboardHandler.registerKeyboardEvents(this);
 
    // ��ʼ�� CharacterFactory
    CharacterFactory::getInstance()->loadConfigurations();

    auto CharacterNode = static_cast<Node*>(rootNode->getChildByName("map1_1")->getChildByName("CharacterNode"));
    if (CharacterNode) {
        // ������ɫ
        CharacterFactory::getInstance()->createCharacter("player", CharacterNode);
    }

    // ע�� keyboard_action_event �¼�������
    auto listener = cocos2d::EventListenerCustom::create("keyboard_action_event", [this](EventCustom* event) {
        ActionNameData* data = static_cast<ActionNameData*>(event->getUserData());
        auto character = CharacterFactory::getInstance()->getCharacter("player");
        if (!character)
        {
            return;
        }
        if (data) {
            switch (data->actionName) {
                // ���ݲ�ͬ�Ķ������ƽ��д���
            case ActionNameEnum::ACTION_MOVE_UP:
                // ���������ƶ��¼�
                break;
            case ActionNameEnum::ACTION_MOVE_DOWN:
                // ���������ƶ��¼�
            case ActionNameEnum::ACTION_ATTACK:
                character->getDisplayObject()->play("atk1");
                break;
                // �����������ƵĴ���
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
    // ע�������¼����������ͷ� KeyboardEventHandler ����
    _keyboardHandler.~KeyboardEventHandler();
    Layer::onExit();
}



