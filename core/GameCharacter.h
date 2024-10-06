#ifndef GAME_CHARACTER_H
#define GAME_CHARACTER_H

#include "cocos2d.h"
#include "../core/SequenceFrameAnimation.h"

// ����ö�����ͱ�ʾ��ǰ��Ϊ
enum class CharacterAction
{
    Stand,
    Moving,
    Casting,
    Attacking,
    BeingAttacked,
    Dead
};

// ��ʼ�����ݽṹ��
struct CharacterInitData
{
    int mapPositionX;
    int mapPositionY;
    CharacterAction currentAction;
    int health;
    int maxHealth;
    int mana;
    int maxMana;
    float manaRecoveryRate;
    int attackPower;
};

class GameCharacter
{
public:
    GameCharacter(std::unique_ptr<SequenceFrameAnimation> displayObj, const CharacterInitData& initData);
    ~GameCharacter();

    SequenceFrameAnimation* getDisplayObject();
private:
    std::unique_ptr<SequenceFrameAnimation> _displayObject;
    int _mapPositionX;
    int _mapPositionY;
    CharacterAction _currentAction;
    int _health;
    int _maxHealth;
    int _mana;
    int _maxMana;
    float _manaRecoveryRate;
    int _attackPower;
};

#endif // GAME_CHARACTER_H