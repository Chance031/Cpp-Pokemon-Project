#pragma once

class Move;

// �ൿ�� ���� (enum)
enum class PlayerActionType
{
    FIGHT,
    USE_ITEM,
    SWITCH_POKEMON,
    RUN,
    CANCEL
};

// �ൿ�� ���� (struct)
struct BattleAction
{
    PlayerActionType type;
    const Move* move = nullptr;
};