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

    // ˽�й��캯����ȷ���ⲿ�޷�ֱ�Ӵ���ʵ��
    BattleDataModel() {}

    // ��ֹ�����͸�ֵ
    BattleDataModel(const BattleDataModel&) = delete;
    BattleDataModel& operator=(const BattleDataModel&) = delete;

    cocos2d::EventListenerCustom* _customListener;

public:

    static BattleDataModel& getInstance();

    // ��ʼ�����ݵķ���
    void init();

    // ������ݵķ���
    void clear();

    // �Զ����¼��Ļص�����
    void onGameCharacterDataUpdate(cocos2d::EventCustom* event);

    std::string hurtEnemyInRange(std::string selfname, float range, int direction,int damage);

    // ���� dataMap �ĺ���
    const std::map<BattleDataTokens, int>& getDataMap() const { return dataMap; }
};

#endif