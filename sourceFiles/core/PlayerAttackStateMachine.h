#ifndef PLAYERATTACKSTATEMACHINE_H
#define PLAYERATTACKSTATEMACHINE_H

#include <string>
#include <map>
#include "CharacterStateMachine.h"
#include "IAttackStateMachine.h"
#include <vector>

// 攻击动作结构体
struct AttackInfo
{
    float duration;
    float offsetx = 0.0f;
    float curveFactor = 1.0f;
    std::vector<float> damageTime;
    float damageRate = 1.0f;
    float attackRange = 50.0f;
};

class PlayerAttackStateMachine : public IAttackStateMachine
{
public:
    PlayerAttackStateMachine();
    ~PlayerAttackStateMachine();

    // 实现接口中的方法
    std::string processStateChange(const StateChangeInfo& stateChangeInfo);
    cocos2d::Vec2 update(float deltaTime);
    void registerOnAttackEnd(std::function<void()> callback);
    void registerOnDamageCheck(std::function<void(const float& attackRange)> callback);

private:
    //config
    std::map<std::string, std::string> attackSequence;
    std::map<std::string, AttackInfo> attackParams;

    //
    std::function<void()> onAttackEndCallback;
    std::function<void(const float& attackRange)> onDamageCheckCallback;
    float currentAttackDuration = 0.0f;
    std::string currentAttack = "";
    int hurtCount = 0;

    static constexpr float resetTimeThreshold = 0.1f;
    float resetTimeCount = 0.0f;
    bool isAttacking = false;

    float customClampedValue(float value, float minValue, float maxValue, float curveFactor);
};

#endif // PLAYERATTACKSTATEMACHINE_H