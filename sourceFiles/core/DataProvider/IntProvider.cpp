#include <iostream>
#include <functional>
#include "IDataProvider.h"  

class IntProvider : public IDataProvider {
private:
    std::string key;
    std::function<void(int)> callback;
    int value;

public:
    // ʵ��ע��ص������ķ���
    void registerCallback(const std::string& key_,int initValue, std::function<void(int)> callback_) {
        key = key_;
        callback = callback_;
        value = initValue;
        callback(value);
    }

    // ʵ����������ֵ�ķ���
    void setData(int nValue) {
        if (value != nValue)
        {
            value = nValue;
            callback(value);
        }
    }
};