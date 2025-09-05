#pragma once

#include <string>
#include <map>

#include "Enums.h"

// ���ϸ� '����'�� �Һ� ������
struct PokemonSpecies
{
	int id;				// ���� ��ȣ
	std::string name;	// ���ϸ� �̸�
	Type type1;			// ���ϸ� Ÿ��1
	Type type2;			// ���ϸ� Ÿ��2

	// ������ (Base Stats)
	std::map<Stat, int> baseStats;

	// TODO: Ư�� ���, ��� �� �ִ� ��� ���, ��ȭ ���� �� �߰�
};

// ���ϸ� '��ü'�� ���� ������
struct PokemonIndividual
{
	int level;
	Nature nature;
	std::map<Stat, int> ivs;
	std::map<Stat, int> evs;
	int currentExp;

	// TODO: �г���, ����, ���� ���� �̻� �� �߰�
};