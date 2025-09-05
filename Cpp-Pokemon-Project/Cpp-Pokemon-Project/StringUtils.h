#pragma once

#include <string>
#include "Enums.h"

namespace StringUtils
{
	Type StringToType(const std::string& str);

	ExpGroup StringToExpGroup(const std::string& str);
}