#include "DataManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

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
    LoadAbilities(basePath + "abilities.csv");

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
    // effect_id가 0 (효과 없음) 이거나 맵에 없는 ID일 경우, 비어있는 기본 MoveEffectData 객체를 반환
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

const AbilityData& DataManager::GetAbilityData(int id) const
{
    if (abilityDatabase_.find(id) == abilityDatabase_.end()) {
        // ID를 찾지 못한 경우, 비어있는 static 객체를 반환하여 안전하게 처리
        static const AbilityData noneAbility;
        return noneAbility;
    }
    return abilityDatabase_.at(id);
}

// =================================================================
// 비공개(private) 로딩 함수 구현
// =================================================================

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

            species.ability1 = fields[17].empty() ? 0 : std::stoi(fields[17]);
            species.ability2 = fields[18].empty() ? 0 : std::stoi(fields[18]);
            species.hidden_ability = fields[19].empty() ? 0 : std::stoi(fields[19]);

            species.exp_group = StringUtils::StringToExpGroup(fields[20]);
            species.catch_rate = std::stoi(fields[21]);

            species.base_exp_yield = fields[22].empty() ? 0 : std::stoi(fields[22]);
            for (int i = 0; i < 3; ++i)
            {
                // ev_yield_stat_1 은 23번, stat_2는 25번, stat_3는 27번 필드에 해당
                int stat_index = 23 + (i * 2);
                // ev_yield_amount_1 은 24번, amount_2는 26번, amount_3는 28번 필드에 해당
                int amount_index = 24 + (i * 2);

                // stat 이름이 "NONE"이 아니고 비어있지 않다면, 유효한 노력치 데이터로 간주
                if (!fields[stat_index].empty() && fields[stat_index] != "NONE")
                {
                    EVYield yield;
                    yield.stat = StringUtils::StringToStat(fields[stat_index]); // 문자열을 Stat enum으로 변환
                    yield.amount = std::stoi(fields[amount_index]);             // 문자열을 정수로 변환

                    // 완성된 EVYield 구조체를 species의 ev_yields 벡터에 추가
                    species.ev_yields.push_back(yield);
                }
            }

            species.base_friendship = fields[29].empty() ? 0 : std::stoi(fields[29]);

            species.category_en = fields[30];
            species.category_kr = fields[31];
            species.height_m = std::stof(fields[32]);
            species.weight_kg = std::stof(fields[33]);
            species.pokedex_entry_en = fields[34];
            species.pokedex_entry_kr = fields[35];

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

void DataManager::LoadMoveEffects(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "CRITICAL ERROR: " << filePath << " 파일을 열 수 없습니다!" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // 헤더 라인은 건너뛰기

    while (std::getline(file, line))
    {
        if (line.empty() || line.front() == '\r') continue;

        auto fields = StringUtils::parseCsvLine(line);
        if (fields.size() < 8) continue;

        try // <<< 1. try-catch 블록 추가
        {
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
        catch (const std::exception& e) // <<< 2. 오류 처리 부분
        {
            std::cerr << "[PARSE ERROR] MoveEffect 데이터 변환 중 오류: " << line << " | " << e.what() << std::endl;
        }
    }

    file.close(); // <<< 3. 파일 닫기 추가
    std::cout << "Move effect data load. (" << moveEffectDatabase_.size() << " entries)" << std::endl;
}

void DataManager::LoadTypeMatchups(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "CRITICAL ERROR: " << filePath << " 파일을 열 수 없습니다!" << std::endl;
        return;
    }

    std::string headerLine;
    std::getline(file, headerLine);
    if (headerLine.empty()) return;

    std::vector<Type> defendingTypes;
    try { // 헤더 파싱 중 오류가 발생할 수 있으므로 try-catch 추가
        std::stringstream ssHeader(headerLine);
        std::string typeName;
        std::getline(ssHeader, typeName, ','); // 첫 번째 빈 컬럼 무시
        while (std::getline(ssHeader, typeName, ',')) {
            if (!typeName.empty() && typeName.back() == '\r') typeName.pop_back();
            defendingTypes.push_back(StringUtils::StringToType(typeName));
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[PARSE ERROR] Type Matchup 헤더 처리 중 오류: " << e.what() << std::endl;
        return; // 헤더에 문제가 있으면 더 이상 진행할 수 없으므로 함수 종료
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line.front() == '\r') continue;

        try { // <<< 데이터 한 줄마다 try-catch로 감싸기
            std::stringstream ss(line);
            std::string attackingTypeName;
            std::getline(ss, attackingTypeName, ',');
            Type attackingType = StringUtils::StringToType(attackingTypeName);

            std::string multiplierStr;
            for (Type defendingType : defendingTypes) {
                if (!std::getline(ss, multiplierStr, ',')) break;
                float multiplier = std::stof(multiplierStr);
                typeMatchups_[attackingType][defendingType] = multiplier;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "[PARSE ERROR] Type Matchup 데이터 변환 중 오류: " << line << " | " << e.what() << std::endl;
        }
    }

    file.close();
    std::cout << "Type matchup data loaded. (" << typeMatchups_.size() << " entries)" << std::endl;
}

void DataManager::LoadAbilities(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "CRITICAL ERROR: " << filePath << " 파일을 열 수 없습니다!" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // 헤더 건너뛰기

    while (std::getline(file, line)) {
        if (line.empty() || line.front() == '\r') continue;

        std::vector<std::string> fields = StringUtils::parseCsvLine(line);
        if (fields.size() < 12) {
            std::cerr << "[PARSE WARNING] Ability 컬럼 개수가 부족한 라인 발견: " << line << std::endl;
            continue;
        }

        AbilityData ability;
        try {
            ability.id = std::stoi(fields[0]);
            ability.name_en = fields[1];
            ability.name_kr = fields[2];
            ability.trigger = StringUtils::StringToAbilityTrigger(fields[3]);
            ability.category = StringUtils::StringToAbilityCategory(fields[4]);
            ability.param_type = fields[5];
            ability.param_value = fields[6];
            ability.secondary_param_type = fields[7];
            ability.secondary_param_value = fields[8];
            ability.target = StringUtils::StringToMoveTarget(fields[9]);
            ability.description_en = fields[10];
            ability.description_kr = fields[11];

            abilityDatabase_[ability.id] = ability;
        }
        catch (const std::exception& e) {
            std::cerr << "[PARSE ERROR] Ability 데이터 변환 중 오류: " << line << " | " << e.what() << std::endl;
        }
    }
    file.close();
    std::cout << "Ability data loaded. (" << abilityDatabase_.size() << " entries)" << std::endl;
}