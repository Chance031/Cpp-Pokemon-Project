#pragma once

#include "Enums.h"

// ���� ����
class Move;
class Pokemon;

// =================================================================
// �÷��̾��� �ൿ ���� ������ ��� ����ü
// =================================================================

enum class PlayerActionType
{
	FIGHT,		// �ο��
	BAG,		// ����
	POKEMON,	// ���ϸ�
	RUN,		// ����ġ��
	CANCEL
};

struct BattleAction
{
	PlayerActionType type = PlayerActionType::CANCEL;	// � ������ �ൿ�� �����ߴ°�
	Move* move = nullptr;								// '�ο��'�� �����ߴٸ�, � ����ΰ�
	int switchPokemonIndex = -1;						// '���ϸ�'�� �����ߴٸ�, �� ��° ���ϸ����� ��ü�� ���ΰ�
};

// =================================================================
// �� ���� ���(�̺�Ʈ)�� BattleScene�� �����ϱ� ���� ����ü
// =================================================================

enum class TurnEventType
{
	TEXT_MESSAGE,		// �ܼ� �ؽ�Ʈ �޽���
	DAMAGE,				// ���ϸ��� �������� �Դ� �̺�Ʈ
	FAINT,				// ���ϸ��� �����ϴ� �̺�Ʈ
	STAT_CHANGE,		// �ɷ�ġ ��ũ ��ȭ �̺�Ʈ
	STATUS_EFFECT,		// ���� �̻� �߻� �̺�Ʈ
};

struct TurnEvent
{
	TurnEventType type = TurnEventType::TEXT_MESSAGE;	// �� �̺�Ʈ�� ����
	Pokemon* target = nullptr;							// �̺�Ʈ�� ����� �Ǵ� ���ϸ�
	std::string message = "";							// ȭ�鿡 ǥ�õ� ������
	int damage = 0;										// ������ ��
	Stat stat = Stat::HP;								// ��ȭ�� �ɷ�ġ
	int stages = 0;										// �ɷ�ġ ��ȭ��
	StatusCondition status = StatusCondition::NONE;		// ����� ���� �̻�
};

struct TurnResult
{
	std::vector<TurnEvent> events;
};

// =================================================================
// ������ ��� ����� ��� ���ο� ����ü
// =================================================================

struct DamageResult
{
	int damageDealt = 0;
	double typeEffectiveness = 1.0;
};