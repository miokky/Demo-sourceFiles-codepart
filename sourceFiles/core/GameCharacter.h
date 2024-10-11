#ifndef GAME_CHARACTER_H
#define GAME_CHARACTER_H

#include "cocos2d.h"
#include "SequenceFrameAnimation.h"
#include "CharacterStateMachine.h"
#include "KeyboardEventHandler.h"
#include "MapManager.h"
#include "IAttackStateMachine.h"
#include "BattleDataModel.h"
#include <string>

// 定义枚举类型表示当前行为
enum class CharacterAction
{
    Stand,
    Moving,
    Casting,
    Attacking,
    BeingAttacked,
    Dead
};

// 初始化数据结构体
struct CharacterInitData
{
    std::string name;
    int mapPositionX;
    int mapPositionY;
    CharacterAction currentAction;
    int health;
    int maxHealth;
    int mana;
    int maxMana;
    float manaRecoveryRate;
    int attackPower;
    int moveSpeed;
    int initScaleX;
    std::shared_ptr<IAttackStateMachine> attackStateMachine;
};

class GameCharacter
{
public:
    GameCharacter(std::unique_ptr<SequenceFrameAnimation> displayObj, const CharacterInitData& initData);
    ~GameCharacter();

    void update(float deltaTime);
    SequenceFrameAnimation* getDisplayObject();
    void processActionCommand(ActionNameEnum command);
    void hurtByOther(int damageDir, int damage);

private:
    std::unique_ptr<SequenceFrameAnimation> _displayObject;
    std::string _name;
    int _mapPositionX;
    int _mapPositionY;
    CharacterAction _currentAction;
    int _health;
    int _maxHealth;
    int _mana;
    int _maxMana;
    float _manaRecoveryRate;
    int _attackPower;
    int _moveSpeed;
    CharacterStateMachine _stateMachine;
    int _facingDirection = 1;
    std::shared_ptr<IAttackStateMachine> _attackStateMachine;
    bool dirtyFlag = true;

    void onCharacterStateChanged(const StateChangeInfo& stateChangeInfo);
    void onEndAttack();
    void publishCharacterData();

    void onDamageCheck(const float& range);
};

#endif // GAME_CHARACTER_H