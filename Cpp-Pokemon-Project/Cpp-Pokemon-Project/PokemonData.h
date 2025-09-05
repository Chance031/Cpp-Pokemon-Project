#pragma once

#include <string>
#include <map>

#include "Enums.h"

// 포켓몬 '종족'의 불변 데이터
struct PokemonSpecies
{
	int id;				// 도감 번호
	std::string name;	// 포켓몬 이름
	Type type1;			// 포켓몬 타입1
	Type type2;			// 포켓몬 타입2

	// 종족값 (Base Stats)
	std::map<Stat, int> baseStats;

	// TODO: 특성 목록, 배울 수 있는 기술 목록, 진화 정보 등 추가
};

// 포켓몬 '개체'의 가변 데이터
struct PokemonIndividual
{
	int level;
	Nature nature;
	std::map<Stat, int> ivs;
	std::map<Stat, int> evs;
	int currentExp;

	// TODO: 닉네임, 성별, 현재 상태 이상 등 추가
};