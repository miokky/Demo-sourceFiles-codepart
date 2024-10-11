#include <iostream>
#include <functional>
#include "IDataProvider.h"  

class FloatProvider : public IDataProvider {
private:
    std::string key;
    std::function<void(float)> callback;
    float value;

public:
    // ʵ��ע��ص������ķ���
    void registerCallback(const std::string& key_, float initValue, std::function<void(float)> callback_) {
        key = key_;
        callback = callback_;
        value = initValue;
        callback(value);
    }

    // ʵ����������ֵ�ķ���
    void setData(float nValue) {
        if (value != nValue) {
            value = nValue;
            callback(value);
        }
    }
};