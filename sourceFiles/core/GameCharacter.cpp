#include "GameCharacter.h"

GameCharacter::GameCharacter(std::unique_ptr<SequenceFrameAnimation> displayObj, const CharacterInitData& initData)
    : _displayObject(std::move(displayObj)),
    _name(initData.name),
    _mapPositionX(initData.mapPositionX),
    _mapPositionY(initData.mapPositionY),
    _currentAction(initData.currentAction),
    _health(initData.health),
    _maxHealth(initData.maxHealth),
    _mana(initData.mana),
    _maxMana(initData.maxMana),
    _manaRecoveryRate(initData.manaRecoveryRate),
    _attackPower(initData.attackPower),
    _moveSpeed(initData.moveSpeed),
    _stateMachine(),
    _facingDirection(initData.initScaleX)
{
    _stateMachine.registerOnStateChange(CC_CALLBACK_1(GameCharacter::onCharacterStateChanged,this));
    
    _attackStateMachine = initData.attackStateMachine;
    _attackStateMachine.get()->registerOnAttackEnd(CC_CALLBACK_0(GameCharacter::onEndAttack,this));
    _attackStateMachine.get()->registerOnDamageCheck(CC_CALLBACK_1(GameCharacter::onDamageCheck,this));

    _displayObject.get()->setFacingDirection(_facingDirection);
}

GameCharacter::~GameCharacter()
{
    _stateMachine.unregisterOnStateChange();
}

SequenceFrameAnimation* GameCharacter::getDisplayObject()
{
    return _displayObject.get();
}

void GameCharacter::processActionCommand(ActionNameEnum command)
{
    std::string commandName;
    CommandInfo cinfo;
    cinfo.horizontalDirection = 0;
    cinfo.verticalDirection = 0;
    switch (command)
    {
        case ActionNameEnum::ACTION_MOVE_UP:
            cinfo.verticalDirection = 1;
            commandName = "move";
            break;
        case ActionNameEnum::ACTION_MOVE_DOWN:
            cinfo.verticalDirection = -1;
            commandName = "move";
            break;
        case ActionNameEnum::ACTION_MOVE_LEFT:
            cinfo.horizontalDirection = -1;
            commandName = "move";
            break;
        case ActionNameEnum::ACTION_MOVE_RIGHT:
            cinfo.horizontalDirection = 1;
            commandName = "move";
            break;
        case ActionNameEnum::ACTION_MOVE_UP_CANCEL:
            cinfo.verticalDirection = 1;
            commandName = "stop_move";
            break;
        case ActionNameEnum::ACTION_MOVE_DOWN_CANCEL:
            cinfo.verticalDirection = -1;
            commandName = "stop_move";
            break;
        case ActionNameEnum::ACTION_MOVE_LEFT_CANCEL:
            cinfo.horizontalDirection = -1;
            commandName = "stop_move";
            break;
        case ActionNameEnum::ACTION_MOVE_RIGHT_CANCEL:
            cinfo.horizontalDirection = 1;
            commandName = "stop_move";
            break;
        case ActionNameEnum::ACTION_ATTACK:
            commandName = "attack";
            break;
        case ActionNameEnum::ACTION_CAST_SKILL_1:
            commandName = "stiffness";
            cinfo.hurtMoveDirection = 1;
            break;
        default:
            break;
    }

    _stateMachine.processActionCommand(commandName, cinfo);
}

void GameCharacter::onCharacterStateChanged(const StateChangeInfo& stateChangeInfo)
{
    int dir = stateChangeInfo.stateInfo.horizontalDirection;
    int hurtdir = -stateChangeInfo.stateInfo.hurtMoveDirection;
    std::string actionName;
    switch (stateChangeInfo.currentState)
    {
    case CharacterState::Stand:
        _currentAction = CharacterAction::Stand;
        _displayObject.get()->play("stand", true);
        break;
    case CharacterState::Moving:
        if (_currentAction != CharacterAction::Moving)
        {
            _currentAction = CharacterAction::Moving;
            _displayObject.get()->play("move", true);
        }
        if (dir != 0)
        {
            _facingDirection = dir;
            _displayObject.get()->setFacingDirection(_facingDirection);
        }
        break;
    case CharacterState::Attacking:
        _currentAction = CharacterAction::Attacking;
        actionName = _attackStateMachine.get()->processStateChange(stateChangeInfo);
        _displayObject.get()->play(actionName, false);
        break;
    case CharacterState::Stiffness:
        _currentAction = CharacterAction::BeingAttacked;
        _displayObject.get()->play("hurt", false);
        if (hurtdir != 0)
        {
            _facingDirection = hurtdir;
            _displayObject.get()->setFacingDirection(hurtdir);
        }
    default:
        break;
    }
}

void GameCharacter::onEndAttack()
{
    CommandInfo cinfo;
    _stateMachine.processActionCommand("end_attack", cinfo);
}

void GameCharacter::update(float deltaTime)
{
    // 根据角色状态和移动速度等更新角色位置
    int lastX = _mapPositionX, lastY = _mapPositionY;
    cocos2d::Vec2 dir = _stateMachine.getMoveDirection();
    switch (_currentAction)
    {
    case CharacterAction::Moving:
        if (_stateMachine.getCurrentState() == CharacterState::Moving)
        {
            _mapPositionX += dir.x * _moveSpeed * deltaTime;
            _mapPositionY += dir.y * _moveSpeed * deltaTime;
        }
       
        break;
    case CharacterAction::Attacking:

        break;
    default:
        break;
    }
    cocos2d::Vec2 offset = _attackStateMachine.get()->update(deltaTime);
    _mapPositionX += offset.x * _facingDirection;
    _mapPositionY += offset.y;

    offset = _stateMachine.update(deltaTime);
    _mapPositionX += offset.x * _facingDirection;
    _mapPositionY += offset.y;

    // 检查边界
    if (_mapPositionX < MapManager::mapXMin)
        _mapPositionX = MapManager::mapXMin;
    if (_mapPositionX > MapManager::mapXMax)
        _mapPositionX = MapManager::mapXMax;
    if (_mapPositionY < MapManager::mapYMin)
        _mapPositionY = MapManager::mapYMin;
    if (_mapPositionY > MapManager::mapYMax)
        _mapPositionY = MapManager::mapYMax;

    if (lastX != _mapPositionX || lastY != _mapPositionY)
    {
        dirtyFlag = true;
    }

    _displayObject.get()->setPosition(cocos2d::Vec2(_mapPositionX, _mapPositionY));

    _displayObject.get()->setZorder( 500 - (int)_mapPositionY );

    publishCharacterData();
}

void GameCharacter::hurtByOther(int damageDir, int damage)
{
    CommandInfo cinfo;
    cinfo.hurtMoveDirection = damageDir;
    _health -= damage;
    _health = _health < 0 ? 0 : _health;
    dirtyFlag = true;
    _stateMachine.processActionCommand("stiffness", cinfo);
}

void GameCharacter::publishCharacterData()
{
    if (!dirtyFlag) return;
    dirtyFlag = false;

    cocos2d::EventCustom customEvent("game_character_update");
    CharacterInitData* d = new CharacterInitData();
    d->name = _name;
    d->health = _health;
    d->maxHealth = _maxHealth;
    d->mana = _mana;
    d->maxMana = _maxMana;
    d->mapPositionX = _mapPositionX;
    d->mapPositionY = _mapPositionY;
    customEvent.setUserData(d);
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&customEvent);
    delete d;
}

void GameCharacter::onDamageCheck(const float& range)
{
    BattleDataModel::getInstance().hurtEnemyInRange(_name, range, _facingDirection ,_attackPower);
}