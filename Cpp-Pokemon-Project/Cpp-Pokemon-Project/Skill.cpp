#include "Skill.h"

// ������ ����
Skill::Skill(std::string name, Type type, SkillCategory category, int power, int accuracy, int maxPp)
	: name_(name), type_(type), category_(category), power_(power), accuracy_(accuracy), maxPp_(maxPp)
{
	// ���� �� PP�� �ִ�� ä����
	currentPp_ = maxPp_;
}