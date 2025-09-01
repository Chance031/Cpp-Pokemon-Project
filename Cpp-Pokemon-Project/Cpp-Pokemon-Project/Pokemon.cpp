#include "Pokemon.h"

// 생성자 구현
Pokemon::Pokemon(std::string name, int maxHp, int attack, int defense, int speed)
	: name_(name), maxHp_(maxHp), attack_(attack), defense_(defense), speed_(speed)
{
	// 생성시 HP를 최대로 채워줌
	currentHp_ = maxHp_;
}

bool Pokemon::addSkill(const Skill& skill)
{
	if (skills_.size() < 4)
	{
		skills_.push_back(skill);
		return true;
	}
	else
	{
		return false;
	}
}

const std::vector<Skill>& Pokemon::getSkills() const
{
	return skills_;
}
