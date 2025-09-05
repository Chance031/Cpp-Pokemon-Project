#include "DataManager.h"
#include "StringUtils.h"
#include <fstream>
#include <sstream>
#include <iostream>

DataManager* DataManager::instance_ = nullptr;

DataManager* DataManager::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new DataManager();
    }
    return instance_;
}

// 데이터 로딩 총괄
void DataManager::LoadAllData() {
    // 가장 확실한 절대 경로를 사용합니다. (자신의 경로에 맞게 수정)
    std::string basePath = "C:/Users/KGA/Desktop/Cpp-Pokemon-Project/data/";
    LoadAbilities(basePath + "abilities.csv");
    LoadMoves(basePath + "moves.csv");
    LoadPokemonSpecies(basePath + "pokemon_species.csv");
    std::cout << "모든 게임 데이터 로드 완료!" << std::endl;
}

// 포켓몬 데이터 로딩
void DataManager::LoadPokemonSpecies(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: " << filePath << " 파일을 열 수 없습니다." << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // 헤더 건너뛰기

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;
        PokemonSpeciesData data;

        std::getline(ss, field, ','); data.id = std::stoi(field);
        std::getline(ss, field, ','); data.name = field;
        std::getline(ss, field, ','); data.hp = std::stoi(field);
        std::getline(ss, field, ','); data.attack = std::stoi(field);
        std::getline(ss, field, ','); data.defense = std::stoi(field);
        std::getline(ss, field, ','); data.sp_attack = std::stoi(field);
        std::getline(ss, field, ','); data.sp_defense = std::stoi(field);
        std::getline(ss, field, ','); data.speed = std::stoi(field);
        std::getline(ss, field, ','); data.type1 = StringUtils::StringToType(field);
        std::getline(ss, field, ','); data.type2 = StringUtils::StringToType(field);
        std::getline(ss, field, ','); data.ability1_id = std::stoi(field);
        std::getline(ss, field, ','); data.ability2_id = (field.empty() ? 0 : std::stoi(field));
        std::getline(ss, field, ','); data.hidden_ability_id = std::stoi(field);
        std::getline(ss, field, ','); data.exp_group = StringUtils::StringToExpGroup(field);

        pokemonDatabase_[data.id] = data;
    }
    file.close();
}

// 기술/특성 데이터 로딩 (내용 추가)
void DataManager::LoadMoves(const std::string& filePath) { /* ... moves.csv 파싱 로직 ... */ }
void DataManager::LoadAbilities(const std::string& filePath) { /* ... abilities.csv 파싱 로직 ... */ }


// --- 데이터 반환 함수들 ---
const PokemonSpeciesData& DataManager::GetPokemonSpeciesData(int id) const {
    return pokemonDatabase_.at(id);
}
const Move& DataManager::GetMoveData(int id) const {
    return moveDatabase_.at(id);
}
const Ability& DataManager::GetAbilityData(int id) const {
    return abilityDatabase_.at(id);
}