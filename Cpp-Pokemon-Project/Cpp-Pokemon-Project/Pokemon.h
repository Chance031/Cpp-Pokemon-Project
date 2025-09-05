#pragma once

#include <vector>

#include "PokemonData.h"

class Move;

class Pokemon
{
public:
	// ������
	Pokemon(const PokemonSpecies& species, const PokemonIndividual& individual);

	// --- ���� �������̽� (Public API) ---

	// Getter �Լ�
	
private:
	// --- ����� ���� (Internal Logic) ---

	// 1. �Һ� ������ (���ϸ� ���� ����)
	const PokemonSpecies& species_;

	// 2. ���� ������ (���ϸ� ��ü ����)
	PokemonIndividual individual_;

	// 3. ���� ���� ���� (Derived State)
	std::map<Stat, int> finalStats_;	// HP, ����, ��� �� ���� �ɷ�ġ
	int currentHp_;
	std::vector<Move> moveset_;	// �����
};