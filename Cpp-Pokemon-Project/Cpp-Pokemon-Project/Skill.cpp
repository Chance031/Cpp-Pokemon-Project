#include "Skill.h"

// ������ ����
Skill::Skill(std::string name, int power, int accuracy, int maxPp)
	: name_(name), power_(power), accuracy_(accuracy), maxPp_(maxPp)
{
	// ���� �� PP�� �ִ�� ä����
	currentPp_ = maxPp_;
}