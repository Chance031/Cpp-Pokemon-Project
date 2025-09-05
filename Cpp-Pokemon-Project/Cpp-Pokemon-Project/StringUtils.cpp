#include "StringUtils.h"

Type StringUtils::StringToType(const std::string& str)
{
    if (str == "NORMAL") return Type::NORMAL;       // 노말
    if (str == "FIRE") return Type::FIRE;           // 불
    if (str == "WATER") return Type::WATER;         // 물
    if (str == "GRASS") return Type::GRASS;         // 풀
    if (str == "ELECTRIC") return Type::ELECTRIC;   // 전기
    if (str == "ICE") return Type::ICE;             // 얼음
    if (str == "FIGHTING") return Type::FIGHTING;   // 격투
    if (str == "POISON") return Type::POISON;       // 독
    if (str == "GROUND") return Type::GROUND;       // 땅
    if (str == "FLYING") return Type::FLYING;       // 비행
    if (str == "PSYCHIC") return Type::PSYCHIC;     // 에스퍼
    if (str == "BUG") return Type::BUG;             // 벌레
    if (str == "ROCK") return Type::ROCK;           // 바위
    if (str == "GHOST") return Type::GHOST;         // 고스트
    if (str == "DRAGON") return Type::DRAGON;       // 드래곤
    if (str == "DARK") return Type::DARK;           // 악
    if (str == "STEEL") return Type::STEEL;         // 강철
    if (str == "FAIRY") return Type::FAIRY;         // 요정

    return Type::NONE;
}

ExpGroup StringUtils::StringToExpGroup(const std::string& str)
{
    if (str == "ERRATIC") return ExpGroup::ERRATIC;
    if (str == "FAST") return ExpGroup::FAST;
    if (str == "MEDIUM_FAST") return ExpGroup::MEDIUM_FAST;
    if (str == "MEDIUM_SLOW") return ExpGroup::MEDIUM_SLOW;
    if (str == "SLOW") return ExpGroup::SLOW;
    if (str == "FLUCTUATING") return ExpGroup::FLUCTUATING;

    return ExpGroup::MEDIUM_FAST;
}
