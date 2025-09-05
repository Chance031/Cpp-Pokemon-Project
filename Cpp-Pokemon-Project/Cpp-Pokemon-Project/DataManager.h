#pragma once

#include <string>
#include <vector>
#include <map>
#include "Enums.h"
#include "Move.h"
#include "Ability.h"

// CSV 데이터의 구조체 정의
struct PokemonSpeciesData {
    int id;
    std::string name;
    int hp, attack, defense, sp_attack, sp_defense, speed;
    Type type1, type2;
    int ability1_id, ability2_id, hidden_ability_id;
    ExpGroup exp_group;
};

class DataManager {
public:
    static DataManager* GetInstance();

    // 모든 CSV 파일을 읽어 데이터를 메모리에 로드
    void LoadAllData();

    // 로드된 데이터를 외부에서 가져갈 수 있게 하는 함수
    const PokemonSpeciesData& GetPokemonSpeciesData(int id) const;
    const Move& GetMoveData(int id) const;
    const Ability& GetAbilityData(int id) const;

private:
    DataManager() {}

    // 각 CSV 파일을 실제로 읽는 비공개 함수
    void LoadPokemonSpecies(const std::string& filePath);
    void LoadMoves(const std::string& filePath);
    void LoadAbilities(const std::string& filePath);

    // 로드된 데이터를 저장할 '데이터베이스' 변수들
    std::map<int, PokemonSpeciesData> pokemonDatabase_;
    std::map<int, Move> moveDatabase_;
    std::map<int, Ability> abilityDatabase_;

    static DataManager* instance_;
};