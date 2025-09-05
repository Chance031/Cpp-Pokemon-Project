#pragma once

#include <string>

#include "Enums.h"

// 기술의 '원형(Prototype)' 데이터 구조체
// 이 데이터는 한 번 로드된 후 게임 내에서 변하지 않습니다.
struct MoveData
{
	int id;
	std::string name;
	std::string description;

	Type type;
	MoveCategory category;
	int power;
	int accuracy;
	int maxPp;
	int priority;

	MoveTarget moveTarget;

	// 각종 플래그 (Flags)
	bool isContact;
	bool isSoundMove;
	bool isProtectable;
	// ... 기타 필요한 모든 플래그 ...
};