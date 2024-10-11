#include "BattleDataModel.h"

BattleDataModel& BattleDataModel::getInstance() {
    static BattleDataModel instance;
    return instance;
}


void BattleDataModel::init() {
    // 初始化数据
    dataMap[PlayerPosX] = 0;
    dataMap[PlayerPosY] = 0;
    dataMap[PlayerHP] = 0;
    dataMap[PlayerMP] = 0;
    dataMap[PlayerHPMAX] = 0;
    dataMap[PlayerMPMAX] = 0;
    dataMap[BossPosX] = 0;
    dataMap[BossPosY] = 0;
    dataMap[BossHP] = 0;
    dataMap[BossMP] = 0;
    dataMap[BossHPMAX] = 0;
    dataMap[BossMPMAX] = 0;

    // 注册 _customListener
    _customListener = cocos2d::Director::getInstance()->getEventDispatcher()->addCustomEventListener("game_character_update", CC_CALLBACK_1(BattleDataModel::onGameCharacterDataUpdate, this));
}

void BattleDataModel::clear() {
    // 清理数据
    dataMap.clear();

    // 移除 _customListener
    cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(_customListener);
}

void BattleDataModel::onGameCharacterDataUpdate(cocos2d::EventCustom* event)
{
    CharacterInitData* data = static_cast<CharacterInitData*>(event->getUserData());
    if (data->name == "boss")
    {
        dataMap[BossPosX]       = data->mapPositionX;
        dataMap[BossPosY]       = data->mapPositionY;
        dataMap[BossHP]         = data->health;
        dataMap[BossMP]         = data->mana;
        dataMap[BossHPMAX]      = data->maxHealth;
        dataMap[BossMPMAX]      = data->maxMana;
    }
    else if (data->name == "player")
    {
        dataMap[PlayerPosX]     = data->mapPositionX;
        dataMap[PlayerPosY]     = data->mapPositionY;
        dataMap[PlayerHP]       = data->health;
        dataMap[PlayerMP]       = data->mana;
        dataMap[PlayerHPMAX]    = data->maxHealth;
        dataMap[PlayerMPMAX]    = data->maxMana;
    }

    cocos2d::EventCustom customEvent("game_character_data_change");
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&customEvent);
}

std::string BattleDataModel::hurtEnemyInRange(std::string selfname, float range, int direction,int damage)
{
    std::string hurtTarget = "";
    if (selfname == "player")
    {
        cocos2d::Vec2 dis(dataMap[BossPosX] - dataMap[PlayerPosX], dataMap[BossPosY] - dataMap[PlayerPosY]);
        if (dis.distance(cocos2d::Vec2(0,0)) <= range)
        {
            hurtTarget = "boss";
        }
    }
    else if (selfname == "boss")
    {
        cocos2d::Vec2 dis(dataMap[PlayerPosX] - dataMap[BossPosX], dataMap[PlayerPosY] - dataMap[BossPosY]);
        if (dis.distance(cocos2d::Vec2(0, 0)) <= range)
        {
            hurtTarget = "player";
        }
    }

    cocos2d::EventCustom customEvent("game_character_hurt");
    HurtData* hd = new HurtData();
    hd->damage = damage;
    hd->name = hurtTarget;
    hd->dir = direction;
    customEvent.setUserData(hd);
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&customEvent);

    delete hd;

    return "";
}