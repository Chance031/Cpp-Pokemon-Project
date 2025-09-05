#pragma once

#include <string>
#include <vector>
#include <map>
#include "Enums.h"
#include "Move.h"
#include "Ability.h"

// CSV �������� ����ü ����
struct PokemonSpeciesData {
    int id;
    std::string name;
    int hp, attack, defense, sp_attack, sp_defense, speed;
    Type type1, type2;
    int ability1_id, ability2_id, hidden_ability_id;
    ExpGroup exp_group;
};

class DataManager {
public:
    static DataManager* GetInstance();

    // ��� CSV ������ �о� �����͸� �޸𸮿� �ε�
    void LoadAllData();

    // �ε�� �����͸� �ܺο��� ������ �� �ְ� �ϴ� �Լ�
    const PokemonSpeciesData& GetPokemonSpeciesData(int id) const;
    const Move& GetMoveData(int id) const;
    const Ability& GetAbilityData(int id) const;

private:
    DataManager() {}

    // �� CSV ������ ������ �д� ����� �Լ�
    void LoadPokemonSpecies(const std::string& filePath);
    void LoadMoves(const std::string& filePath);
    void LoadAbilities(const std::string& filePath);

    // �ε�� �����͸� ������ '�����ͺ��̽�' ������
    std::map<int, PokemonSpeciesData> pokemonDatabase_;
    std::map<int, Move> moveDatabase_;
    std::map<int, Ability> abilityDatabase_;

    static DataManager* instance_;
};