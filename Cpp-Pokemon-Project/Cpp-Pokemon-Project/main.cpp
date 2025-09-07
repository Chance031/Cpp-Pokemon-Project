#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "DataManager.h"
#include "Pokemon.h"
#include "Move.h"

// ... StatToString, PrintAllStats 함수는 이전과 동일 ...

// (테스트용) Stat 열거형을 문자열로 바꿔주는 간단한 함수
std::string StatToString(Stat stat) {
    switch (stat) {
    case Stat::HP:              return "HP";
    case Stat::ATTACK:          return "공격";
    case Stat::DEFENSE:         return "방어";
    case Stat::SPECIAL_ATTACK:  return "특수공격";
    case Stat::SPECIAL_DEFENSE: return "특수방어";
    case Stat::SPEED:           return "스피드";
    default:                    return "???";
    }
}

// (테스트용) 포켓몬의 현재 모든 능력치를 출력하는 함수
void PrintAllStats(const Pokemon& pokemon) {
    std::cout << "--- [" << pokemon.GetName() << " Lv." << pokemon.GetLevel() << "] 현재 능력치 ---" << std::endl;
    for (int i = 0; i <= static_cast<int>(Stat::SPEED); ++i) {
        Stat currentStat = static_cast<Stat>(i);
        std::cout << "  " << StatToString(currentStat) << ": " << pokemon.GetStat(currentStat) << std::endl;
    }
}

int main()
{
    std::cout << "--- 이상한사탕 테스트 시작 ---" << std::endl;

    // 1. 데이터 로드
    DataManager::GetInstance().LoadAllData();
    std::cout << std::endl;

    // 2. 5레벨 이상해씨 준비
    const PokemonSpecies& bulbasaurSpecies = DataManager::GetInstance().GetPokemonSpecies(1);
    PokemonIndividual testIndividual = { 5, Nature::HARDY, /* ... IVs ... */{ {Stat::HP, 31}, {Stat::ATTACK, 31}, {Stat::DEFENSE, 31}, {Stat::SPECIAL_ATTACK, 31}, {Stat::SPECIAL_DEFENSE, 31}, {Stat::SPEED, 31} }, /* ... EVs ... */{ {Stat::HP, 0}, {Stat::ATTACK, 0}, {Stat::DEFENSE, 0}, {Stat::SPECIAL_ATTACK, 0}, {Stat::SPECIAL_DEFENSE, 0}, {Stat::SPEED, 0} }, 0 };
    Pokemon myBulbasaur(bulbasaurSpecies, testIndividual);

    for (int i = 0; i < 10; ++i)
    {
        PrintAllStats(myBulbasaur);
        std::cout << std::endl;

        std::cout << ">>> UseRareCandy() 함수를 호출합니다..." << std::endl;
        myBulbasaur.UseRareCandy();
        std::cout << std::endl;
    }

    PrintAllStats(myBulbasaur);

    std::cout << "\n--- 테스트 종료 ---" << std::endl;
    return 0;
}