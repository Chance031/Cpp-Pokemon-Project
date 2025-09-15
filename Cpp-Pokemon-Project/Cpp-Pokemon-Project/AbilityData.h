#pragma once

#include <string>

#include "Enums.h"
#include "Move.h"

struct AbilityData
{
    int id = 0;
    std::string name_en = "";
    std::string name_kr = "";

    AbilityTrigger trigger = AbilityTrigger::PASSIVE;
    AbilityCategory category = AbilityCategory::SPECIAL_LOGIC;

    std::string param_type = "";
    std::string param_value = "";
    std::string secondary_param_type = "";
    std::string secondary_param_value = "";

    MoveTarget target = MoveTarget::SELF;

    std::string description_en = "";
    std::string description_kr = "";
};