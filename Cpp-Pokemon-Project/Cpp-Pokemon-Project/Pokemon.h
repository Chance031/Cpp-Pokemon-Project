#pragma once

#include <vector>
#include <string>

#include "AbilityData.h"
#include "MoveData.h"
#include "PokemonData.h"

class Move;

class Pokemon
{
public:
	// 생성자
	Pokemon(const PokemonSpecies& species, const PokemonIndividual& individual);
	
	// =================================================================
	// 공개 인터페이스 (Public API)
	// =================================================================

	// --- 정보 조회 (Getters) ---
	const std::string& GetName() const;
	int GetLevel() const { return individual_.level; }
	int GetCurrentHP() const { return currentHp_; }
	int GetMaxHP() const { return maxHp_; }
	int GetStat(Stat stat) const;
	const std::vector<Move>& GetMoveset() const;
	std::vector<Move>& GetMovesetForModify() { return moveset_; }
	const AbilityData* GetActiveAbility() const { return activeAbility_; }
	Gender GetGender() const { return individual_.gender; }
	StatusCondition GetPrimaryStatus() const { return individual_.primaryStatus; }
	Type GetType1() const { return species_->type1; }
	Type GetType2() const { return species_->type2; }

	// --- 상태 확인 (Checkers) ---
	bool IsFainted() const { return currentHp_ <= 0; }
	bool HasVolatileStatus(VolatileStatus status) const;

	// --- 상태 변경 (State Modifiers) ---
	void TakeDamage(int damage);
	void Heal(int amount);
	void LearnMove(const MoveData& moveData);
	void Evolve(const PokemonSpecies& newSpecies);
	void SetPrimaryStatus(StatusCondition newStatus);
	void AddVolatileStatus(VolatileStatus newStatus);
	void ClearVolatileStatuses();
	void UseRareCandy();

	// --- 성장 (Growth) ---
	void AddExp(int amount);
	
private:
	// =================================================================
	// 비공개 로직 (Internal Logic)
	// =================================================================

	LevelUpResult LevelUp();
	void RecalculateStats();
	int CalculateStatInternal(Stat stat, double natureMod) const;
	void DetermineActiveAbility();

	// --- 멤버 변수 ---

	// 불변 데이터 (포켓몬 종족 정보)
	const PokemonSpecies* species_;

	// 가변 데이터 (포켓몬 개체 정보)
	PokemonIndividual individual_;

	// 계산된 현재 상태 (Derived State)
	int currentHp_ = 0;								// 현재 HP
	int maxHp_ = 0;									// 최대 HP
	std::map<Stat, int> finalStats_{};				// 실수치
	std::vector<Move> moveset_{};					// 기술
	const AbilityData* activeAbility_ = nullptr;	// 특성
	// StatusCondition primaryStatus_;
};