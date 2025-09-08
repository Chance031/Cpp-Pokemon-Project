#pragma once

#include <string>
#include <utility>

#include "Enums.h"

namespace StringUtils
{
	Type StringToType(const std::string& str);

	ExpGroup StringToExpGroup(const std::string& str);

	std::pair<Stat, Stat> GetNatureStatMods(Nature nature);
}