#pragma once

#include <string>

class Skill
{
public:
	// 생성자 선언
	Skill(std::string name, int power, int accuracy, int maxPp);

	// Getter 함수
	std::string getName() const { return name_; }
	int getPower() const { return power_; }
	int getAccuracy() const { return accuracy_; }
	int getMaxPp() const { return maxPp_; }
	int getCurrentPp() const { return currentPp_; }

private:
	std::string name_;
	int power_;
	int accuracy_;
	int maxPp_;
	int currentPp_;
};