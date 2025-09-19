#pragma once

#include <map>
#include <string>
#include <vector>

#include "PokemonData.h"
#include "Move.h"

class Pokemon
{
public:
	// --- ������ ---
	Pokemon(const PokemonSpecies& species, const PokemonIndividual& individual);
	
	// --- ���� ��ȸ (Getters) ---
	const std::string& GetName() const;
	int GetLevel() const;
	int GetCurrentHP() const;
	int GetMaxHP() const;
	StatusCondition GetPrimaryStatus() const;
	const std::vector<Move>& GetMoveset() const;
	std::vector<Move>& GetMovesetForModify(); // ��� PP ���� �����ؾ� �� �� ���
	const AbilityData* GetAbility() const;
	Type GetType1() const;
	Type GetType2() const;
	const PokemonSpecies& GetSpecies() const;

	// --- ���� Ȯ�� (Checkers) ---
	bool IsFainted() const;

	// --- ���� ���� (State Modifiers) ---
	void TakeDamage(int damage);
	LevelUpResult AddExp(int amount);
	void LearnMove(const MoveData& moveData);
	void SetPrimaryStatus(StatusCondition newStatus);
	void ApplyStatStageChange(Stat stat, int stages);
	void ResetStatStages();

	// --- ���� (Growth) ---
	int GetStat(Stat stat) const; // ���� ���� ���� �ɷ�ġ(HP ����)�� ��ȯ
	int GetStatStage(Stat stat) const;
	
private:
	// --- ���� ���� ---
	void RecalculateStats(); // ������ �Ǵ� ��ü�� ���� �� �ɷ�ġ�� �ٽ� ���

	// --- ��� ���� ---
	const PokemonSpecies* m_species;  // ���ϸ� ���� ������ (�Һ�)
	PokemonIndividual m_individual; // ���ϸ� ��ü ������ (����)

	std::vector<Move> m_moveset;      // ���� ���� �ִ� ��� ���
	std::map<Stat, int> m_currentStats; // ����, ��ü��, ���ġ, ������ ��� �ݿ��� ���� �ɷ�ġ
	std::map<Stat, int> m_statStages; // ���� �� ��ũ ��ȭ (-6 ~ +6)
	int m_currentHP;                  // ���� HP
	int m_maxHP;                      // �ִ� HP
	const AbilityData* m_ability;     // ���� ����Ǵ� Ư�� ������
};