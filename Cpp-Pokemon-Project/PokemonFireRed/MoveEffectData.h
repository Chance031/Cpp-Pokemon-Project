#pragma once

#include <string>
#include <vector>

#include "Enums.h"

// 스탯 랭크 변화 정보를 담는 구조체
struct StatChange
{
    Stat stat;      // 대상 스탯 (예: ATTACK)
    int stages;     // 변화량 (예: +1, -2)
};

// move_effects.csv의 한 줄을 파싱하여 저장할 최종 구조체
struct MoveEffectData
{
    int id = 0;
    std::string identifier = "";
    EffectCategory category = EffectCategory::NONE;
    MoveTarget target = MoveTarget::SELECTED_TARGET;

    // 카테고리에 따라 파싱될 데이터
    StatusCondition primaryStatus = StatusCondition::NONE;
    VolatileStatus volatileStatus = VolatileStatus::NONE;
    std::vector<StatusCondition> multiStatusOptions{};
    std::vector<StatChange> statChanges{};
};
