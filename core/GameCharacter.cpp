#include "GameCharacter.h"

GameCharacter::GameCharacter(std::unique_ptr<SequenceFrameAnimation> displayObj, const CharacterInitData& initData)
    : _displayObject(std::move(displayObj)),
    _mapPositionX(initData.mapPositionX),
    _mapPositionY(initData.mapPositionY),
    _currentAction(initData.currentAction),
    _health(initData.health),
    _maxHealth(initData.maxHealth),
    _mana(initData.mana),
    _maxMana(initData.maxMana),
    _manaRecoveryRate(initData.manaRecoveryRate),
    _attackPower(initData.attackPower)
{
}

GameCharacter::~GameCharacter()
{

}

SequenceFrameAnimation* GameCharacter::getDisplayObject()
{
    return _displayObject.get();
}