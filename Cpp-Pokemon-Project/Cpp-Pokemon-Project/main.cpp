#include <iostream>
#include <vector>
#include "DataManager.h"
#include "Pokemon.h"
#include "BattleManager.h"

int main()
{
    // 1. 모든 게임 데이터를 로드합니다.
    DataManager::GetInstance().LoadAllData();
    std::cout << std::endl;

    // 2. 플레이어 파티 생성 (이상해씨)
    const PokemonSpecies& bulbasaurSpecies = DataManager::GetInstance().GetPokemonSpecies(1);
    PokemonIndividual bulbasaurIndividual;
    bulbasaurIndividual.level = 5;
    // IV/EV 초기화
    bulbasaurIndividual.ivs[Stat::HP] = 31; bulbasaurIndividual.evs[Stat::HP] = 0;
    bulbasaurIndividual.ivs[Stat::ATTACK] = 31; bulbasaurIndividual.evs[Stat::ATTACK] = 0;
    bulbasaurIndividual.ivs[Stat::DEFENSE] = 31; bulbasaurIndividual.evs[Stat::DEFENSE] = 0;
    bulbasaurIndividual.ivs[Stat::SPECIAL_ATTACK] = 31; bulbasaurIndividual.evs[Stat::SPECIAL_ATTACK] = 0;
    bulbasaurIndividual.ivs[Stat::SPECIAL_DEFENSE] = 31; bulbasaurIndividual.evs[Stat::SPECIAL_DEFENSE] = 0;
    bulbasaurIndividual.ivs[Stat::SPEED] = 31; bulbasaurIndividual.evs[Stat::SPEED] = 0;

    Pokemon bulbasaur(bulbasaurSpecies, bulbasaurIndividual);
    bulbasaur.LearnMove(DataManager::GetInstance().GetMoveData(33)); // 몸통박치기
    bulbasaur.LearnMove(DataManager::GetInstance().GetMoveData(45)); // 울음소리
    std::vector<Pokemon> playerParty = { bulbasaur };


    // 3. 야생 포켓몬 파티 생성 (파이리)
    const PokemonSpecies& charmanderSpecies = DataManager::GetInstance().GetPokemonSpecies(4);
    PokemonIndividual charmanderIndividual;
    charmanderIndividual.level = 5;
    // IV/EV 초기화
    charmanderIndividual.ivs = bulbasaurIndividual.ivs;
    charmanderIndividual.evs = bulbasaurIndividual.evs;

    Pokemon charmander(charmanderSpecies, charmanderIndividual);
    charmander.LearnMove(DataManager::GetInstance().GetMoveData(10)); // 할퀴기
    charmander.LearnMove(DataManager::GetInstance().GetMoveData(45)); // 울음소리
    std::vector<Pokemon> wildParty = { charmander };

    // 4. 전투 관리자 생성 및 전투 시작!
    BattleManager battle(playerParty, wildParty);
    battle.Start();

    return 0;
}