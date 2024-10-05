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
        return -1;
    }
 

    auto CharacterNode = static_cast<Sprite*>(rootNode->getChildByName("CharacterNode"));
    if (CharacterNode) {

    }
    

    // 注册键盘事件处理类
    _keyboardHandler.registerKeyboardEvents(this);


    return true;
}

void BattleScene::onExit()
{
    // 注销键盘事件监听器并释放 KeyboardEventHandler 对象
    _keyboardHandler.~KeyboardEventHandler();
    Layer::onExit();
}