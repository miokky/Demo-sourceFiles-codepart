#pragma once
#include <functional>
// ״̬��ģ����ͷ�ļ�

template<typename T>
class StateMachine {
public:
    StateMachine(T initialState);

    // �ı�״̬�ķ���
    void changeState(T newState);

    // �����״̬
    void addState(T newState);

    // ����״̬�仯�ص�����
    void setStateChangeCallback(std::function<void(T, T)> callback);

    T getCurrentState() const;

private:
    T currentState;
    std::function<void(T, T)> stateChangeCallback;
};