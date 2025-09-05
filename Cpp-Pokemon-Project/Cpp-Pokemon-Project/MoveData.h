#pragma once

#include <string>

#include "Enums.h"

// ����� '����(Prototype)' ������ ����ü
// �� �����ʹ� �� �� �ε�� �� ���� ������ ������ �ʽ��ϴ�.
struct MoveData
{
	int id;
	std::string name;
	std::string description;

	Type type;
	MoveCategory category;
	int power;
	int accuracy;
	int maxPp;
	int priority;

	MoveTarget moveTarget;

	// ���� �÷��� (Flags)
	bool isContact;
	bool isSoundMove;
	bool isProtectable;
	// ... ��Ÿ �ʿ��� ��� �÷��� ...
};