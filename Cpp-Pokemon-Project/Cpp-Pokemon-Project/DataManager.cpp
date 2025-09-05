#include "DataManager.h"

#include "StringUtils.h"

#include <fstream>
#include <sstream>
#include <iostream>

DataManager& DataManager::GetInstance()
{
    static DataManager instance;
    return instance;
}

void DataManager::LoadAllData()
{
    std::string basePath = "../data/";

    // �� ������ �ε� �Լ��� ������� ȣ��
    LoadPokemonSpecies(basePath + "Pokemon_species.csv");
    //LoadMoveData(basePath + "moves.csv");

    std::cout << "--- ��� ���� ������ �ε� �Ϸ� ---" << std::endl;
}

const PokemonSpecies& DataManager::GetPokemonSpecies(int id) const
{
    return speciesDatabase_.at(id);
}

const MoveData& DataManager::GetMoveData(int id) const
{
    return moveDatabase_.at(id);
}

// --- �����(private) �ε� �Լ� ���� ---
void DataManager::LoadPokemonSpecies(const std::string& filePath)
{
    std::ifstream file(filePath); // ���� ����
    if (!file.is_open())
    {
        std::cerr << "CRITICAL ERROR: " << filePath << " ������ �� �� �����ϴ�!" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);

    // ���� ������ �� �پ� �б�
    while (std::getline(file, line))
    {
        try
        {
            std::stringstream ss(line);
            std::string field;
            PokemonSpecies species;

            // CSV�� �� �ʵ带 ������� �о species ��ü�� ä���ֱ�
            std::getline(ss, field, ','); species.id = std::stoi(field);
            std::getline(ss, field, ','); species.name = field;
            std::getline(ss, field, ','); species.baseStats[Stat::HP] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::ATTACK] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::DEFENSE] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::SPECIAL_ATTACK] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::SPECIAL_DEFENSE] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::SPEED] = std::stoi(field);
            std::getline(ss, field, ','); species.type1 = StringUtils::StringToType(field);
            std::getline(ss, field, ','); species.type2 = StringUtils::StringToType(field); // type2�� ������ NONE���� ó����

            // �ϼ��� species ��ü�� �����ͺ��̽�(��)�� ����
            speciesDatabase_[species.id] = species;
        }
        catch (const std::exception& e)
        {
            // ���� �߻� ��, � �ٿ��� � ������ ������ ���
            std::cerr << "[PARSE ERROR] �� �ٿ��� ������ �߻��߽��ϴ�: " << line << std::endl;
            std::cerr << "                ���� ����: " << e.what() << std::endl;
        }
    }

    file.close(); // ���� �ݱ�
    std::cout << "Pokemon species data loaded. (" << speciesDatabase_.size() << " entries)" << std::endl;
}

void DataManager::LoadMoveData(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "CRITICAL ERROR: " << filePath << " ������ �� �� �����ϴ�!" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // ��� �ǳʶٱ�

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;
        MoveData move;

        // CSV �ʵ带 ������� �б�
        std::getline(ss, field, ','); move.id = std::stoi(field);
        std::getline(ss, field, ','); move.name = field;
        // ... (description, type, category �� MoveData�� ��� �ʵ带 ä��� ���� �߰�) ...
        // ����: std::getline(ss, field, ','); move.type = StringUtils::StringToType(field);

        moveDatabase_[move.id] = move;
    }
    file.close();
    std::cout << "Move data loaded. (" << moveDatabase_.size() << " entries)" << std::endl;
}



