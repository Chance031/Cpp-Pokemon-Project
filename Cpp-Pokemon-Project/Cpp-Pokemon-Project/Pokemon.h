#pragma once

#include <vector>

#include "PokemonData.h"

class Move;

class Pokemon
{
public:
	// 생성자
	Pokemon(const PokemonSpecies& species, const PokemonIndividual& individual);

	// --- 공개 인터페이스 (Public API) ---

	// Getter 함수
	
private:
	// --- 비공개 로직 (Internal Logic) ---

	// 1. 불변 데이터 (포켓몬 종족 정보)
	const PokemonSpecies& species_;

	// 2. 가변 데이터 (포켓몬 개체 정보)
	PokemonIndividual individual_;

	// 3. 계산된 현재 상태 (Derived State)
	std::map<Stat, int> finalStats_;	// HP, 공격, 방어 등 최종 능력치
	int currentHp_;
	std::vector<Move> moveset_;	// 기술폭
};