#ifndef IATTACKSTATEMACHINE_H
#define IATTACKSTATEMACHINE_H

#include <string>
#include <functional>
#include "CharacterStateMachine.h"

class IAttackStateMachine
{
public:
    virtual ~IAttackStateMachine() {}

    // ����״̬�ı���Ϣ��������һ����������
    virtual std::string processStateChange(const StateChangeInfo& stateChangeInfo) = 0;

    // ����״̬��
    virtual cocos2d::Vec2 update(float deltaTime) = 0;

    // ע�ṥ�������Ļص�����
    virtual void registerOnAttackEnd(std::function<void()> callback) = 0;

    //ע�ṥ���ж�
    virtual void registerOnDamageCheck(std::function<void(const float& attackRange)> callback) = 0;
};

#endif // IATTACKSTATEMACHINE_H