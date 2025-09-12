#pragma once

#include <string>
#include <map>
#include <vector>

#include "Enums.h"

// --- 기술폭(Learnset) 관련 구조체 ---
struct LevelUpMove
{
	int level = 0;					// 기술을 배우는 특정 레벨
	int moveId = 0;					// 배울 기술의 ID
};

// --- 레벨업 결과 관련 구조체 ---
struct LevelUpResult
{
	int evolutionId = 0;			// 진화할 포켓몬의 도감 ID
	std::vector<int> newMoveIds;	// 해당 레벨업으로 배우게 된 기술 ID 목록
};

// --- 노력치(EV) 데이터를 담을 구조체 ---
struct EVYield
{
	Stat stat = Stat::HP;
	int amount = 0;
};

// =================================================================
// 포켓몬 '종족'의 불변 데이터 (CSV에서 로드)
// =================================================================
struct PokemonSpecies
{
	// --- 기본 정보 ---
	int id = 0;												// 도감 번호
	std::string name_en = "";								// 포켓몬 이름(en)
	std::string name_kr = "";								// 몬스터 이름(kr)

	// --- 능력치 ---
	std::map<Stat, int> baseStats{};						// H, A, B, C, D, S

	// --- 진화 ---
	int evolutionLevel = 0;									// 진화 레벨
	int evolutionTargetId = 0;								// 진화 타겟

	// --- 타입 ---
	Type type1 = Type::NONE;								// 포켓몬 타입1
	Type type2 = Type::NONE;								// 포켓몬 타입2

	// --- 교배 및 알 ---
	float male_ratio_pct = -1.0f;							// 성비
	std::string egg_group_1 = "NONE";						// 알그룹1
	std::string egg_group_2 = "NONE";						// 알그룹2
	int hatch_counter = 0;									// 부화 카운트

	// --- 특성 ---
	std::string ability1 = "NONE";							// 특성1
	std::string ability2 = "NONE";							// 특성2
	std::string hidden_ability = "NONE";					// 숨겨진 특성

	// --- 성장 관련 데이터 ---
	ExpGroup exp_group = ExpGroup::MEDIUM_FAST;				// 경험치 그룹
	int catch_rate = 0;										// 포획률
	int base_exp_yield = 0;									// 기초 경험치
	std::vector<EVYield> ev_yields{};						// 쓰려뜨렸을 때 주는 노력치 목록
	int base_friendship = 0;								// 기초 친밀도

	// --- 도감 정보 ---
	std::string category_en = "";							// 분류(en)
	std::string category_kr = "";							// 분류(kr)
	float height_m = 0.0f;									// 신장
	float weight_kg =0.0f;									// 체중
	std::string pokedex_entry_en = "";						// 도감 설명(en)
	std::string pokedex_entry_kr = "";						// 도감 설명(kr)

	// --- 기술폭 ---
	std::vector<LevelUpMove> learnset{};					// 레벨업 시 배우는 기술 목록
};

// =================================================================
// 포켓몬 '개체'의 가변 데이터 (게임 내에서 변경)
// =================================================================
struct PokemonIndividual
{
	int level = 1;											// 현재 레벨
	Nature nature = Nature::HARDY;							// 성격
	std::map<Stat, int> ivs{};								// 개체값
	std::map<Stat, int> evs{};								// 노력치
	int currentExp = 0;										// 현재 레벨에서 쌓은 경험치

	std::string nickname = "";								// 닉네임
	Gender gender = Gender::GENDERLESS;						// 성별

	StatusCondition primaryStatus = StatusCondition::NONE;	// 독, 마비 등 전투가 끝나도 지속되는 주요 상태 이상 (하나만 가능)
	std::vector<VolatileStatus> volatileStatuses{};			// 혼란, 씨뿌리기 등 전투 중에만 적용되는 일시적 상태들 (여러 개 가능)
};