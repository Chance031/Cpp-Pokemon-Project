#pragma once

#include <string>
#include <utility>
#include <vector>

#include "Enums.h"

namespace StringUtils
{
	// CSV 한 줄을 받아 따옴표를 고려하여 필드 벡터로 분리하는 함수
	std::vector<std::string> parseCsvLine(const std::string& line);

	// CSV의 문자열을 Enum으로 변환하는 함수들
	Type StringToType(const std::string& str);
	ExpGroup StringToExpGroup(const std::string& str);
	MoveCategory StringToMoveCategory(const std::string& str);
	MoveTarget StringToMoveTarget(const std::string& str);

	// Nature enum을 받아 스탯 보정 정보를 반환하는 함수
	std::pair<Stat, Stat> GetNatureStatMods(Nature nature);
}