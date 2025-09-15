#pragma once

class Move;

// 행동의 종류 (enum)
enum class PlayerActionType
{
    FIGHT,              // 싸운다
    USE_ITEM,           // 가방
    SWITCH_POKEMON,     // 포켓몬 교체
    RUN,                // 도망친다
    CANCEL              // 선택 취소 (뒤로가기)
};

// 행동의 내용 (struct)
struct BattleAction
{
    PlayerActionType type;
    const Move* move = nullptr;
};