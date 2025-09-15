#pragma once

#include <map>
#include <vector>
#include <string>

#include "AbilityData.h"
#include "PokemonData.h"
#include "MoveData.h"
#include "Enums.h"

class DataManager
{
public:
	// 싱글톤 인스턴스를 가져옵니다.
	static DataManager& GetInstance();

	// 모든 CSV 데이터를 로드합니다.
	void LoadAllData();

	// ID를 통해 원형 데이터를 가져오는 함수들
	const PokemonSpecies& GetPokemonSpecies(int id) const;
	const MoveData& GetMoveData(int id) const;
	const MoveEffectData& GetMoveEffectData(int id) const;
	const float GetTypeMatchup(Type attackingType, Type defendingType) const;
	const AbilityData& GetAbilityData(int id) const;

private:
	// 외부에서 생성 방지
	DataManager() = default;

	// =================================================================
	// 비공개(private) 로딩 함수 구현
	// =================================================================
	void LoadPokemonSpecies(const std::string& filePath);
	void LoadMoves(const std::string& filePath);
	void LoadMoveEffects(const std::string& filePath);
	void LoadTypeMatchups(const std::string& filePath);
	void LoadAbilities(const std::string& filePath);

	// 로드된 데이터를 저장할 데이터 베이스
	std::map<int, PokemonSpecies> speciesDatabase_;
	std::map<int, MoveData> moveDatabase_;
	std::map<int, MoveEffectData> moveEffectDatabase_;
	std::map<Type, std::map<Type, float>> typeMatchups_;
	std::map<int, AbilityData> abilityDatabase_;
};