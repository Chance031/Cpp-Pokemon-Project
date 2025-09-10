#include "DataManager.h"

#include "StringUtils.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::vector<std::string> parseCsvLine(const std::string& line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string field;
    bool inQuotes = false;

    char c;
    while (ss.get(c)) {
        if (c == '"') {
            inQuotes = !inQuotes;
        }
        else if (c == ',' && !inQuotes) {
            result.push_back(field);
            field.clear();
        }
        else {
            field += c;
        }
    }
    result.push_back(field); // 마지막 필드 추가
    return result;
}

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
    LoadMoves(basePath + "moves.csv");
    LoadTypeMatchups(basePath + "type_matchups.csv");

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

const float DataManager::GetTypeMatchup(Type attackingType, Type defendingType) const
{
    return typeMatchups_.at(attackingType).at(defendingType);
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

    while (std::getline(file, line))
    {
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

        try
        {
            // --- 최종 CSV 순서에 맞춰 모든 데이터를 읽습니다 ---

            // 1. 기본 정보
            std::getline(ss, field, ','); species.id = std::stoi(field);
            std::getline(ss, field, ','); species.name_en = field;
            std::getline(ss, field, ','); species.name_kr = field;

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
            std::getline(ss, field, ','); species.category_en = field;
            std::getline(ss, field, ','); species.category_kr = field;
            std::getline(ss, field, ','); species.height_m = std::stof(field);
            std::getline(ss, field, ','); species.weight_kg = std::stof(field);
            std::getline(ss, field, ','); species.pokedex_entry_en = field;

            // 마지막 컬럼은 쉼표(,)가 아닌 줄의 끝까지 읽습니다.
            std::getline(ss, field); species.pokedex_entry_kr = field;

            speciesDatabase_[species.id] = species;
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << "[PARSE ERROR] 데이터 변환 중 오류 발생: " << line << std::endl;
            std::cerr << "                오류 내용: " << e.what() << std::endl;
        }
    }

    file.close();
    std::cout << "Pokemon species data loaded. (" << speciesDatabase_.size() << " entries)" << std::endl;
}

void DataManager::LoadMoves(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "CRITICAL ERROR: " << filePath << " 파일을 열 수 없습니다!" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // 헤더 건너뛰기

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '\r') continue;

        // [수정] 새로운 파서 함수를 호출하여 한 줄을 필드 벡터로 변환
        std::vector<std::string> fields = parseCsvLine(line);

        // 컬럼 개수가 맞는지 다시 한번 확인 (안전장치)
        if (fields.size() < 31) { // 헤더에 정의된 컬럼 수
            std::cerr << "[PARSE WARNING] 컬럼 개수가 부족한 라인 발견: " << line << std::endl;
            continue;
        }

        MoveData move;
        try {
            // [수정] 인덱스를 사용하여 각 필드에 접근
            move.id = std::stoi(fields[0]);
            move.identifier = fields[1];
            move.name_kr = fields[2];
            move.name_en = fields[3];

            move.type = StringUtils::StringToType(fields[4]);
            move.category = StringUtils::StringToMoveCategory(fields[5]);
            move.power = std::stoi(fields[6]);
            move.accuracy = std::stoi(fields[7]);
            move.pp = std::stoi(fields[8]);
            move.priority = std::stoi(fields[9]);
            move.target = StringUtils::StringToMoveTarget(fields[10]);

            move.effect_id = std::stoi(fields[11]);
            move.effect_chance = std::stoi(fields[12]);

            move.description_kr = fields[13];
            move.description_en = fields[14];

            // 모든 플래그
            move.has_high_crit_rate = (std::stoi(fields[15]) == 1);
            move.is_contact = (std::stoi(fields[16]) == 1);
            move.is_sound = (std::stoi(fields[17]) == 1);
            move.is_punch = (std::stoi(fields[18]) == 1);
            move.is_biting = (std::stoi(fields[19]) == 1);
            move.is_snatchable = (std::stoi(fields[20]) == 1);
            move.is_slicing = (std::stoi(fields[21]) == 1);
            move.is_bullet = (std::stoi(fields[22]) == 1);
            move.is_wind = (std::stoi(fields[23]) == 1);
            move.is_powder = (std::stoi(fields[24]) == 1);
            move.is_callable_by_metronome = (std::stoi(fields[25]) == 1);
            move.is_affected_by_gravity = (std::stoi(fields[26]) == 1);
            move.defrosts_user = (std::stoi(fields[27]) == 1);
            move.is_reflectable = (std::stoi(fields[28]) == 1);
            move.is_protectable = (std::stoi(fields[29]) == 1);
            move.is_copyable = (std::stoi(fields[30]) == 1);

            moveDatabase_[move.id] = move;
        }
        catch (const std::exception& e) {
            std::cerr << "[PARSE ERROR] Move 데이터 변환 중 오류 발생: " << line << std::endl;
            std::cerr << "                오류 내용: " << e.what() << std::endl;
        }
    }

    file.close();
    std::cout << "Move data loaded. (" << moveDatabase_.size() << " entries)" << std::endl;
}

void DataManager::LoadTypeMatchups(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "CRITICAL ERROR: " << filePath << " 파일을 열 수 없습니다!" << std::endl;
        return;
    }

    // 1. 헤더 줄을 읽어 방어(Defending) 타입의 순서를 파악합니다.
    std::string headerLine;
    std::getline(file, headerLine);
    std::stringstream ssHeader(headerLine);
    std::string typeName;

    std::vector<Type> defendingTypes;
    std::getline(ssHeader, typeName, ','); // 첫 번째 빈 칸은 무시
    while (std::getline(ssHeader, typeName, ',')) {
        defendingTypes.push_back(StringUtils::StringToType(typeName));
    }

    // 2. 데이터 줄을 한 줄씩 읽습니다.
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string attackingTypeName;

        // 2-1. 첫 번째 칸에서 공격(Attacking) 타입을 파악합니다.
        std::getline(ss, attackingTypeName, ',');
        Type attackingType = StringUtils::StringToType(attackingTypeName);

        // 2-2. 헤더에서 읽은 방어 타입 순서에 맞춰 상성 배율을 저장합니다.
        std::string multiplierStr;
        for (Type defendingType : defendingTypes) {
            std::getline(ss, multiplierStr, ',');
            float multiplier = std::stof(multiplierStr);
            typeMatchups_[attackingType][defendingType] = multiplier;
        }
    }

    file.close();
    std::cout << "Type matchup data loaded." << std::endl;
}