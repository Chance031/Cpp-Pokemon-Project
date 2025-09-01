#pragma once

#include <string>

#include "Enums.h"

class Skill
{
public:
	// 생성자 선언
	Skill(std::string name, Type type, SkillCategory category, int power, int accuracy, int maxPp);

	// Getter 함수
	std::string getName() const { return name_; }
	int getPower() const { return power_; }
	int getAccuracy() const { return accuracy_; }
	int getMaxPp() const { return maxPp_; }
	int getCurrentPp() const { return currentPp_; }

private:
	std::string name_;			// 이름
	Type type_;					// 기술의 타입
	SkillCategory category_;	// 물리/특수/변화 구분
	int power_;					// 공격력
	int accuracy_;				// 명중률
	int maxPp_;					// 최대 PP
	int currentPp_;				// 현재 PP
};