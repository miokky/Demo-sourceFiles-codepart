#ifndef KEYBOARD_EVENT_HANDLER_H
#define KEYBOARD_EVENT_HANDLER_H

#include "cocos2d.h"

enum class ActionNameEnum
{
    ACTION_MOVE_UP,
    ACTION_MOVE_DOWN,
    ACTION_MOVE_LEFT,
    ACTION_MOVE_RIGHT,
    ACTION_MOVE_STOP,
    ACTION_ATTACK,
    ACTION_CAST_SKILL_1
    // ���Լ�����Ӹ��ද�����Ƶ�ö��ֵ
};

// ���� ActionNameData �ṹ��
struct ActionNameData
{
    ActionNameEnum actionName;

    ActionNameData(ActionNameEnum name) : actionName(name) {}
};

class KeyboardEventHandler
{
public:
    KeyboardEventHandler();
    ~KeyboardEventHandler();

    void registerKeyboardEvents(cocos2d::Node* target);
    void dispatchKeyboardEvent(ActionNameEnum actionName);

private:
    cocos2d::EventListenerKeyboard* _keyboardListener;
    std::unordered_map<cocos2d::EventKeyboard::KeyCode, ActionNameEnum> _keyActionMap;
};

#endif // KEYBOARD_EVENT_HANDLER_H