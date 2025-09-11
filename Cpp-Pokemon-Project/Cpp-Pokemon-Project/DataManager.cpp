#include "DataManager.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "StringUtils.h"

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
    LoadMoveEffects(basePath + "move_effects.csv");
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

const MoveEffectData& DataManager::GetMoveEffectData(int id) const
{
    // effect_id가 0 (효과 없음) 이거나 맵에 없는 ID일 경우,
    // 비어있는 기본 MoveEffectData 객체를 반환합니다.
    if (id == 0 || moveEffectDatabase_.find(id) == moveEffectDatabase_.end()) {
        static const MoveEffectData noneEffect; // static으로 선언하여 매번 생성되지 않도록 함
        return noneEffect;
    }
    return moveEffectDatabase_.at(id);
}

const float DataManager::GetTypeMatchup(Type attackingType, Type defendingType) const
{
    return typeMatchups_.at(attackingType).at(defendingType);
}

// --- 비공개(private) 로딩 함수 구현 ---
void DataManager::LoadPokemonSpecies(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) 
    {
        std::cerr << "CRITICAL ERROR: " << filePath << " 파일을 열 수 없습니다!" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // 헤더 건너뛰기

    while (std::getline(file, line)) 
    {
        if (line.empty() || line.front() == '\r') continue;

        std::vector<std::string> fields = StringUtils::parseCsvLine(line);
        if (fields.size() < 28) // 최소 컬럼 수 확인
        { 
            std::cerr << "[PARSE WARNING] Pokemon species 컬럼 개수가 부족한 라인 발견: " << line << std::endl;
            continue;
        }

        PokemonSpecies species;
        try 
        {
            species.id = std::stoi(fields[0]);
            species.name_en = fields[1];
            species.name_kr = fields[2];

            species.baseStats[Stat::HP] = std::stoi(fields[3]);
            species.baseStats[Stat::ATTACK] = std::stoi(fields[4]);
            species.baseStats[Stat::DEFENSE] = std::stoi(fields[5]);
            species.baseStats[Stat::SPECIAL_ATTACK] = std::stoi(fields[6]);
            species.baseStats[Stat::SPECIAL_DEFENSE] = std::stoi(fields[7]);
            species.baseStats[Stat::SPEED] = std::stoi(fields[8]);

            species.evolutionLevel = fields[9].empty() ? 0 : std::stoi(fields[9]);
            species.evolutionTargetId = fields[10].empty() ? 0 : std::stoi(fields[10]);

            species.type1 = StringUtils::StringToType(fields[11]);
            species.type2 = StringUtils::StringToType(fields[12]);

            species.male_ratio_pct = fields[13].empty() ? -1.0f : std::stof(fields[13]);
            species.egg_group_1 = fields[14];
            species.egg_group_2 = fields[15];
            species.hatch_counter = fields[16].empty() ? 0 : std::stoi(fields[16]);

            species.ability1 = fields[17];
            species.ability2 = fields[18];
            species.hidden_ability = fields[19];

            species.exp_group = StringUtils::StringToExpGroup(fields[20]);
            species.catch_rate = std::stoi(fields[21]);

            species.category_en = fields[22];
            species.category_kr = fields[23];
            species.height_m = std::stof(fields[24]);
            species.weight_kg = std::stof(fields[25]);
            species.pokedex_entry_en = fields[26];
            species.pokedex_entry_kr = fields[27];

            speciesDatabase_[species.id] = species;
        }
        catch (const std::exception& e) 
        {
            std::cerr << "[PARSE ERROR] Pokemon species 데이터 변환 중 오류: " << line << " | " << e.what() << std::endl;
        }
    }
    file.close();
    std::cout << "Pokemon species data loaded. (" << speciesDatabase_.size() << " entries)" << std::endl;
}

void DataManager::LoadMoves(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) 
    {
        std::cerr << "CRITICAL ERROR: " << filePath << " 파일을 열 수 없습니다!" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // 헤더 건너뛰기

    while (std::getline(file, line)) 
    {
        if (line.empty() || line.front() == '\r') continue;

        std::vector<std::string> fields = StringUtils::parseCsvLine(line);
        if (fields.size() < 27) // 최소 컬럼 수 확인
        { 
            std::cerr << "[PARSE WARNING] Move 컬럼 개수가 부족한 라인 발견: " << line << std::endl;
            continue;
        }

        MoveData move;
        try 
        {
            move.id = std::stoi(fields[0]);
            move.identifier = fields[1];
            move.name_en = fields[2];
            move.name_kr = fields[3];

            move.type = StringUtils::StringToType(fields[4]);
            move.category = StringUtils::StringToMoveCategory(fields[5]);
            move.power = std::stoi(fields[6]);
            move.accuracy = std::stoi(fields[7]);
            move.pp = std::stoi(fields[8]);
            move.priority = std::stoi(fields[9]);
            move.target = StringUtils::StringToMoveTarget(fields[10]);

            move.effect_id = std::stoi(fields[11]);
            move.effect_chance = std::stoi(fields[12]);

            move.description_en = fields[13];
            move.description_kr = fields[14];

            move.has_high_crit_rate = (std::stoi(fields[15]) == 1);
            move.is_contact = (std::stoi(fields[16]) == 1);
            move.is_protectable = (std::stoi(fields[17]) == 1);
            move.is_reflectable = (std::stoi(fields[18]) == 1);
            move.defrosts_user = (std::stoi(fields[19]) == 1);
            move.is_punch = (std::stoi(fields[20]) == 1);
            move.is_biting = (std::stoi(fields[21]) == 1);
            move.is_slicing = (std::stoi(fields[22]) == 1);
            move.is_sound = (std::stoi(fields[23]) == 1);
            move.is_powder = (std::stoi(fields[24]) == 1);
            move.is_bullet = (std::stoi(fields[25]) == 1);
            move.is_wind = (std::stoi(fields[26]) == 1);
            // CSV 파일에 정의된 나머지 플래그들도 같은 방식으로 추가합니다.

            moveDatabase_[move.id] = move;
        }
        catch (const std::exception& e) 
        {
            std::cerr << "[PARSE ERROR] Move 데이터 변환 중 오류: " << line << " | " << e.what() << std::endl;
        }
    }
    file.close();
    std::cout << "Move data loaded. (" << moveDatabase_.size() << " entries)" << std::endl;
}

// [핵심] move_effects.csv를 읽고 파싱하는 함수
void DataManager::LoadMoveEffects(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) { /* ... 에러 처리 ... */ return; }

    std::string line;
    std::getline(file, line); // 헤더 라인은 건너뛰기

    while (std::getline(file, line))
    {
        auto fields = StringUtils::parseCsvLine(line);
        if (fields.size() < 8) continue;

        MoveEffectData data;
        data.id = std::stoi(fields[0]);
        data.identifier = fields[1];
        data.category = StringUtils::StringToEffectCategory(fields[4]);
        data.target = StringUtils::StringToMoveTarget(fields[7]);

        // 카테고리에 따라 다른 파싱 로직 수행
        switch (data.category)
        {
        case EffectCategory::PRIMARY_STATUS:
            data.primaryStatus = StringUtils::StringToStatusCondition(fields[5]);
            break;
        case EffectCategory::VOLATILE_STATUS:
            data.volatileStatus = StringUtils::StringToVolatileStatus(fields[5]);
            break;
        case EffectCategory::STAT_CHANGE:
        {
            StatChange change;
            change.stat = StringUtils::StringToStat(fields[5]);
            if (!fields[6].empty()) change.stages = std::stoi(fields[6]);
            data.statChanges.push_back(change);
            break;
        }
        // (나중에 다른 카테고리들도 여기에 추가...)
        }
        moveEffectDatabase_[data.id] = data;
    }
    std::cout << "[데이터] 기술 효과 " << moveEffectDatabase_.size() << "개 로드 완료" << std::endl;
}

void DataManager::LoadTypeMatchups(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) 
    {
        std::cerr << "CRITICAL ERROR: " << filePath << " 파일을 열 수 없습니다!" << std::endl;
        return;
    }

    std::string headerLine;
    std::getline(file, headerLine);
    if (headerLine.empty()) return;

    std::stringstream ssHeader(headerLine);
    std::string typeName;
    std::vector<Type> defendingTypes;

    std::getline(ssHeader, typeName, ',');
    while (std::getline(ssHeader, typeName, ',')) 
    {
        if (!typeName.empty() && typeName.back() == '\r') typeName.pop_back();
        defendingTypes.push_back(StringUtils::StringToType(typeName));
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line.front() == '\r') continue;

        std::stringstream ss(line);
        std::string attackingTypeName;

        std::getline(ss, attackingTypeName, ',');
        Type attackingType = StringUtils::StringToType(attackingTypeName);

        std::string multiplierStr;
        for (Type defendingType : defendingTypes) 
        {
            if (!std::getline(ss, multiplierStr, ',')) break;
            float multiplier = std::stof(multiplierStr);
            typeMatchups_[attackingType][defendingType] = multiplier;
        }
    }

    file.close();
    std::cout << "Type matchup data loaded." << std::endl;
}