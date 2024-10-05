#pragma once
#include <functional>
// 状态机模板类头文件

template<typename T>
class StateMachine {
public:
    StateMachine(T initialState);

    // 改变状态的方法
    void changeState(T newState);

    // 添加新状态
    void addState(T newState);

    // 设置状态变化回调函数
    void setStateChangeCallback(std::function<void(T, T)> callback);

    T getCurrentState() const;

private:
    T currentState;
    std::function<void(T, T)> stateChangeCallback;
};