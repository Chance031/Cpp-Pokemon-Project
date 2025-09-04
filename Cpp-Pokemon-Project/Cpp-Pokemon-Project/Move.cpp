#include "Skill.h"

// 생성자 구현
Skill::Skill(std::string name, Type type, SkillCategory category, int power, int accuracy, int maxPp)
	: name_(name), type_(type), category_(category), power_(power), accuracy_(accuracy), maxPp_(maxPp)
{
	// 생성 시 PP를 최대로 채워줌
	currentPp_ = maxPp_;
}