#include <iostream>
#include <vector>
#include "DataManager.h"
#include "Pokemon.h"
#include "BattleManager.h" // <<< BattleManager.h 포함

int main()
{
    // 1. 데이터 로드
    DataManager::GetInstance().LoadAllData();
    std::cout << std::endl;

    // 2. 플레이어 파티 생성 (이상해씨)
    const PokemonSpecies& bulbasaurSpecies = DataManager::GetInstance().GetPokemonSpecies(1);
    PokemonIndividual bulbasaurIndividual;
    bulbasaurIndividual.level = 5;
    // ... (IV/EV 초기화 코드) ...
    bulbasaurIndividual.ivs[Stat::HP] = 31; bulbasaurIndividual.evs[Stat::HP] = 0;
    bulbasaurIndividual.ivs[Stat::ATTACK] = 31; bulbasaurIndividual.evs[Stat::ATTACK] = 0;
    bulbasaurIndividual.ivs[Stat::DEFENSE] = 31; bulbasaurIndividual.evs[Stat::DEFENSE] = 0;
    bulbasaurIndividual.ivs[Stat::SPECIAL_ATTACK] = 31; bulbasaurIndividual.evs[Stat::SPECIAL_ATTACK] = 0;
    bulbasaurIndividual.ivs[Stat::SPECIAL_DEFENSE] = 31; bulbasaurIndividual.evs[Stat::SPECIAL_DEFENSE] = 0;
    bulbasaurIndividual.ivs[Stat::SPEED] = 31; bulbasaurIndividual.evs[Stat::SPEED] = 0;

    Pokemon bulbasaur(bulbasaurSpecies, bulbasaurIndividual);
    std::vector<Pokemon> playerParty = { bulbasaur };

    // 3. 야생 포켓몬 파티 생성 (파이리)
    const PokemonSpecies& charmanderSpecies = DataManager::GetInstance().GetPokemonSpecies(4);
    PokemonIndividual charmanderIndividual;
    charmanderIndividual.level = 5;
    // ... (IV/EV 초기화 코드) ...
    charmanderIndividual.ivs[Stat::HP] = 31; charmanderIndividual.evs[Stat::HP] = 0;
    charmanderIndividual.ivs[Stat::ATTACK] = 31; charmanderIndividual.evs[Stat::ATTACK] = 0;
    charmanderIndividual.ivs[Stat::DEFENSE] = 31; charmanderIndividual.evs[Stat::DEFENSE] = 0;
    charmanderIndividual.ivs[Stat::SPECIAL_ATTACK] = 31; charmanderIndividual.evs[Stat::SPECIAL_ATTACK] = 0;
    charmanderIndividual.ivs[Stat::SPECIAL_DEFENSE] = 31; charmanderIndividual.evs[Stat::SPECIAL_DEFENSE] = 0;
    charmanderIndividual.ivs[Stat::SPEED] = 31; charmanderIndividual.evs[Stat::SPEED] = 0;

    Pokemon charmander(charmanderSpecies, charmanderIndividual);
    std::vector<Pokemon> wildParty = { charmander };

    // 4. 전투 관리자 생성 및 전투 시작!
    BattleManager battle(playerParty, wildParty);
    battle.Start();

    return 0;
}