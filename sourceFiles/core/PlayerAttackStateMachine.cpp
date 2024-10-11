#include "PlayerAttackStateMachine.h"

PlayerAttackStateMachine::PlayerAttackStateMachine()
{
    attackSequence["atk1"] = "atk2";
    attackSequence["atk2"] = "atk3";
    attackSequence["atk3"] = "atk4";
    attackSequence["atk4"] = "atk5";
    attackSequence["atk5"] = "atk2";

    attackParams["atk1"] = { 1.0f, 100.0f, 1.2f, std::vector<float>(),100.0f };
    attackParams["atk2"] = { 0.7f, 50.0f, 1.0f, std::vector<float>(),75.0f };
    attackParams["atk3"] = { 1.0f, 30.0f, 1.2f, std::vector<float>(),100.0f };
    attackParams["atk4"] = { 1.3f, 70.0f, 0.8f, std::vector<float>(),125.0f };
    attackParams["atk5"] = { 1.5f, 30.0f, -1.0f, std::vector<float>(),125.0f };

    attackParams["atk1"].damageTime.push_back(0.3f); attackParams["atk1"].damageTime.push_back(0.6f);
    attackParams["atk2"].damageTime.push_back(0.4f);
    attackParams["atk3"].damageTime.push_back(0.5f);
    attackParams["atk4"].damageTime.push_back(0.5f);
    attackParams["atk5"].damageTime.push_back(0.8f); attackParams["atk5"].damageTime.push_back(1.2f);
}

PlayerAttackStateMachine::~PlayerAttackStateMachine() {
    onDamageCheckCallback = nullptr;
    onAttackEndCallback = nullptr;
}

std::string PlayerAttackStateMachine::processStateChange(const StateChangeInfo& stateChangeInfo)
{
    if (stateChangeInfo.lastState == CharacterState::Moving)
    {
        currentAttack = "atk1";
    }
    else
    {
        if (currentAttack.empty())
        {
            currentAttack = "atk2";
        }
        else
        {
            currentAttack = attackSequence[currentAttack];
        }
    }
    currentAttackDuration = 0;
    isAttacking = true;
    hurtCount = 0;
    return currentAttack;
}

cocos2d::Vec2 PlayerAttackStateMachine::update(float deltaTime)
{
    if (!isAttacking && resetTimeCount < resetTimeThreshold)
    {
        resetTimeCount += deltaTime;
        if (resetTimeCount >= resetTimeThreshold)
        {
            currentAttack = "";
        }
    }

    currentAttackDuration += deltaTime;
    cocos2d::Vec2 result = cocos2d::Vec2(0, 0);
    if (isAttacking)
    {
        float lastValue = customClampedValue(currentAttackDuration - deltaTime, 0, attackParams[currentAttack].duration, attackParams[currentAttack].curveFactor);
        float currentValue = customClampedValue(currentAttackDuration, 0, attackParams[currentAttack].duration, attackParams[currentAttack].curveFactor);
        float offsetX = (currentValue - lastValue) * attackParams[currentAttack].offsetx;
        result = cocos2d::Vec2(offsetX, 0);
    }

    int hurttime = attackParams[currentAttack].damageTime.size();
    if (isAttacking)
    {
        cocos2d::log("checkHurtPoint ----------------: %d", (currentAttackDuration));
        cocos2d::log("checkHurtPoint hc----------------: %d", (hurtCount));
        cocos2d::log("checkHurtPoint ht----------------: %d", (hurttime));
        if (hurtCount < hurttime)
        {
            if (currentAttackDuration >= attackParams[currentAttack].damageTime[hurtCount])
            {
                onDamageCheckCallback(attackParams[currentAttack].attackRange);
                hurtCount++;
            }
        }
    }

    if (isAttacking && currentAttackDuration >= attackParams[currentAttack].duration)
    {
        isAttacking = false;
        onAttackEndCallback();
        resetTimeCount = 0;
    }

    return result;
}

void PlayerAttackStateMachine::registerOnAttackEnd(std::function<void()> callback)
{
    onAttackEndCallback = callback;
}

void PlayerAttackStateMachine::registerOnDamageCheck(std::function<void(const float& attackRange)> callback)
{
    onDamageCheckCallback = callback;
}

float PlayerAttackStateMachine::customClampedValue(float value, float minValue, float maxValue, float curveFactor) {
    if (value <= minValue) return minValue;
    if (value >= maxValue) return maxValue;

    float t = (value - minValue) / (maxValue - minValue);
    return minValue + (maxValue - minValue) * (1 - std::pow(1 - t, 2)) * curveFactor;
}
