#pragma once

class Move;

// 행동의 종류 (enum)
enum class PlayerActionType
{
    FIGHT,
    USE_ITEM,
    SWITCH_POKEMON,
    RUN,
    CANCEL
};

// 행동의 내용 (struct)
struct BattleAction
{
    PlayerActionType type;
    const Move* move = nullptr;
};