#include "StateMachine.h"
#include <iostream>

template<typename T>
StateMachine<T>::StateMachine(T initialState) : currentState(initialState) {}

template<typename T>
void StateMachine<T>::changeState(T newState) {
    T oldState = currentState;
    std::cout << "Transitioning from " << static_cast<int>(oldState) << " to " << static_cast<int>(newState) << "." << std::endl;
    currentState = newState;
    if (stateChangeCallback) {
        stateChangeCallback(oldState, newState);
    }
}

template<typename T>
void StateMachine<T>::addState(T newState) {
    // 这里可以根据需要进行一些额外的初始化操作
    std::cout << "Added new state: " << static_cast<int>(newState) << "." << std::endl;
}

template<typename T>
void StateMachine<T>::setStateChangeCallback(std::function<void(T, T)> callback) {
    stateChangeCallback = callback;
}

template<typename T>
T StateMachine<T>::getCurrentState() const {
    return currentState;
}