#include <iostream>
#include <functional>
#include "IDataProvider.h"  

class FloatProvider : public IDataProvider {
private:
    std::string key;
    std::function<void(float)> callback;
    float value;

public:
    // 实现注册回调函数的方法
    void registerCallback(const std::string& key_, float initValue, std::function<void(float)> callback_) {
        key = key_;
        callback = callback_;
        value = initValue;
        callback(value);
    }

    // 实现设置数据值的方法
    void setData(float nValue) {
        if (value != nValue) {
            value = nValue;
            callback(value);
        }
    }
};