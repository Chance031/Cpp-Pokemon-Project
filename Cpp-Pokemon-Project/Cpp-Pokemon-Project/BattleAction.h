#pragma once

class Move;

// �ൿ�� ���� (enum)
enum class PlayerActionType
{
    FIGHT,              // �ο��
    USE_ITEM,           // ����
    SWITCH_POKEMON,     // ���ϸ� ��ü
    RUN,                // ����ģ��
    CANCEL              // ���� ��� (�ڷΰ���)
};

// �ൿ�� ���� (struct)
struct BattleAction
{
    PlayerActionType type;
    const Move* move = nullptr;
};