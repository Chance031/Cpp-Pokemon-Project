#pragma once

#include "PokemonData.h"
#include "MoveData.h"

#include <vector>

class Move;

class Pokemon
{
public:
	// ������
	Pokemon(const PokemonSpecies& species, const PokemonIndividual& individual);

	// --- ���� �������̽� (Public API) ---

	// Getter �Լ�
	const std::string& GetName() const { return species_.name; }
	int GetLevel() const { return individual_.level; }
	int GetCurrentHP() const { return currentHp_; }
	int GetStat(Stat stat) const;

	// ���� ���� (State Modifiers)
	void TakeDamage(int damage);
	void Heal(int amount);
	void AddExp(int amount);

	void LearnMove(const MoveData& moveData);
	const std::vector<Move>& GetMoveset() const;
	
private:
	// --- ����� ���� (Internal Logic) ---
	void LevelUp();
	void RecalculateStats();
	int CalculateStatInternal(Stat stat, double natureMod) const;

	// --- ��� ���� ---

	// 1. �Һ� ������ (���ϸ� ���� ����)
	const PokemonSpecies& species_;

	// 2. ���� ������ (���ϸ� ��ü ����)
	PokemonIndividual individual_;

	// 3. ���� ���� ���� (Derived State)
	std::map<Stat, int> finalStats_;	// �Ǽ�ġ
	int currentHp_;						// ���� HP
	std::vector<Move> moveset_;			// ���
	// StatusCondition primaryStatus_;
};