#pragma once

#include "PokemonData.h"
#include "MoveData.h"

#include <vector>

class Move;

class Pokemon
{
public:
	// 생성자
	Pokemon(const PokemonSpecies& species, const PokemonIndividual& individual);

	// --- 공개 인터페이스 (Public API) ---

	// Getter 함수
	const std::string& GetName() const { return species_.name; }
	int GetLevel() const { return individual_.level; }
	int GetCurrentHP() const { return currentHp_; }
	int GetStat(Stat stat) const;

	// 상태 변경 (State Modifiers)
	void TakeDamage(int damage);
	void Heal(int amount);
	void AddExp(int amount);

	void LearnMove(const MoveData& moveData);
	const std::vector<Move>& GetMoveset() const;
	
private:
	// --- 비공개 로직 (Internal Logic) ---
	void LevelUp();
	void RecalculateStats();
	int CalculateStatInternal(Stat stat, double natureMod) const;

	// --- 멤버 변수 ---

	// 1. 불변 데이터 (포켓몬 종족 정보)
	const PokemonSpecies& species_;

	// 2. 가변 데이터 (포켓몬 개체 정보)
	PokemonIndividual individual_;

	// 3. 계산된 현재 상태 (Derived State)
	std::map<Stat, int> finalStats_;	// 실수치
	int currentHp_;						// 현재 HP
	std::vector<Move> moveset_;			// 기술
	// StatusCondition primaryStatus_;
};