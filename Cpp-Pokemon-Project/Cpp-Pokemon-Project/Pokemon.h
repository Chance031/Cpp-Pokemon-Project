#pragma once

#include <string>
#include <vector>
#include "Skill.h"

class Pokemon
{
public:
	// 생성자 선언
	Pokemon(std::string name, int maxHp, int attack, int defense, int speed);

	// Getter 함수
	std::string getName() const { return name_; }
	int getMaxHp() const { return maxHp_; }
	int getCurrentHp() const { return currentHp_; }
	int getAttack() const { return attack_; }
	int getDefense() const { return defense_; }
	int getSpeed() const { return speed_; }

	bool addSkill(const Skill& skill);
	const std::vector<Skill>& getSkills() const;

private:
	std::string name_;
	int maxHp_;
	int currentHp_;
	int attack_;
	int defense_;
	int speed_;

	std::vector<Skill> skills_;
};