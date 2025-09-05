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

// ������ �ε� �Ѱ�
void DataManager::LoadAllData() {
    // ���� Ȯ���� ���� ��θ� ����մϴ�. (�ڽ��� ��ο� �°� ����)
    std::string basePath = "C:/Users/KGA/Desktop/Cpp-Pokemon-Project/data/";
    LoadAbilities(basePath + "abilities.csv");
    LoadMoves(basePath + "moves.csv");
    LoadPokemonSpecies(basePath + "pokemon_species.csv");
    std::cout << "��� ���� ������ �ε� �Ϸ�!" << std::endl;
}

// ���ϸ� ������ �ε�
void DataManager::LoadPokemonSpecies(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: " << filePath << " ������ �� �� �����ϴ�." << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // ��� �ǳʶٱ�

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

// ���/Ư�� ������ �ε� (���� �߰�)
void DataManager::LoadMoves(const std::string& filePath) { /* ... moves.csv �Ľ� ���� ... */ }
void DataManager::LoadAbilities(const std::string& filePath) { /* ... abilities.csv �Ľ� ���� ... */ }


// --- ������ ��ȯ �Լ��� ---
const PokemonSpeciesData& DataManager::GetPokemonSpeciesData(int id) const {
    return pokemonDatabase_.at(id);
}
const Move& DataManager::GetMoveData(int id) const {
    return moveDatabase_.at(id);
}
const Ability& DataManager::GetAbilityData(int id) const {
    return abilityDatabase_.at(id);
}