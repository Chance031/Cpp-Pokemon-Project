#pragma once

#include <string>
#include <vector>
#include "Skill.h"

class Pokemon
{
public:
	// TODO

private:
	std::string name_;
	int maxHp_;
	int currentHp_;
	int attack_;
	int defense_;
	int speed_;

	std::vector<Skill> skills_;
};