#pragma once

#include <string>
#include <utility>
#include <vector>

#include "Enums.h"

namespace StringUtils
{
    // CSV �� ���� �Ľ��ϴ� ���� �Լ�
    std::vector<std::string> parseCsvLine(const std::string& line);

    // --- CSV ���ڿ��� Enum���� ��ȯ�ϴ� �Լ��� ---
    Type StringToType(const std::string& str);
    ExpGroup StringToExpGroup(const std::string& str);
    MoveCategory StringToMoveCategory(const std::string& str);

    // [MoveTarget] CSV�� "target" �÷��� ��ȯ
    MoveTarget StringToMoveTarget(const std::string& str);

    // [EffectCategory] CSV�� "category" �÷��� ��ȯ (���� �߿�!)
    EffectCategory StringToEffectCategory(const std::string& str);

    // [Stat] CSV�� "param_1" (�ɷ�ġ)�� ��ȯ
    Stat StringToStat(const std::string& str);

    // [StatusCondition] CSV�� "param_1" (�����̻�)�� ��ȯ
    StatusCondition StringToStatusCondition(const std::string& str);

    // [VolatileStatus] CSV�� "param_1" (�ֹ߼� �����̻�)�� ��ȯ
    VolatileStatus StringToVolatileStatus(const std::string& str);

    // [Nature] ���� ���� �Լ�
    std::pair<Stat, Stat> GetNatureStatMods(Nature nature);
}