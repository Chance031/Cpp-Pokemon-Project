#include "DataManager.h"

#include <fstream>
#include <sstream>
#include <iostream>

// 싱글톤 인스턴스 초기화
DataManager* DataManager::instance_ = nullptr;

DataManager* DataManager::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new DataManager();
    }
    return instance_;
}

void DataManager::LoadAllData() {
    LoadPokemonData("../data/pokemon_species.csv");
    LoadMoveData("../data/moves.csv");
}

void DataManager::LoadPokemonData(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // 헤더(첫 줄) 건너뛰기

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;

        PokemonSpeciesData data;

        // CSV 각 열을 순서대로 읽어서 구조체에 저장
        std::getline(ss, field, ','); data.id = std::stoi(field);
        std::getline(ss, field, ','); data.name = field;
        std::getline(ss, field, ','); data.hp = std::stoi(field);
        // ... attack, defense 등 나머지 데이터도 순서대로 읽기 ...

        pokemonDatabase_[data.id] = data; // map에 저장
    }

    file.close();
    std::cout << "Pokemon data loaded." << std::endl;
}

// ... GetPokemonSpecies, LoadMoveData 등 나머지 함수 구현 ...