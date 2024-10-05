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
        return -1;
    }
 

    auto CharacterNode = static_cast<Sprite*>(rootNode->getChildByName("CharacterNode"));
    if (CharacterNode) {

    }
    

    // ע������¼�������
    _keyboardHandler.registerKeyboardEvents(this);


    return true;
}

void BattleScene::onExit()
{
    // ע�������¼����������ͷ� KeyboardEventHandler ����
    _keyboardHandler.~KeyboardEventHandler();
    Layer::onExit();
}