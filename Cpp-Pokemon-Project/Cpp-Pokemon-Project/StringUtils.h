#pragma once

#include <string>
#include <utility>
#include <vector>

#include "Enums.h"

namespace StringUtils
{
	// CSV �� ���� �޾� ����ǥ�� ����Ͽ� �ʵ� ���ͷ� �и��ϴ� �Լ�
	std::vector<std::string> parseCsvLine(const std::string& line);

	// CSV�� ���ڿ��� Enum���� ��ȯ�ϴ� �Լ���
	Type StringToType(const std::string& str);
	ExpGroup StringToExpGroup(const std::string& str);
	MoveCategory StringToMoveCategory(const std::string& str);
	MoveTarget StringToMoveTarget(const std::string& str);

	// Nature enum�� �޾� ���� ���� ������ ��ȯ�ϴ� �Լ�
	std::pair<Stat, Stat> GetNatureStatMods(Nature nature);
}