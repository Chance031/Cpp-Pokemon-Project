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
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "CRITICAL ERROR: " << filePath << " 파일을 열 수 없습니다!" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // CSV 파일의 첫 번째 줄(헤더)은 건너뜁니다.

    while (std::getline(file, line)) {
        // 행 끝에 불필요한 공백(CR)이 있는 경우 제거
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            continue; // 빈 줄은 건너뛰기
        }

        std::stringstream ss(line);
        std::string field;
        PokemonSpecies species;

        try {
            // --- 최종 CSV 순서에 맞춰 모든 데이터를 읽습니다 ---

            // 1. 기본 정보
            std::getline(ss, field, ','); species.id = std::stoi(field);
            std::getline(ss, field, ','); species.name_kr = field;
            std::getline(ss, field, ','); species.name_en = field;

            // 2. 능력치
            std::getline(ss, field, ','); species.baseStats[Stat::HP] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::ATTACK] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::DEFENSE] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::SPECIAL_ATTACK] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::SPECIAL_DEFENSE] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::SPEED] = std::stoi(field);

            // 3. 진화 (빈 칸은 0으로 처리)
            std::getline(ss, field, ','); species.evolutionLevel = field.empty() ? 0 : std::stoi(field);
            std::getline(ss, field, ','); species.evolutionTargetId = field.empty() ? 0 : std::stoi(field);

            // 4. 타입
            std::getline(ss, field, ','); species.type1 = StringUtils::StringToType(field);
            std::getline(ss, field, ','); species.type2 = StringUtils::StringToType(field);

            // 5. 교배 및 알 (stof는 문자열을 float으로 변환)
            std::getline(ss, field, ','); species.male_ratio_pct = field.empty() ? -1.0f : std::stof(field);
            std::getline(ss, field, ','); species.egg_group_1 = field;
            std::getline(ss, field, ','); species.egg_group_2 = field;
            std::getline(ss, field, ','); species.hatch_counter = field.empty() ? 0 : std::stoi(field);

            // 6. 특성
            std::getline(ss, field, ','); species.ability1 = field;
            std::getline(ss, field, ','); species.ability2 = field;
            std::getline(ss, field, ','); species.hidden_ability = field;

            // 7. 기타 정보
            std::getline(ss, field, ','); species.exp_group = StringUtils::StringToExpGroup(field);
            std::getline(ss, field, ','); species.catch_rate = std::stoi(field);

            // 8. 도감 정보
            std::getline(ss, field, ','); species.category_kr = field;
            std::getline(ss, field, ','); species.category_en = field;
            std::getline(ss, field, ','); species.height_m = std::stof(field);
            std::getline(ss, field, ','); species.weight_kg = std::stof(field);
            std::getline(ss, field, ','); species.pokedex_entry_kr = field;

            // 마지막 컬럼은 쉼표(,)가 아닌 줄의 끝까지 읽습니다.
            std::getline(ss, field); species.pokedex_entry_en = field;

            speciesDatabase_[species.id] = species;
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "[PARSE ERROR] 데이터 변환 중 오류 발생: " << line << std::endl;
            std::cerr << "                오류 내용: " << e.what() << std::endl;
        }
    }

    file.close();
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



