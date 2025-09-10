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
    result.push_back(field); // ������ �ʵ� �߰�
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

    // �� ������ �ε� �Լ��� ������� ȣ��
    LoadPokemonSpecies(basePath + "Pokemon_species.csv");
    LoadMoves(basePath + "moves.csv");
    LoadTypeMatchups(basePath + "type_matchups.csv");

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

const float DataManager::GetTypeMatchup(Type attackingType, Type defendingType) const
{
    return typeMatchups_.at(attackingType).at(defendingType);
}

// --- �����(private) �ε� �Լ� ���� ---
void DataManager::LoadPokemonSpecies(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "CRITICAL ERROR: " << filePath << " ������ �� �� �����ϴ�!" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // CSV ������ ù ��° ��(���)�� �ǳʶݴϴ�.

    while (std::getline(file, line))
    {
        // �� ���� ���ʿ��� ����(CR)�� �ִ� ��� ����
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            continue; // �� ���� �ǳʶٱ�
        }

        std::stringstream ss(line);
        std::string field;
        PokemonSpecies species;

        try
        {
            // --- ���� CSV ������ ���� ��� �����͸� �н��ϴ� ---

            // 1. �⺻ ����
            std::getline(ss, field, ','); species.id = std::stoi(field);
            std::getline(ss, field, ','); species.name_en = field;
            std::getline(ss, field, ','); species.name_kr = field;

            // 2. �ɷ�ġ
            std::getline(ss, field, ','); species.baseStats[Stat::HP] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::ATTACK] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::DEFENSE] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::SPECIAL_ATTACK] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::SPECIAL_DEFENSE] = std::stoi(field);
            std::getline(ss, field, ','); species.baseStats[Stat::SPEED] = std::stoi(field);

            // 3. ��ȭ (�� ĭ�� 0���� ó��)
            std::getline(ss, field, ','); species.evolutionLevel = field.empty() ? 0 : std::stoi(field);
            std::getline(ss, field, ','); species.evolutionTargetId = field.empty() ? 0 : std::stoi(field);

            // 4. Ÿ��
            std::getline(ss, field, ','); species.type1 = StringUtils::StringToType(field);
            std::getline(ss, field, ','); species.type2 = StringUtils::StringToType(field);

            // 5. ���� �� �� (stof�� ���ڿ��� float���� ��ȯ)
            std::getline(ss, field, ','); species.male_ratio_pct = field.empty() ? -1.0f : std::stof(field);
            std::getline(ss, field, ','); species.egg_group_1 = field;
            std::getline(ss, field, ','); species.egg_group_2 = field;
            std::getline(ss, field, ','); species.hatch_counter = field.empty() ? 0 : std::stoi(field);

            // 6. Ư��
            std::getline(ss, field, ','); species.ability1 = field;
            std::getline(ss, field, ','); species.ability2 = field;
            std::getline(ss, field, ','); species.hidden_ability = field;

            // 7. ��Ÿ ����
            std::getline(ss, field, ','); species.exp_group = StringUtils::StringToExpGroup(field);
            std::getline(ss, field, ','); species.catch_rate = std::stoi(field);

            // 8. ���� ����
            std::getline(ss, field, ','); species.category_en = field;
            std::getline(ss, field, ','); species.category_kr = field;
            std::getline(ss, field, ','); species.height_m = std::stof(field);
            std::getline(ss, field, ','); species.weight_kg = std::stof(field);
            std::getline(ss, field, ','); species.pokedex_entry_en = field;

            // ������ �÷��� ��ǥ(,)�� �ƴ� ���� ������ �н��ϴ�.
            std::getline(ss, field); species.pokedex_entry_kr = field;

            speciesDatabase_[species.id] = species;
        }
        catch (const std::invalid_argument& e)
        {
            std::cerr << "[PARSE ERROR] ������ ��ȯ �� ���� �߻�: " << line << std::endl;
            std::cerr << "                ���� ����: " << e.what() << std::endl;
        }
    }

    file.close();
    std::cout << "Pokemon species data loaded. (" << speciesDatabase_.size() << " entries)" << std::endl;
}

void DataManager::LoadMoves(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "CRITICAL ERROR: " << filePath << " ������ �� �� �����ϴ�!" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // ��� �ǳʶٱ�

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '\r') continue;

        // [����] ���ο� �ļ� �Լ��� ȣ���Ͽ� �� ���� �ʵ� ���ͷ� ��ȯ
        std::vector<std::string> fields = parseCsvLine(line);

        // �÷� ������ �´��� �ٽ� �ѹ� Ȯ�� (������ġ)
        if (fields.size() < 31) { // ����� ���ǵ� �÷� ��
            std::cerr << "[PARSE WARNING] �÷� ������ ������ ���� �߰�: " << line << std::endl;
            continue;
        }

        MoveData move;
        try {
            // [����] �ε����� ����Ͽ� �� �ʵ忡 ����
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

            // ��� �÷���
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
            std::cerr << "[PARSE ERROR] Move ������ ��ȯ �� ���� �߻�: " << line << std::endl;
            std::cerr << "                ���� ����: " << e.what() << std::endl;
        }
    }

    file.close();
    std::cout << "Move data loaded. (" << moveDatabase_.size() << " entries)" << std::endl;
}

void DataManager::LoadTypeMatchups(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "CRITICAL ERROR: " << filePath << " ������ �� �� �����ϴ�!" << std::endl;
        return;
    }

    // 1. ��� ���� �о� ���(Defending) Ÿ���� ������ �ľ��մϴ�.
    std::string headerLine;
    std::getline(file, headerLine);
    std::stringstream ssHeader(headerLine);
    std::string typeName;

    std::vector<Type> defendingTypes;
    std::getline(ssHeader, typeName, ','); // ù ��° �� ĭ�� ����
    while (std::getline(ssHeader, typeName, ',')) {
        defendingTypes.push_back(StringUtils::StringToType(typeName));
    }

    // 2. ������ ���� �� �پ� �н��ϴ�.
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string attackingTypeName;

        // 2-1. ù ��° ĭ���� ����(Attacking) Ÿ���� �ľ��մϴ�.
        std::getline(ss, attackingTypeName, ',');
        Type attackingType = StringUtils::StringToType(attackingTypeName);

        // 2-2. ������� ���� ��� Ÿ�� ������ ���� �� ������ �����մϴ�.
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