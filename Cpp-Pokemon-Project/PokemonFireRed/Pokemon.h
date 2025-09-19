#pragma once

#include <map>
#include <string>
#include <vector>

#include "PokemonData.h"
#include "Move.h"

class Pokemon
{
public:
	// --- 생성자 ---
	Pokemon(const PokemonSpecies& species, const PokemonIndividual& individual);
	
	// --- 정보 조회 (Getters) ---
	const std::string& GetName() const;
	int GetLevel() const;
	int GetCurrentHP() const;
	int GetMaxHP() const;
	StatusCondition GetPrimaryStatus() const;
	const std::vector<Move>& GetMoveset() const;
	std::vector<Move>& GetMovesetForModify(); // 기술 PP 등을 변경해야 할 때 사용
	const AbilityData* GetAbility() const;
	Type GetType1() const;
	Type GetType2() const;
	const PokemonSpecies& GetSpecies() const;

	// --- 상태 확인 (Checkers) ---
	bool IsFainted() const;

	// --- 상태 변경 (State Modifiers) ---
	void TakeDamage(int damage);
	LevelUpResult AddExp(int amount);
	void LearnMove(const MoveData& moveData);
	void SetPrimaryStatus(StatusCondition newStatus);
	void ApplyStatStageChange(Stat stat, int stages);
	void ResetStatStages();

	// --- 성장 (Growth) ---
	int GetStat(Stat stat) const; // 최종 계산된 현재 능력치(HP 제외)를 반환
	int GetStatStage(Stat stat) const;
	
private:
	// --- 내부 로직 ---
	void RecalculateStats(); // 레벨업 또는 개체값 변경 시 능력치를 다시 계산

	// --- 멤버 변수 ---
	const PokemonSpecies* m_species;  // 포켓몬 종족 데이터 (불변)
	PokemonIndividual m_individual; // 포켓몬 개체 데이터 (가변)

	std::vector<Move> m_moveset;      // 현재 배우고 있는 기술 목록
	std::map<Stat, int> m_currentStats; // 레벨, 개체값, 노력치, 성격이 모두 반영된 현재 능력치
	std::map<Stat, int> m_statStages; // 전투 중 랭크 변화 (-6 ~ +6)
	int m_currentHP;                  // 현재 HP
	int m_maxHP;                      // 최대 HP
	const AbilityData* m_ability;     // 현재 적용되는 특성 데이터
};