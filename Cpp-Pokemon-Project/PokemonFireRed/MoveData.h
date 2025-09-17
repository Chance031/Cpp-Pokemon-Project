#pragma once

#include <string>

#include "Enums.h"

struct MoveData
{
	// 1. �⺻ �ĺ� ����
	int id = 0;
	std::string identifier = "";
	std::string name_kr = "";
	std::string name_en = "";

	// 2. �ٽ� ���� ��ġ
	Type type = Type::NONE;
	MoveCategory category = MoveCategory::STATUS;
	int power = 0;
	int accuracy = 0;
	int pp = 0;
	int priority = 0;
	MoveTarget target = MoveTarget::SELECTED_TARGET;

	// 3. �ΰ� ȿ��
	int effect_id = 0; // move_effects.csv�� ID ����
	int effect_chance = 0;

	// 4. �� ����
	std::string description_kr = "";
	std::string description_en = "";

	// 5. ��� ���� �÷���
	bool has_high_crit_rate = false;
	bool is_contact = false;
	bool is_protectable = false;
	bool is_reflectable = false;
	bool defrosts_user = false;
	bool is_punch = false;
	bool is_biting = false;
	bool is_slicing = false;
	bool is_sound = false;
	bool is_powder = false;
	bool is_bullet = false;
	bool is_wind = false;
	bool is_snatchable = false;
	bool is_affected_by_gravity = false;
	bool is_copyable = false;
	bool is_callable_by_metronome = false;
};