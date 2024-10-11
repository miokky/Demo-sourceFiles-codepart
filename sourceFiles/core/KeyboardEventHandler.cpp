#include "KeyboardEventHandler.h"
#include "cocos2d.h"

using namespace cocos2d;

KeyboardEventHandler::KeyboardEventHandler()
{
    _keyboardListener = nullptr;
}

KeyboardEventHandler::~KeyboardEventHandler()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(_keyboardListener);
}

void KeyboardEventHandler::registerKeyboardEvents(cocos2d::Node* target)
{
    // 从配置文件加载键值映射
    std::string configPath = FileUtils::getInstance()->fullPathForFilename("configs/keypressConfig.txt");
    std::string content = FileUtils::getInstance()->getStringFromFile(configPath);

    std::vector<std::string> lines;
    std::istringstream iss(content);
    std::string line;
    while (std::getline(iss, line))
    {
        lines.push_back(line);
    }

    for (const std::string& configLine : lines)
    {
        std::istringstream lineStream(configLine);
        std::string keyChar;
        std::string actionNameStr;
        lineStream >> keyChar >> actionNameStr;

        cocos2d::EventKeyboard::KeyCode keyCode;
        ActionNameEnum actionName;

        if (keyChar == "W")
            keyCode = EventKeyboard::KeyCode::KEY_W;
        else if (keyChar == "S")
            keyCode = EventKeyboard::KeyCode::KEY_S;
        else if (keyChar == "A")
            keyCode = EventKeyboard::KeyCode::KEY_A;
        else if (keyChar == "D")
            keyCode = EventKeyboard::KeyCode::KEY_D;
        else if (keyChar == "J")
            keyCode = EventKeyboard::KeyCode::KEY_J;
        else if (keyChar == "K")
            keyCode = EventKeyboard::KeyCode::KEY_K;
        else
            continue;

        if (actionNameStr == "move_up")
            actionName = ActionNameEnum::ACTION_MOVE_UP;
        else if (actionNameStr == "move_down")
            actionName = ActionNameEnum::ACTION_MOVE_DOWN;
        else if (actionNameStr == "move_left")
            actionName = ActionNameEnum::ACTION_MOVE_LEFT;
        else if (actionNameStr == "move_right")
            actionName = ActionNameEnum::ACTION_MOVE_RIGHT;
        else if (actionNameStr == "attack")
            actionName = ActionNameEnum::ACTION_ATTACK;
        else if (actionNameStr == "cast_skill_1")
            actionName = ActionNameEnum::ACTION_CAST_SKILL_1;
        else
            continue;

        _keyActionMap[keyCode] = actionName;
    }

    _keyboardListener = cocos2d::EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
        {
            auto it = _keyActionMap.find(keyCode);
            if (it != _keyActionMap.end())
            {
                dispatchKeyboardEvent(it->second);
            }
            return true;
        };
    _keyboardListener->onKeyReleased = [this](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
        {
            auto it = _keyActionMap.find(keyCode);
            if (it != _keyActionMap.end())
            {
                ActionNameEnum actionName = it->second;
                switch (actionName)
                {
                case ActionNameEnum::ACTION_MOVE_UP:
                    dispatchKeyboardEvent(ActionNameEnum::ACTION_MOVE_UP_CANCEL);
                    break;
                case ActionNameEnum::ACTION_MOVE_DOWN:
                    dispatchKeyboardEvent(ActionNameEnum::ACTION_MOVE_DOWN_CANCEL);
                    break;
                case ActionNameEnum::ACTION_MOVE_LEFT:
                    dispatchKeyboardEvent(ActionNameEnum::ACTION_MOVE_LEFT_CANCEL);
                    break;
                case ActionNameEnum::ACTION_MOVE_RIGHT:
                    dispatchKeyboardEvent(ActionNameEnum::ACTION_MOVE_RIGHT_CANCEL);
                    break;
                default:
                    break;
                }
            }
            return true;
        };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyboardListener, target);
}

void KeyboardEventHandler::dispatchKeyboardEvent(ActionNameEnum actionName)
{
    // 根据动作名称进行事件派发或其他处理逻辑
    cocos2d::EventCustom customEvent("keyboard_action_event");
    ActionNameData* actionNameData = new ActionNameData(actionName);
    customEvent.setUserData(actionNameData);
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&customEvent);
    delete actionNameData;
}