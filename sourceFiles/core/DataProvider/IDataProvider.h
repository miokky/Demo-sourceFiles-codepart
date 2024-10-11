#include <iostream>
#include <string>
#include <functional>

template<typename T>
class IDataProvider {
private:
    T data;
    std::function<void(T)> callback;

public:
    IDataProvider(T initValue) : data(initValue) {}

    // 注册回调函数
    void registerCallback(std::function<void(T)> cb) {
        callback = cb;
    }

    // 设置数据值，如果有对应的回调函数则调用
    void setData(T value) {
        data = value;
        if (callback) {
            callback(value);
        }
    }

    T getData() const {
        return data;
    }

    ~IDataProvider() {
        callback = nullptr;
    }
};