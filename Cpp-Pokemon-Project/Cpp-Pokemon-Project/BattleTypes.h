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
	Move* move = nullptr;
	int switchPokemonIndex = -1; // 교체할 포켓몬의 파티 인덱스 (0부터 시작)
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