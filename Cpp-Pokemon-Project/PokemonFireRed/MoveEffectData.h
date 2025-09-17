#pragma once

#include <string>
#include <vector>

#include "Enums.h"

// ���� ��ũ ��ȭ ������ ��� ����ü
struct StatChange
{
    Stat stat;      // ��� ���� (��: ATTACK)
    int stages;     // ��ȭ�� (��: +1, -2)
};

// move_effects.csv�� �� ���� �Ľ��Ͽ� ������ ���� ����ü
struct MoveEffectData
{
    int id = 0;
    std::string identifier = "";
    EffectCategory category = EffectCategory::NONE;
    MoveTarget target = MoveTarget::SELECTED_TARGET;

    // ī�װ��� ���� �Ľ̵� ������
    StatusCondition primaryStatus = StatusCondition::NONE;
    VolatileStatus volatileStatus = VolatileStatus::NONE;
    std::vector<StatusCondition> multiStatusOptions{};
    std::vector<StatChange> statChanges{};
};
