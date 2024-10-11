#ifndef BATTLEDATAMODEL_H
#define BATTLEDATAMODEL_H

#include <iostream>
#include <map>
#include <string>
#include "cocos2d.h"
#include "GameCharacter.h"

enum BattleDataTokens
{
    PlayerPosX,
    PlayerPosY,
    PlayerHP,
    PlayerMP,
    PlayerHPMAX,
    PlayerMPMAX,
    BossPosX,
    BossPosY,
    BossHP,
    BossMP,
    BossHPMAX,
    BossMPMAX
};

struct HurtData
{
    std::string name;
    int dir;
    int damage;
};

class BattleDataModel {
private:
    std::map<BattleDataTokens, int> dataMap;

    // 私有构造函数，确保外部无法直接创建实例
    BattleDataModel() {}

    // 禁止拷贝和赋值
    BattleDataModel(const BattleDataModel&) = delete;
    BattleDataModel& operator=(const BattleDataModel&) = delete;

    cocos2d::EventListenerCustom* _customListener;

public:

    static BattleDataModel& getInstance();

    // 初始化数据的方法
    void init();

    // 清空数据的方法
    void clear();

    // 自定义事件的回调函数
    void onGameCharacterDataUpdate(cocos2d::EventCustom* event);

    std::string hurtEnemyInRange(std::string selfname, float range, int direction,int damage);

    // 访问 dataMap 的函数
    const std::map<BattleDataTokens, int>& getDataMap() const { return dataMap; }
};

#endif