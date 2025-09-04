#pragma once

#include <string>
#include <vector>
#include <map>

#include "Pokemon.h" // ���ϸ� ������ ������ �˾ƾ� ��
#include "Move.h"    // ��� ������ ������ �˾ƾ� ��

// ���ϸ��� ��� '����' �����͸� ���� ����ü
struct PokemonSpeciesData {
    int id;
    std::string name;
    int hp, attack, defense, sp_attack, sp_defense, speed;
    Type type1, type2;

};

class DataManager {
public:
    // �̱��� �ν��Ͻ��� �������� �Լ�
    static DataManager* GetInstance();

    // ��� �����͸� �ε��ϴ� �Լ�
    void LoadAllData();

    // �����͸� �������� �Լ�
    const PokemonSpeciesData& GetPokemonSpecies(int id) const;
    const Move& GetMove(int id) const;

private:
    // �����ڸ� private���� ����� �ܺο��� ��ü�� �������� ���ϰ� ��
    DataManager() {}

    // CSV ���� �ε� �Լ�
    void LoadPokemonData(const std::string& filePath);
    void LoadMoveData(const std::string& filePath);

    // �ε�� �����͸� ������ ����
    std::map<int, PokemonSpeciesData> pokemonDatabase_;
    std::map<int, Move> moveDatabase_;

    // �̱��� �ν��Ͻ�
    static DataManager* instance_;
};