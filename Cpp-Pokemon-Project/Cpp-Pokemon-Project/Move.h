#pragma once

#include <string>

#include "Enums.h"

class Skill
{
public:
	// ������ ����
	Skill(std::string name, Type type, SkillCategory category, int power, int accuracy, int maxPp);

	// Getter �Լ�
	std::string getName() const { return name_; }
	int getPower() const { return power_; }
	int getAccuracy() const { return accuracy_; }
	int getMaxPp() const { return maxPp_; }
	int getCurrentPp() const { return currentPp_; }

private:
	std::string name_;			// �̸�
	Type type_;					// ����� Ÿ��
	SkillCategory category_;	// ����/Ư��/��ȭ ����
	int power_;					// ���ݷ�
	int accuracy_;				// ���߷�
	int maxPp_;					// �ִ� PP
	int currentPp_;				// ���� PP
};