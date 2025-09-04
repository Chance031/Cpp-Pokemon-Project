#pragma once

#include <string>
#include <vector>
#include <map>

#include "Pokemon.h" // 포켓몬 데이터 구조를 알아야 함
#include "Move.h"    // 기술 데이터 구조를 알아야 함

// 포켓몬의 모든 '고정' 데이터를 담을 구조체
struct PokemonSpeciesData {
    int id;
    std::string name;
    int hp, attack, defense, sp_attack, sp_defense, speed;
    Type type1, type2;

};

class DataManager {
public:
    // 싱글톤 인스턴스를 가져오는 함수
    static DataManager* GetInstance();

    // 모든 데이터를 로드하는 함수
    void LoadAllData();

    // 데이터를 가져오는 함수
    const PokemonSpeciesData& GetPokemonSpecies(int id) const;
    const Move& GetMove(int id) const;

private:
    // 생성자를 private으로 만들어 외부에서 객체를 생성하지 못하게 함
    DataManager() {}

    // CSV 파일 로딩 함수
    void LoadPokemonData(const std::string& filePath);
    void LoadMoveData(const std::string& filePath);

    // 로드된 데이터를 저장할 변수
    std::map<int, PokemonSpeciesData> pokemonDatabase_;
    std::map<int, Move> moveDatabase_;

    // 싱글톤 인스턴스
    static DataManager* instance_;
};