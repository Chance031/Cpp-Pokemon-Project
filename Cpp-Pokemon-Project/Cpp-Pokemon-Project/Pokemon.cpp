#include "Pokemon.h"

// ������ ����
Pokemon::Pokemon(std::string name, int maxHp, int attack, int defense, int speed)
	: name_(name), maxHp_(maxHp), attack_(attack), defense_(defense), speed_(speed)
{
	// ������ HP�� �ִ�� ä����
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
