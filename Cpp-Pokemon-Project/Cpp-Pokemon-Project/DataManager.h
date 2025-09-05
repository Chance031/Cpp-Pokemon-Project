#pragma once

#include <map>

#include "PokemonData.h"
#include "MoveData.h"

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

private:
	// 비공개(private) 함수 '선언'
	void LoadPokemonSpecies(const std::string& filePath);
	void LoadMoveData(const std::string& filePath);

	// 외부에서 생성 방지
	DataManager() = default;

	// 로드된 데이터를 저장할 데이터 베이스
	std::map<int, PokemonSpecies> speciesDatabase_;
	std::map<int, MoveData> moveDatabase_;
};