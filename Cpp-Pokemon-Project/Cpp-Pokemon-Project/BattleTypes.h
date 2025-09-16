#pragma once

#include "Enums.h"
#include "Move.h"
#include "Pokemon.h"

class Move; // 전방 선언

// 플레이어의 '선택'을 담는 구조체
enum class PlayerActionType
{
	FIGHT,
	BAG,
	POKEMON,
	RUN,
	CANCEL
};

struct BattleAction
{
	PlayerActionType type = PlayerActionType::CANCEL;
	Move* move = nullptr; // const 제거!
};

// 확정된 '턴 행동' 정보를 담는 구조체
struct TurnAction
{
	Pokemon* attacker;
	Pokemon* target;
	Move* move;
};

// 데미지 계산 '결과'를 담는 구조체
struct DamageResult
{
	int damageDealt = 0;
	double typeEffectiveness = 1.0;
};