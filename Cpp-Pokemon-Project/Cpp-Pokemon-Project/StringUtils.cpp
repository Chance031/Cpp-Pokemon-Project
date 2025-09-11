#include "StringUtils.h"

#include <map>
#include <iostream>
#include <sstream>

namespace StringUtils
{
    std::vector<std::string> StringUtils::parseCsvLine(const std::string& line)
    {
        std::vector<std::string> result;
        std::string field;
        bool in_quotes = false;

        for (char c : line)
        {
            if (c == '"')
            {
                in_quotes = !in_quotes;
            }
            else if (c == ',' && !in_quotes)
            {
                result.push_back(field);
                field.clear();
            }
            else
            {
                field += c;
            }
        }
        result.push_back(field);
        return result;
    }

    Type StringUtils::StringToType(const std::string& str)
    {
        // static: 이 맵은 프로그램 시작 시 단 한 번만 생성됩니다.
        static const std::map<std::string, Type> stringToTypeMap = 
        {
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

        if (it != stringToTypeMap.end()) 
        {
            return it->second; // 맵에서 값을 찾으면 해당 Type 반환
        }

        return Type::NONE; // 찾지 못하면 기본값 NONE 반환
    }

    ExpGroup StringUtils::StringToExpGroup(const std::string& str)
    {
        static const std::map<std::string, ExpGroup> stringToExpGroupMap = 
        {
                {"ERRATIC", ExpGroup::ERRATIC},
                {"FAST", ExpGroup::FAST},
                {"MEDIUM_FAST", ExpGroup::MEDIUM_FAST},
                {"MEDIUM_SLOW", ExpGroup::MEDIUM_SLOW},
                {"SLOW", ExpGroup::SLOW},
                {"FLUCTUATING", ExpGroup::FLUCTUATING}
        };

        auto it = stringToExpGroupMap.find(str);

        if (it != stringToExpGroupMap.end()) 
        {
            return it->second;
        }

        return ExpGroup::MEDIUM_FAST; // 기본값
    }

    MoveCategory StringUtils::StringToMoveCategory(const std::string& str)
    {
        static const std::map<std::string, MoveCategory> map = {
            {"PHYSICAL", MoveCategory::PHYSICAL},
            {"SPECIAL", MoveCategory::SPECIAL},
            {"STATUS", MoveCategory::STATUS}
        };
        auto it = map.find(str);
        return (it != map.end()) ? it->second : MoveCategory::STATUS;
    }

    MoveTarget StringUtils::StringToMoveTarget(const std::string& str)
    {
        static const std::map<std::string, MoveTarget> map =
        {
            {"SELECTED_TARGET",        MoveTarget::SELECTED_TARGET},
            {"SELF",                   MoveTarget::SELF},
            {"ALLY",                   MoveTarget::ALLY},
            {"RANDOM_TARGET",          MoveTarget::RANDOM_TARGET},
            {"ALL_ADJACENT_FOES",      MoveTarget::ALL_ADJACENT_FOES},
            {"ALL_ADJACENT_POKEMON",   MoveTarget::ALL_ADJACENT_POKEMON},
            {"FIELD",                  MoveTarget::FIELD},
            {"SPECIAL",                MoveTarget::SPECIAL}
        };
        auto it = map.find(str);
        return (it != map.end()) ? it->second : MoveTarget::SELECTED_TARGET;
    }

    EffectCategory StringToEffectCategory(const std::string& str)
    {
        static const std::map<std::string, EffectCategory> map = {
            {"NONE", EffectCategory::NONE},
            {"PRIMARY_STATUS", EffectCategory::PRIMARY_STATUS},
            {"MULTI_STATUS", EffectCategory::MULTI_STATUS},
            {"VOLATILE_STATUS", EffectCategory::VOLATILE_STATUS},
            {"STAT_CHANGE", EffectCategory::STAT_CHANGE},
            {"MULTI_STAT_CHANGE", EffectCategory::MULTI_STAT_CHANGE},
            {"HEAL", EffectCategory::HEAL},
            {"DAMAGE", EffectCategory::DAMAGE},

            {"MULTI_HIT", EffectCategory::MULTI_HIT},
            {"CHARGE_TURN", EffectCategory::CHARGE_TURN},
            {"TWO_TURN_ATTACK", EffectCategory::TWO_TURN_ATTACK},
            {"RECHARGE_TURN", EffectCategory::RECHARGE_TURN},
            {"RECOIL", EffectCategory::RECOIL},
            {"DRAIN", EffectCategory::DRAIN},
            {"INSTANT_EFFECT", EffectCategory::INSTANT_EFFECT},
            {"FIXED_DAMAGE", EffectCategory::FIXED_DAMAGE},
            {"POST_BATTLE", EffectCategory::POST_BATTLE}
        };
        auto it = map.find(str);
        if (it == map.end()) {
            std::cout << "[StringUtils 경고] 알 수 없는 EffectCategory: '" << str << "'\n";
            return EffectCategory::NONE;
        }
        return it->second;
    }

    Stat StringToStat(const std::string& str)
    {
        static const std::map<std::string, Stat> map = {
            {"HP", Stat::HP},
            {"ATTACK", Stat::ATTACK},
            {"DEFENSE", Stat::DEFENSE},
            {"SPECIAL_ATTACK", Stat::SPECIAL_ATTACK},
            {"SPECIAL_DEFENSE", Stat::SPECIAL_DEFENSE},
            {"SPEED", Stat::SPEED},
            {"ACCURACY", Stat::ACCURACY},
            {"EVASION", Stat::EVASION}
        };
        auto it = map.find(str);
        if (it == map.end()) {
            std::cout << "[StringUtils 경고] 알 수 없는 Stat: '" << str << "'\n";
            // 기본값으로 ATTACK을 반환하거나, 에러 처리를 할 수 있습니다.
            return Stat::ATTACK;
        }
        return it->second;
    }

    StatusCondition StringToStatusCondition(const std::string& str)
    {
        static const std::map<std::string, StatusCondition> map = {
            {"NONE", StatusCondition::NONE},
            {"POISON", StatusCondition::POISON},
            {"TOXIC", StatusCondition::TOXIC},
            {"BURN", StatusCondition::BURN},
            {"PARALYSIS", StatusCondition::PARALYSIS},
            {"SLEEP", StatusCondition::SLEEP},
            {"FREEZE", StatusCondition::FREEZE}
        };
        auto it = map.find(str);
        if (it == map.end()) {
            std::cout << "[StringUtils 경고] 알 수 없는 StatusCondition: '" << str << "'\n";
            return StatusCondition::NONE;
        }
        return it->second;
    }

    // [핵심] "param_1" 컬럼 번역기 (휘발성 상태이상용)
    VolatileStatus StringToVolatileStatus(const std::string& str)
    {
        static const std::map<std::string, VolatileStatus> map = {
            {"NONE", VolatileStatus::NONE},
            {"CONFUSION", VolatileStatus::CONFUSION},
            {"FLINCH", VolatileStatus::FLINCH},
            {"LEECH_SEED", VolatileStatus::LEECH_SEED},
            {"TRAPPED", VolatileStatus::TRAPPED}
        };
        auto it = map.find(str);
        if (it == map.end()) {
            std::cout << "[StringUtils 경고] 알 수 없는 VolatileStatus: '" << str << "'\n";
            return VolatileStatus::NONE;
        }
        return it->second;
    }

    std::pair<Stat, Stat> StringUtils::GetNatureStatMods(Nature nature)
    {
        switch (nature)
        {
            // 공격 상승
        case Nature::LONELY:  return { Stat::ATTACK, Stat::DEFENSE };
        case Nature::ADAMANT: return { Stat::ATTACK, Stat::SPECIAL_ATTACK };
        case Nature::NAUGHTY: return { Stat::ATTACK, Stat::SPECIAL_DEFENSE };
        case Nature::BRAVE:   return { Stat::ATTACK, Stat::SPEED };
                          // 방어 상승
        case Nature::BOLD:    return { Stat::DEFENSE, Stat::ATTACK };
        case Nature::IMPISH:  return { Stat::DEFENSE, Stat::SPECIAL_ATTACK };
        case Nature::LAX:     return { Stat::DEFENSE, Stat::SPECIAL_DEFENSE };
        case Nature::RELAXED: return { Stat::DEFENSE, Stat::SPEED };
                            // 특공 상승
        case Nature::MODEST:  return { Stat::SPECIAL_ATTACK, Stat::ATTACK };
        case Nature::MILD:    return { Stat::SPECIAL_ATTACK, Stat::DEFENSE };
        case Nature::RASH:    return { Stat::SPECIAL_ATTACK, Stat::SPECIAL_DEFENSE };
        case Nature::QUIET:   return { Stat::SPECIAL_ATTACK, Stat::SPEED };
                          // 특방 상승
        case Nature::CALM:    return { Stat::SPECIAL_DEFENSE, Stat::ATTACK };
        case Nature::GENTLE:  return { Stat::SPECIAL_DEFENSE, Stat::DEFENSE };
        case Nature::CAREFUL: return { Stat::SPECIAL_DEFENSE, Stat::SPECIAL_ATTACK };
        case Nature::SASSY:   return { Stat::SPECIAL_DEFENSE, Stat::SPEED };
                          // 스피드 상승
        case Nature::TIMID:   return { Stat::SPEED, Stat::ATTACK };
        case Nature::HASTY:   return { Stat::SPEED, Stat::DEFENSE };
        case Nature::JOLLY:   return { Stat::SPEED, Stat::SPECIAL_ATTACK };
        case Nature::NAIVE:   return { Stat::SPEED, Stat::SPECIAL_DEFENSE };
                          // 보정 없음
        default:              return { Stat::HP, Stat::HP }; // 오르고 내리는 스탯이 없음
        }
    }
}