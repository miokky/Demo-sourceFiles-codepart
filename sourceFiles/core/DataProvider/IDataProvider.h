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

    // ע��ص�����
    void registerCallback(std::function<void(T)> cb) {
        callback = cb;
    }

    // ��������ֵ������ж�Ӧ�Ļص����������
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