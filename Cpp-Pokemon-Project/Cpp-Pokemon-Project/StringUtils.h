#pragma once

#include <string>
#include <utility>
#include <vector>

#include "Enums.h"

namespace StringUtils
{
    // CSV 한 줄을 파싱하는 헬퍼 함수
    std::vector<std::string> parseCsvLine(const std::string& line);

    // --- CSV 문자열을 Enum으로 변환하는 함수들 ---
    Type StringToType(const std::string& str);
    ExpGroup StringToExpGroup(const std::string& str);
    MoveCategory StringToMoveCategory(const std::string& str);

    // [MoveTarget] CSV의 "target" 컬럼을 변환
    MoveTarget StringToMoveTarget(const std::string& str);

    // [EffectCategory] CSV의 "category" 컬럼을 변환 (가장 중요!)
    EffectCategory StringToEffectCategory(const std::string& str);

    // [Stat] CSV의 "param_1" (능력치)을 변환
    Stat StringToStat(const std::string& str);

    // [StatusCondition] CSV의 "param_1" (상태이상)을 변환
    StatusCondition StringToStatusCondition(const std::string& str);

    // [VolatileStatus] CSV의 "param_1" (휘발성 상태이상)을 변환
    VolatileStatus StringToVolatileStatus(const std::string& str);

    // [Nature] 성격 관련 함수
    std::pair<Stat, Stat> GetNatureStatMods(Nature nature);
}