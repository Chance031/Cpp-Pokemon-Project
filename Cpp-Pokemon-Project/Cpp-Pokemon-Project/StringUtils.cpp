#include "StringUtils.h"

#include <map>

Type StringUtils::StringToType(const std::string& str)
{
    // static: �� ���� ���α׷� ���� �� �� �� ���� �����˴ϴ�.
    static const std::map<std::string, Type> stringToTypeMap = {
        {"NORMAL", Type::NORMAL},     
        {"FIRE", Type::FIRE},
        {"WATER", Type::WATER},       
        {"GRASS", Type::GRASS},
        {"ELECTRIC", Type::ELECTRIC}, 
        {"ICE", Type::ICE},
        {"FIGHTING", Type::FIGHTING}, 
        {"POISON", Type::POISON},
        {"GROUND", Type::GROUND},     
        {"FLYING", Type::FLYING},
        {"PSYCHIC", Type::PSYCHIC},   
        {"BUG", Type::BUG},
        {"ROCK", Type::ROCK},         
        {"GHOST", Type::GHOST},
        {"DRAGON", Type::DRAGON},     
        {"DARK", Type::DARK},
        {"STEEL", Type::STEEL},       
        {"FAIRY", Type::FAIRY},
        {"NONE", Type::NONE}
    };

    auto it = stringToTypeMap.find(str);
    if (it != stringToTypeMap.end()) {
        return it->second; // �ʿ��� ���� ã���� �ش� Type ��ȯ
    }
    return Type::NONE; // ã�� ���ϸ� �⺻�� NONE ��ȯ
}

ExpGroup StringUtils::StringToExpGroup(const std::string& str)
{
    static const std::map<std::string, ExpGroup> stringToExpGroupMap = {
            {"ERRATIC", ExpGroup::ERRATIC},
            {"FAST", ExpGroup::FAST},
            {"MEDIUM_FAST", ExpGroup::MEDIUM_FAST},
            {"MEDIUM_SLOW", ExpGroup::MEDIUM_SLOW},
            {"SLOW", ExpGroup::SLOW},
            {"FLUCTUATING", ExpGroup::FLUCTUATING}
    };

    auto it = stringToExpGroupMap.find(str);
    if (it != stringToExpGroupMap.end()) {
        return it->second;
    }
    return ExpGroup::MEDIUM_FAST; // �⺻��
}

std::pair<Stat, Stat> StringUtils::GetNatureStatMods(Nature nature)
{
    switch (nature)
    {
    // ���� ���
    case Nature::LONELY:  return { Stat::ATTACK, Stat::DEFENSE };
    case Nature::ADAMANT: return { Stat::ATTACK, Stat::SPECIAL_ATTACK };
    case Nature::NAUGHTY: return { Stat::ATTACK, Stat::SPECIAL_DEFENSE };
    case Nature::BRAVE:   return { Stat::ATTACK, Stat::SPEED };
    // ��� ���
    case Nature::BOLD:    return { Stat::DEFENSE, Stat::ATTACK };
    case Nature::IMPISH:  return { Stat::DEFENSE, Stat::SPECIAL_ATTACK };
    case Nature::LAX:     return { Stat::DEFENSE, Stat::SPECIAL_DEFENSE };
    case Nature::RELAXED: return { Stat::DEFENSE, Stat::SPEED };
    // Ư�� ���
    case Nature::MODEST:  return { Stat::SPECIAL_ATTACK, Stat::ATTACK };
    case Nature::MILD:    return { Stat::SPECIAL_ATTACK, Stat::DEFENSE };
    case Nature::RASH:    return { Stat::SPECIAL_ATTACK, Stat::SPECIAL_DEFENSE };
    case Nature::QUIET:   return { Stat::SPECIAL_ATTACK, Stat::SPEED };
    // Ư�� ���
    case Nature::CALM:    return { Stat::SPECIAL_DEFENSE, Stat::ATTACK };
    case Nature::GENTLE:  return { Stat::SPECIAL_DEFENSE, Stat::DEFENSE };
    case Nature::CAREFUL: return { Stat::SPECIAL_DEFENSE, Stat::SPECIAL_ATTACK };
    case Nature::SASSY:   return { Stat::SPECIAL_DEFENSE, Stat::SPEED };
    // ���ǵ� ���
    case Nature::TIMID:   return { Stat::SPEED, Stat::ATTACK };
    case Nature::HASTY:   return { Stat::SPEED, Stat::DEFENSE };
    case Nature::JOLLY:   return { Stat::SPEED, Stat::SPECIAL_ATTACK };
    case Nature::NAIVE:   return { Stat::SPEED, Stat::SPECIAL_DEFENSE };
    // ���� ����
    default:              return { Stat::HP, Stat::HP }; // ������ ������ ������ ����
    }
}
