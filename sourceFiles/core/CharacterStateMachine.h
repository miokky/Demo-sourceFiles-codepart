#ifndef CHARACTERSTATEMACHINE_H
#define CHARACTERSTATEMACHINE_H

#include <string>
#include <functional>
#include "cocos2d.h"

enum class CharacterState
{
    Stand,
    Moving,
    Attacking,
    Stiffness
};

// ������Ϣ�ṹ��
struct CommandInfo
{
    int horizontalDirection = 0;
    int verticalDirection = 0;
    int hurtMoveDirection = 0;
};

// ״̬�ı���Ϣ�ṹ��
struct StateChangeInfo
{
    CharacterState lastState;
    CharacterState currentState;
    CommandInfo stateInfo;
};

class CharacterStateMachine
{
public:
    CharacterStateMachine();
    ~CharacterStateMachine();

    // ��ȡ��ǰ״̬
    CharacterState getCurrentState() const;
    cocos2d::Vec2 getMoveDirection();

    // ����������
    void processActionCommand(const std::string& actionCommand, const CommandInfo& commandInfo);

    // ע��״̬�ı��¼�
    void registerOnStateChange(std::function<void(const StateChangeInfo&)> callback);

    // ��ע��״̬�ı��¼�
    void unregisterOnStateChange();

    cocos2d::Vec2 update(float delta);

private:
    CharacterState _currentState = CharacterState::Stand;
    CharacterState _nextState = CharacterState::Stand;
    std::function<void(const StateChangeInfo&)> _onStateChangeCallback;
    int _horizontalDirection = 0;
    int _verticalDirection = 0;
    int _hurtMoveDirection = 0;

    static constexpr float _stiffnessMax = 0.3f;
    float _stiffnessTime = 0.0f;

    float customClampedValue(float value, float minValue, float maxValue, float curveFactor);
};

#endif // CHARACTERSTATEMACHINE_H