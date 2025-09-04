#pragma once

#include <string>

#include "Enums.h"

class Move
{
public:
	// ������ ����
    Move(int id, const std::string& name, const std::string& description, Type type, MoveCategory category, int power, int accuracy, int pp, int priority, MoveTarget target, bool isContact, bool isSoundMove, bool isSpreadMove, bool isProtectable_, bool canBeRedirected);

	// Getter �Լ�
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

	void DecrementPp(); // PP�� 1 ���ҽ�Ű�� �Լ�

private:
    int id_;                      // ����� �ĺ��ϴ� ���� ID
    std::string name_;            // �̸�
    std::string description_;     // ��� ����

    // �⺻ ����
    Type type_;
    MoveCategory category_;
    int power_;
    int accuracy_;
    int maxPp_;
    int currentPp_;
    int priority_;

    // ��� ����
    MoveTarget moveTarget_;

    // �÷���(Flag) ����
    bool isContact_;        // ����� ���� �������� ������ �ϴ°�
    bool isSoundMove_;      // ����� �Ҹ� �迭�ΰ�
    bool isSpreadMove_;     // ����� ���� ��Ʋ���� ���� ����� ���ÿ� �����ϴ°�
    bool isProtectable_;    // ����� ����� ��� �迭 ����� �����°�
    bool canBeRedirected_;  // ����� Ư���� ���� ����� �ٲ� �� �ִ°�

    // Ư�� �ൿ ����
    MoveExecutionType executionType_;
};