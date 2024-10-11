#include <iostream>
#include <functional>
#include "IDataProvider.h"  

class IntProvider : public IDataProvider {
private:
    std::string key;
    std::function<void(int)> callback;
    int value;

public:
    // 实现注册回调函数的方法
    void registerCallback(const std::string& key_,int initValue, std::function<void(int)> callback_) {
        key = key_;
        callback = callback_;
        value = initValue;
        callback(value);
    }

    // 实现设置数据值的方法
    void setData(int nValue) {
        if (value != nValue)
        {
            value = nValue;
            callback(value);
        }
    }
};