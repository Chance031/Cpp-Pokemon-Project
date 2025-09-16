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
    bulbasaur.LearnMove(DataManager::GetInstance().GetMoveData(77)); // 독가루 추가!

    const PokemonSpecies& pidgeySpecies = DataManager::GetInstance().GetPokemonSpecies(16); // 16: 구구
    PokemonIndividual pidgeyIndividual;
    pidgeyIndividual.level = 5;
    pidgeyIndividual.ivs[Stat::HP] = 31; pidgeyIndividual.evs[Stat::HP] = 0;
    pidgeyIndividual.ivs[Stat::ATTACK] = 31; pidgeyIndividual.evs[Stat::ATTACK] = 0;
    pidgeyIndividual.ivs[Stat::DEFENSE] = 31; pidgeyIndividual.evs[Stat::DEFENSE] = 0;
    pidgeyIndividual.ivs[Stat::SPECIAL_ATTACK] = 31; pidgeyIndividual.evs[Stat::SPECIAL_ATTACK] = 0;
    pidgeyIndividual.ivs[Stat::SPECIAL_DEFENSE] = 31; pidgeyIndividual.evs[Stat::SPECIAL_DEFENSE] = 0;
    pidgeyIndividual.ivs[Stat::SPEED] = 31; pidgeyIndividual.evs[Stat::SPEED] = 0;

    Pokemon pidgey(pidgeySpecies, pidgeyIndividual);
    pidgey.LearnMove(DataManager::GetInstance().GetMoveData(33)); // 몸통박치기
    pidgey.LearnMove(DataManager::GetInstance().GetMoveData(18)); // 회오리

    std::vector<Pokemon> playerParty = { bulbasaur, pidgey }; // 이상해씨와 구구를 모두 파티에 포함

    // 3. 야생 포켓몬 파티 생성 (아보크로 변경)
    const PokemonSpecies& arbokSpecies = DataManager::GetInstance().GetPokemonSpecies(24); // 24: 아보크
    PokemonIndividual arbokIndividual;
    arbokIndividual.level = 5;
    // IV/EV 초기화
    arbokIndividual.ivs = bulbasaurIndividual.ivs;
    arbokIndividual.evs = bulbasaurIndividual.evs;

    Pokemon arbok(arbokSpecies, arbokIndividual);
    arbok.LearnMove(DataManager::GetInstance().GetMoveData(33)); // 몸통박치기
    std::vector<Pokemon> wildParty = { arbok };

    // 4. 전투 관리자 생성 및 전투 시작!
    BattleManager battle(playerParty, wildParty);
    battle.Start();

    return 0;
}