#pragma once

#include "PokemonData.h"
#include "MoveData.h"

#include <vector>
#include <string>

class Move;

class Pokemon
{
public:
	// ������
	Pokemon(const PokemonSpecies& species, const PokemonIndividual& individual);
	
	// =================================================================
	// ���� �������̽� (Public API)
	// =================================================================

	// --- ���� ��ȸ (Getters) ---
	const std::string& GetName() const;
	int GetLevel() const { return individual_.level; }
	int GetCurrentHP() const { return currentHp_; }
	int GetMaxHP() const { return maxHp_; }
	int GetStat(Stat stat) const;
	const std::vector<Move>& GetMoveset() const;
	const std::string& GetActiveAbility() const { return activeAbility_; }
	Gender GetGender() const { return individual_.gender; }
	StatusCondition GetPrimaryStatus() const { return individual_.primaryStatus; }

	// --- ���� Ȯ�� (Checkers) ---
	bool IsFainted() const { return currentHp_ <= 0; }
	bool HasVolatileStatus(VolatileStatus status) const;

	// --- ���� ���� (State Modifiers) ---
	void TakeDamage(int damage);
	void Heal(int amount);
	void LearnMove(const MoveData& moveData);
	void Evolve(const PokemonSpecies& newSpecies);
	void SetPrimaryStatus(StatusCondition newStatus);
	void AddVolatileStatus(VolatileStatus newStatus);
	void ClearVolatileStatuses();
	void UseRareCandy();

	// --- ���� (Growth) ---
	void AddExp(int amount);
	
private:
	// =================================================================
	// ����� ���� (Internal Logic)
	// =================================================================

	LevelUpResult LevelUp();
	void RecalculateStats();
	int CalculateStatInternal(Stat stat, double natureMod) const;
	void DetermineActiveAbility();

	// --- ��� ���� ---

	// �Һ� ������ (���ϸ� ���� ����)
	const PokemonSpecies* species_;

	// ���� ������ (���ϸ� ��ü ����)
	PokemonIndividual individual_;

	// ���� ���� ���� (Derived State)
	int currentHp_ = 0;						// ���� HP
	int maxHp_ = 0;							// �ִ� HP
	std::map<Stat, int> finalStats_{};		// �Ǽ�ġ
	std::vector<Move> moveset_{};			// ���
	std::string activeAbility_ = "NONE";	// Ư��
	// StatusCondition primaryStatus_;
};