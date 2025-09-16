#pragma once

#include "Enums.h"
#include "Move.h"
#include "Pokemon.h"

class Move; // ���� ����

// �÷��̾��� '����'�� ��� ����ü
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
	int switchPokemonIndex = -1; // ��ü�� ���ϸ��� ��Ƽ �ε��� (0���� ����)
};

// Ȯ���� '�� �ൿ' ������ ��� ����ü
struct TurnAction
{
	Pokemon* attacker;
	Pokemon* target;
	Move* move;
};

// ������ ��� '���'�� ��� ����ü
struct DamageResult
{
	int damageDealt = 0;
	double typeEffectiveness = 1.0;
};