#ifndef IATTACKSTATEMACHINE_H
#define IATTACKSTATEMACHINE_H

#include <string>
#include <functional>
#include "CharacterStateMachine.h"

class IAttackStateMachine
{
public:
    virtual ~IAttackStateMachine() {}

    // 传入状态改变信息，返回下一个动作名字
    virtual std::string processStateChange(const StateChangeInfo& stateChangeInfo) = 0;

    // 更新状态机
    virtual cocos2d::Vec2 update(float deltaTime) = 0;

    // 注册攻击结束的回调函数
    virtual void registerOnAttackEnd(std::function<void()> callback) = 0;

    //注册攻击判定
    virtual void registerOnDamageCheck(std::function<void(const float& attackRange)> callback) = 0;
};

#endif // IATTACKSTATEMACHINE_H