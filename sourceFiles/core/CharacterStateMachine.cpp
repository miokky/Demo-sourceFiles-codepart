#include "CharacterStateMachine.h"

CharacterStateMachine::CharacterStateMachine() : _currentState(CharacterState::Stand) {}

CharacterStateMachine::~CharacterStateMachine() {
    _onStateChangeCallback = nullptr;
}

CharacterState CharacterStateMachine::getCurrentState() const {
    return _currentState;
}

cocos2d::Vec2 CharacterStateMachine::getMoveDirection()
{
    return cocos2d::Vec2(_horizontalDirection, _verticalDirection);
}

void CharacterStateMachine::processActionCommand(const std::string& actionCommand, const CommandInfo& commandInfo) {
    CharacterState lastState = _currentState;
    if (actionCommand == "move") {
        _horizontalDirection += commandInfo.horizontalDirection;
        _verticalDirection += commandInfo.verticalDirection;
        switch (_currentState) {
        case CharacterState::Stand:
        case CharacterState::Moving:
            _currentState = _horizontalDirection == 0 && _verticalDirection == 0 ? CharacterState::Stand : CharacterState::Moving;
           break;
        case CharacterState::Attacking:
        case CharacterState::Stiffness:
            break;
        }
        _nextState = _horizontalDirection == 0 && _verticalDirection == 0 ? CharacterState::Stand : CharacterState::Moving;
    }
    else if (actionCommand == "stop_move") {
        _horizontalDirection -= commandInfo.horizontalDirection;
        _verticalDirection -= commandInfo.verticalDirection;
        switch (_currentState) {
        case CharacterState::Moving:
        case CharacterState::Stand:
            _currentState = _horizontalDirection == 0 && _verticalDirection == 0 ? CharacterState::Stand : CharacterState::Moving;
            break;
        case CharacterState::Attacking:
        case CharacterState::Stiffness:
            break;
        }
        _nextState = _horizontalDirection == 0 && _verticalDirection == 0 ? CharacterState::Stand : CharacterState::Moving;
    }
    else if (actionCommand == "attack") {
        switch (_currentState) {
        case CharacterState::Stand:
        case CharacterState::Moving:
            _currentState = CharacterState::Attacking;
            break;
        case CharacterState::Attacking:
        case CharacterState::Stiffness:
            _nextState = CharacterState::Attacking;
            break;
        }
    }
    else if (actionCommand == "stiffness") {
        switch (_currentState) {
        case CharacterState::Stand:
        case CharacterState::Moving:
        case CharacterState::Attacking:
            _currentState = CharacterState::Stiffness;
            _hurtMoveDirection = commandInfo.hurtMoveDirection;
            _stiffnessTime = 0;
            _nextState = CharacterState::Stand;
            break;
        case CharacterState::Stiffness:
            //do nothing
            break;
        }
    }
    else if (actionCommand == "end_stiffness" || actionCommand == "end_attack") {
        _currentState = _nextState;
        _nextState = _horizontalDirection == 0 && _verticalDirection == 0 ? CharacterState::Stand : CharacterState::Moving;
    }

    cocos2d::log("oncommand ----------------: %d", static_cast<int>(_currentState));

    if (_onStateChangeCallback) {
        if ((actionCommand == "move" || actionCommand == "stop_move") && (_currentState != CharacterState::Stand && _currentState != CharacterState::Moving))
            return;
        if (lastState == CharacterState::Attacking && actionCommand == "attack")
            return;
        if (lastState == CharacterState::Stiffness && actionCommand == "stiffness")
            return;
        cocos2d::log("onstateChange ----------------: %d", static_cast<int>(_currentState));
        cocos2d::log("onstateChange next state----------------: %d", static_cast<int>(_nextState));
        StateChangeInfo info;
        info.lastState = lastState;
        info.currentState = _currentState;
        CommandInfo cinfo;
        info.stateInfo = cinfo;
        info.stateInfo.horizontalDirection = _horizontalDirection;
        info.stateInfo.verticalDirection = _verticalDirection;
        info.stateInfo.hurtMoveDirection = _hurtMoveDirection;
        _onStateChangeCallback(info);
    }
}

cocos2d::Vec2 CharacterStateMachine::update(float delta)
{
    if (_currentState != CharacterState::Stiffness)
    {
        return cocos2d::Vec2(0,0);
    }
    _stiffnessTime += delta;
    if (_stiffnessTime >= _stiffnessMax)
    {
        CommandInfo cinfo;
        processActionCommand("end_stiffness", cinfo);
    }
    float cx = customClampedValue(_stiffnessTime, 0, 30, 1);
    float lx = customClampedValue(_stiffnessTime - delta, 0, 30, 1);
    float offx = (cx - lx) * _horizontalDirection;
    return cocos2d::Vec2(offx, 0);
}

void CharacterStateMachine::registerOnStateChange(std::function<void(const StateChangeInfo&)> callback) {
    _onStateChangeCallback = callback;
}

void CharacterStateMachine::unregisterOnStateChange() {
    _onStateChangeCallback = nullptr;
}

float CharacterStateMachine::customClampedValue(float value, float minValue, float maxValue, float curveFactor) {
    if (value <= minValue) return minValue;
    if (value >= maxValue) return maxValue;

    float t = (value - minValue) / (maxValue - minValue);
    return minValue + (maxValue - minValue) * (1 - std::pow(1 - t, 2)) * curveFactor;
}