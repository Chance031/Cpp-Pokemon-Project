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
        // static: �� ���� ���α׷� ���� �� �� �� ���� ����
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
            return it->second; // �ʿ��� ���� ã���� �ش� Type ��ȯ
        }

        return Type::NONE; // ã�� ���ϸ� �⺻�� NONE ��ȯ
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

        return ExpGroup::MEDIUM_FAST; // �⺻��
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
            std::cout << "[StringUtils ���] �� �� ���� EffectCategory: '" << str << "'\n";
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
            std::cout << "[StringUtils ���] �� �� ���� Stat: '" << str << "'\n";
            // �⺻������ ATTACK�� ��ȯ�ϰų�, ���� ó���� �� �� ����
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
            std::cout << "[StringUtils ���] �� �� ���� StatusCondition: '" << str << "'\n";
            return StatusCondition::NONE;
        }
        return it->second;
    }

    // [�ٽ�] "param_1" �÷� ������ (�ֹ߼� �����̻��)
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
            std::cout << "[StringUtils ���] �� �� ���� VolatileStatus: '" << str << "'\n";
            return VolatileStatus::NONE;
        }
        return it->second;
    }

    AbilityTrigger StringToAbilityTrigger(const std::string& str)
    {
        static const std::map<std::string, AbilityTrigger> map = {
            // --- ���� ȿ�� ---
            {"PASSIVE", AbilityTrigger::PASSIVE},
            // --- ���� / ���� �� ---
            {"ON_SWITCH_IN", AbilityTrigger::ON_SWITCH_IN},
            {"ON_SWITCH_OUT", AbilityTrigger::ON_SWITCH_OUT},
            // --- �� ��� ---
            {"ON_END_TURN", AbilityTrigger::ON_END_TURN},
            // --- �ǰ� / ���� �� ---
            {"ON_CONTACT", AbilityTrigger::ON_CONTACT},
            {"ON_HIT_BY_MOVE", AbilityTrigger::ON_HIT_BY_MOVE},
            {"ON_HIT_BY_TYPE", AbilityTrigger::ON_HIT_BY_TYPE},
            {"ON_HIT_PHYSICAL", AbilityTrigger::ON_HIT_PHYSICAL},
            {"ON_CRITICAL_HIT", AbilityTrigger::ON_CRITICAL_HIT},
            {"ON_MOVE_TARGET", AbilityTrigger::ON_MOVE_TARGET},
            // --- �ൿ �� ---
            {"ON_MOVE_USE", AbilityTrigger::ON_MOVE_USE},
            {"ON_MOVE_LAST", AbilityTrigger::ON_MOVE_LAST},
            // --- ���� ��ȭ �� ---
            {"ON_STATUS", AbilityTrigger::ON_STATUS},
            {"ON_STAT_LOWERED", AbilityTrigger::ON_STAT_LOWERED},
            {"ON_FOE_STAT_UP", AbilityTrigger::ON_FOE_STAT_UP},
            {"ON_FLINCH", AbilityTrigger::ON_FLINCH},
            {"ON_INTIMIDATED", AbilityTrigger::ON_INTIMIDATED},
            // --- HP ��ȭ �� ---
            {"ON_LOW_HP", AbilityTrigger::ON_LOW_HP},
            // --- ����(Faint) ���� ---
            {"ON_KO", AbilityTrigger::ON_KO},
            {"ON_FAINT", AbilityTrigger::ON_FAINT},
            {"ON_FAINT_CONTACT", AbilityTrigger::ON_FAINT_CONTACT},
            {"ON_ANY_FAINT", AbilityTrigger::ON_ANY_FAINT},
            {"ON_ALLY_FAINT", AbilityTrigger::ON_ALLY_FAINT},
            // --- �ʵ� / ���� / ������ ���� ---
            {"ON_WEATHER", AbilityTrigger::ON_WEATHER},
            {"ON_TERRAIN", AbilityTrigger::ON_TERRAIN},
            {"ON_TERRAIN_CHANGE", AbilityTrigger::ON_TERRAIN_CHANGE},
            {"ON_BERRY_EAT", AbilityTrigger::ON_BERRY_EAT},
            {"ON_ALLY_ITEM_USE", AbilityTrigger::ON_ALLY_ITEM_USE},
            {"ON_OTHER_MOVE_USE", AbilityTrigger::ON_OTHER_MOVE_USE},
            {"ON_END_BATTLE", AbilityTrigger::ON_END_BATTLE},
            // --- Ư�� ���� ---
            {"SPECIAL_LOGIC", AbilityTrigger::SPECIAL_LOGIC}
        };
        auto it = map.find(str);
        return (it != map.end()) ? it->second : AbilityTrigger::SPECIAL_LOGIC;
    }

    AbilityCategory StringToAbilityCategory(const std::string& str)
    {
        static const std::map<std::string, AbilityCategory> map = {
            // --- �ɷ�ġ(Stat) ���� ---
            {"STAT_CHANGE", AbilityCategory::STAT_CHANGE},
            {"STAT_MODIFIER", AbilityCategory::STAT_MODIFIER},
            {"STAT_MODIFIER_STATUS", AbilityCategory::STAT_MODIFIER_STATUS},
            {"STAT_MODIFIER_IMMUNITY", AbilityCategory::STAT_MODIFIER_IMMUNITY},
            {"REVERSE_STAT_CHANGES", AbilityCategory::REVERSE_STAT_CHANGES},
            // --- ������ ��� ���� ---
            {"DAMAGE_MODIFIER", AbilityCategory::DAMAGE_MODIFIER},
            {"DAMAGE_MODIFIER_TYPE", AbilityCategory::DAMAGE_MODIFIER_TYPE},
            {"DAMAGE_IMMUNITY", AbilityCategory::DAMAGE_IMMUNITY},
            {"DAMAGE", AbilityCategory::DAMAGE},
            // --- �����̻�(Status) ���� ---
            {"PRIMARY_STATUS", AbilityCategory::PRIMARY_STATUS},
            {"MULTI_STATUS", AbilityCategory::MULTI_STATUS},
            {"VOLATILE_STATUS", AbilityCategory::VOLATILE_STATUS},
            {"STATUS_IMMUNITY", AbilityCategory::STATUS_IMMUNITY},
            {"STATUS_HEAL", AbilityCategory::STATUS_HEAL},
            {"STATUS_TRANSFER", AbilityCategory::STATUS_TRANSFER},
            // --- ���(Move) ���� ---
            {"MOVE_IMMUNITY", AbilityCategory::MOVE_IMMUNITY},
            {"MOVE_MODIFIER", AbilityCategory::MOVE_MODIFIER},
            {"EFFECT_CHANCE_MOD", AbilityCategory::EFFECT_CHANCE_MOD},
            {"PP_USAGE_MODIFIER", AbilityCategory::PP_USAGE_MODIFIER},
            {"ACCURACY_MODIFIER", AbilityCategory::ACCURACY_MODIFIER},
            {"PRIORITY_MODIFIER", AbilityCategory::PRIORITY_MODIFIER},
            {"CRITICAL_HIT_MODIFIER", AbilityCategory::CRITICAL_HIT_MODIFIER},
            // --- Ÿ��(Type) ���� ---
            {"TYPE_CHANGE", AbilityCategory::TYPE_CHANGE},
            {"TYPE_IMMUNITY", AbilityCategory::TYPE_IMMUNITY},
            {"TYPE_EFFECTIVENESS", AbilityCategory::TYPE_EFFECTIVENESS},
            // --- �ʵ�(Field) ���� ---
            {"WEATHER_CHANGE", AbilityCategory::WEATHER_CHANGE},
            {"TERRAIN_CHANGE", AbilityCategory::TERRAIN_CHANGE},
            {"FIELD_EFFECT_NEGATE", AbilityCategory::FIELD_EFFECT_NEGATE},
            // --- ���/ȸ�� ���� ---
            {"HEAL", AbilityCategory::HEAL},
            {"ABSORB_BOOST", AbilityCategory::ABSORB_BOOST},
            {"REDIRECT_BOOST", AbilityCategory::REDIRECT_BOOST},
            // --- ��Ÿ ȿ�� ---
            {"EFFECT_IMMUNITY", AbilityCategory::EFFECT_IMMUNITY},
            {"SURVIVE_LETHAL", AbilityCategory::SURVIVE_LETHAL},
            {"TRAP", AbilityCategory::TRAP},
            {"ITEM_ACTION", AbilityCategory::ITEM_ACTION},
            {"BATTLE_ACTION", AbilityCategory::BATTLE_ACTION},
            {"TRANSFORM", AbilityCategory::TRANSFORM},
            {"WEIGHT_MODIFIER", AbilityCategory::WEIGHT_MODIFIER},
            // --- Ư�� ���� ---
            {"SPECIAL_LOGIC", AbilityCategory::SPECIAL_LOGIC}
        };
        auto it = map.find(str);
        return (it != map.end()) ? it->second : AbilityCategory::SPECIAL_LOGIC;
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

    std::string StatToKorean(Stat stat) {
        switch (stat) {
        case Stat::ATTACK: return "����";
        case Stat::DEFENSE: return "���";
        case Stat::SPECIAL_ATTACK: return "Ư������";
        case Stat::SPECIAL_DEFENSE: return "Ư�����";
        case Stat::SPEED: return "���ǵ�";
        case Stat::ACCURACY: return "���߷�";
        case Stat::EVASION: return "ȸ����";
        default: return "�ɷ�ġ";
        }
    }
}