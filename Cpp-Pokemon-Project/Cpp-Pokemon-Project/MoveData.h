#pragma once

#include <string>

#include "Enums.h"

struct MoveData
{
	// 1. 기본 식별 정보
	int id = 0;
	std::string identifier = "";
	std::string name_kr = "";
	std::string name_en = "";

	// 2. 핵심 전투 수치
	Type type = Type::NONE;
	MoveCategory category = MoveCategory::STATUS;
	int power = 0;
	int accuracy = 0;
	int pp = 0;
	int priority = 0;
	MoveTarget target = MoveTarget::SELECTED_TARGET;

	// 3. 부가 효과
	std::string effect_id = "NONE";
	int effect_chance = 0;

	// 4. 상세 설명
	std::string description_kr = "";
	std::string description_en = "";

	// 5. 전투 상호작용 플래그
	bool has_high_crit_rate = false;
	bool is_contact = false;
	bool is_protectable = false;
	bool is_reflectable = false;
	bool defrosts_user = false;

	// 6. 기술 특성 플래그
	bool is_punch = false;
	bool is_biting = false;
	bool is_slicing = false;
	bool is_sound = false;
	bool is_powder = false;
	bool is_bullet = false;
	bool is_wind = false;
};