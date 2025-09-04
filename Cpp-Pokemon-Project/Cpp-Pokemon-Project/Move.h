#pragma once

#include <string>

#include "Enums.h"

class Move
{
public:
	// 생성자 선언
    Move(int id, const std::string& name, const std::string& description, Type type, MoveCategory category, int power, int accuracy, int pp, int priority, MoveTarget target, bool isContact, bool isSoundMove, bool isSpreadMove, bool isProtectable_, bool canBeRedirected);

	// Getter 함수
	std::string GetName() const { return name_; }
	Type GetType() const { return type_; }
	MoveCategory GetCategory() const { return category_; }
	int GetPower() const { return power_; }
	int GetAccuracy() const { return accuracy_; }
	int GetCurrentPp() const { return currentPp_; }
    int GetMaxPp() const { return maxPp_; }
	int GetPriority() const { return priority_; }
	bool IsContact() const { return isContact_; }
	MoveTarget GetTargetType() const { return moveTarget_; }

	void DecrementPp(); // PP를 1 감소시키는 함수

private:
    int id_;                      // 기술을 식별하는 고유 ID
    std::string name_;            // 이름
    std::string description_;     // 기술 설명

    // 기본 정보
    Type type_;
    MoveCategory category_;
    int power_;
    int accuracy_;
    int maxPp_;
    int currentPp_;
    int priority_;

    // 대상 정보
    MoveTarget moveTarget_;

    // 플래그(Flag) 정보
    bool isContact_;        // 기술이 상대와 물리적인 접촉을 하는가
    bool isSoundMove_;      // 기술이 소리 계열인가
    bool isSpreadMove_;     // 기술이 더블 배틀에서 여러 대상을 동시에 공격하는가
    bool isProtectable_;    // 기술이 상대의 방어 계열 기술에 막히는가
    bool canBeRedirected_;  // 기술이 특성에 의해 대상이 바뀔 수 있는가

    // 특수 행동 정보
    MoveExecutionType executionType_;
};