#include "DataManager.h"

#include <fstream>
#include <sstream>
#include <iostream>

// �̱��� �ν��Ͻ� �ʱ�ȭ
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
    std::getline(file, line); // ���(ù ��) �ǳʶٱ�

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;

        PokemonSpeciesData data;

        // CSV �� ���� ������� �о ����ü�� ����
        std::getline(ss, field, ','); data.id = std::stoi(field);
        std::getline(ss, field, ','); data.name = field;
        std::getline(ss, field, ','); data.hp = std::stoi(field);
        // ... attack, defense �� ������ �����͵� ������� �б� ...

        pokemonDatabase_[data.id] = data; // map�� ����
    }

    file.close();
    std::cout << "Pokemon data loaded." << std::endl;
}

// ... GetPokemonSpecies, LoadMoveData �� ������ �Լ� ���� ...