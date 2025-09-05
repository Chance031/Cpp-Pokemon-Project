#include "StringUtils.h"

Type StringUtils::StringToType(const std::string& str)
{
    if (str == "NORMAL") return Type::NORMAL;       // �븻
    if (str == "FIRE") return Type::FIRE;           // ��
    if (str == "WATER") return Type::WATER;         // ��
    if (str == "GRASS") return Type::GRASS;         // Ǯ
    if (str == "ELECTRIC") return Type::ELECTRIC;   // ����
    if (str == "ICE") return Type::ICE;             // ����
    if (str == "FIGHTING") return Type::FIGHTING;   // ����
    if (str == "POISON") return Type::POISON;       // ��
    if (str == "GROUND") return Type::GROUND;       // ��
    if (str == "FLYING") return Type::FLYING;       // ����
    if (str == "PSYCHIC") return Type::PSYCHIC;     // ������
    if (str == "BUG") return Type::BUG;             // ����
    if (str == "ROCK") return Type::ROCK;           // ����
    if (str == "GHOST") return Type::GHOST;         // ��Ʈ
    if (str == "DRAGON") return Type::DRAGON;       // �巡��
    if (str == "DARK") return Type::DARK;           // ��
    if (str == "STEEL") return Type::STEEL;         // ��ö
    if (str == "FAIRY") return Type::FAIRY;         // ����

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
