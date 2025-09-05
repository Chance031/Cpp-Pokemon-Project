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

    // 각 데이터 로딩 함수를 순서대로 호출
    LoadPokemonSpecies(basePath + "Pokemon_species.csv");
    //LoadMoveData(basePath + "moves.csv");

    std::cout << "--- 모든 게임 데이터 로드 완료 ---" << std::endl;
}

const PokemonSpecies& DataManager::GetPokemonSpecies(int id) const
{
    return speciesDatabase_.at(id);
}

const MoveData& DataManager::GetMoveData(int id) const
{
    return moveDatabase_.at(id);
}

// --- 비공개(private) 로딩 함수 구현 ---
void DataManager::LoadPokemonSpecies(const std::string& filePath)
{
    std::ifstream file(filePath); // 파일 열기
    if (!file.is_open())
    {
        std::cerr << "CRITICAL ERROR: " << filePath << " 파일을 열 수 없습니다!" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);

    // 파일 끝까지 한 줄씩 읽기
    while (std::getline(file, line))
    {
        try
        {
            std::stringstream ss(line);
            std::string field;
            PokemonSpecies species;

            // CSV의 각 필드를 순서대로 읽어서 species 객체에 채워넣기
            std::getline(ss, field, ','); species.id = std::stoi(field);
            std::getline(ss, field, ','); species.name = field;
            std::getline(ss, field, ','); species.baseStats[Stat::HP] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::ATTACK] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::DEFENSE] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::SPECIAL_ATTACK] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::SPECIAL_DEFENSE] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::SPEED] = std::stoi(field);
            std::getline(ss, field, ','); species.type1 = StringUtils::StringToType(field);
            std::getline(ss, field, ','); species.type2 = StringUtils::StringToType(field); // type2가 없으면 NONE으로 처리됨

            // 완성된 species 객체를 데이터베이스(맵)에 삽입
            speciesDatabase_[species.id] = species;
        }
        catch (const std::exception& e)
        {
            // 오류 발생 시, 어떤 줄에서 어떤 오류가 났는지 출력
            std::cerr << "[PARSE ERROR] 이 줄에서 오류가 발생했습니다: " << line << std::endl;
            std::cerr << "                오류 내용: " << e.what() << std::endl;
        }
    }

    file.close(); // 파일 닫기
    std::cout << "Pokemon species data loaded. (" << speciesDatabase_.size() << " entries)" << std::endl;
}

void DataManager::LoadMoveData(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "CRITICAL ERROR: " << filePath << " 파일을 열 수 없습니다!" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // 헤더 건너뛰기

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;
        MoveData move;

        // CSV 필드를 순서대로 읽기
        std::getline(ss, field, ','); move.id = std::stoi(field);
        std::getline(ss, field, ','); move.name = field;
        // ... (description, type, category 등 MoveData의 모든 필드를 채우는 로직 추가) ...
        // 예시: std::getline(ss, field, ','); move.type = StringUtils::StringToType(field);

        moveDatabase_[move.id] = move;
    }
    file.close();
    std::cout << "Move data loaded. (" << moveDatabase_.size() << " entries)" << std::endl;
}



