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
    bulbasaurIndividual.ivs[Stat::HP] = 31; bulbasaurIndividual.evs[Stat::HP] = 0;
    bulbasaurIndividual.ivs[Stat::ATTACK] = 31; bulbasaurIndividual.evs[Stat::ATTACK] = 0;
    bulbasaurIndividual.ivs[Stat::DEFENSE] = 31; bulbasaurIndividual.evs[Stat::DEFENSE] = 0;
    bulbasaurIndividual.ivs[Stat::SPECIAL_ATTACK] = 31; bulbasaurIndividual.evs[Stat::SPECIAL_ATTACK] = 0;
    bulbasaurIndividual.ivs[Stat::SPECIAL_DEFENSE] = 31; bulbasaurIndividual.evs[Stat::SPECIAL_DEFENSE] = 0;
    bulbasaurIndividual.ivs[Stat::SPEED] = 31; bulbasaurIndividual.evs[Stat::SPEED] = 0;

    Pokemon bulbasaur(bulbasaurSpecies, bulbasaurIndividual);
    bulbasaur.LearnMove(DataManager::GetInstance().GetMoveData(33)); // 몸통박치기
    bulbasaur.LearnMove(DataManager::GetInstance().GetMoveData(45)); // 울음소리

    // =======================================================
    // ▼▼▼▼▼ 이 부분이 가장 중요합니다! ▼▼▼▼▼
    // 'bulbasaur' 객체를 'playerParty' 벡터에 넣고 있습니다.
    std::vector<Pokemon> playerParty = { bulbasaur };
    // =======================================================


    // 3. 야생 포켓몬 파티 생성 (파이리)
    const PokemonSpecies& charmanderSpecies = DataManager::GetInstance().GetPokemonSpecies(4);
    PokemonIndividual charmanderIndividual;
    charmanderIndividual.level = 5;
    charmanderIndividual.ivs[Stat::HP] = 31; charmanderIndividual.evs[Stat::HP] = 0;
    charmanderIndividual.ivs[Stat::ATTACK] = 31; charmanderIndividual.evs[Stat::ATTACK] = 0;
    charmanderIndividual.ivs[Stat::DEFENSE] = 31; charmanderIndividual.evs[Stat::DEFENSE] = 0;
    charmanderIndividual.ivs[Stat::SPECIAL_ATTACK] = 31; charmanderIndividual.evs[Stat::SPECIAL_ATTACK] = 0;
    charmanderIndividual.ivs[Stat::SPECIAL_DEFENSE] = 31; charmanderIndividual.evs[Stat::SPECIAL_DEFENSE] = 0;
    charmanderIndividual.ivs[Stat::SPEED] = 31; charmanderIndividual.evs[Stat::SPEED] = 0;

    Pokemon charmander(charmanderSpecies, charmanderIndividual);
    charmander.LearnMove(DataManager::GetInstance().GetMoveData(10)); // 할퀴기
    charmander.LearnMove(DataManager::GetInstance().GetMoveData(45)); // 울음소리

    // =======================================================
    // ▼▼▼▼▼ 여기도 마찬가지로 중요합니다! ▼▼▼▼▼
    // 'charmander' 객체를 'wildParty' 벡터에 넣고 있습니다.
    std::vector<Pokemon> wildParty = { charmander };
    // =======================================================


    // 4. 전투 관리자 생성 및 전투 시작!
    BattleManager battle(playerParty, wildParty);
    battle.Start();

    return 0;
}