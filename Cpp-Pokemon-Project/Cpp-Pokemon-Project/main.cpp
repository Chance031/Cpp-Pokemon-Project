#include <iostream>      // std::cout, std::endl을 사용하기 위해 필요
#include "DataManager.h"   // DataManager를 사용하기 위해 필요
#include "Pokemon.h"       // Pokemon, PokemonSpecies, PokemonIndividual을 사용하기 위해 필요

// 테스트용 간단한 출력 함수
std::string TriggerToString(AbilityTrigger trigger)
{
    // StringUtils.cpp의 map을 반대로 이용하거나 간단히 switch문으로 구현
    switch (trigger)
    {
    case AbilityTrigger::PASSIVE: return "PASSIVE";
    case AbilityTrigger::ON_SWITCH_IN: return "ON_SWITCH_IN";
    case AbilityTrigger::ON_END_TURN: return "ON_END_TURN";
    case AbilityTrigger::ON_CONTACT: return "ON_CONTACT";
    case AbilityTrigger::ON_HIT_BY_TYPE: return "ON_HIT_BY_TYPE";
    case AbilityTrigger::ON_LOW_HP: return "ON_LOW_HP";
    default: return "UNKNOWN";
    }
}

int main()
{
    // 1. 모든 게임 데이터를 로드합니다.
    DataManager::GetInstance().LoadAllData();
    std::cout << "\n--- 최종 연결 테스트 시작 ---\n" << std::endl;

    // 2. 테스트용 포켓몬을 생성합니다. (1번 이상해씨)
    const PokemonSpecies& bulbasaurSpecies = DataManager::GetInstance().GetPokemonSpecies(1);

    // 포켓몬 개체(Individual) 정보는 간단하게 만듭니다.
    PokemonIndividual bulbasaurIndividual;
    bulbasaurIndividual.level = 5;

    // IV와 EV 맵을 초기화합니다.
    bulbasaurIndividual.ivs[Stat::HP] = 31;
    bulbasaurIndividual.ivs[Stat::ATTACK] = 31;
    bulbasaurIndividual.ivs[Stat::DEFENSE] = 31;
    bulbasaurIndividual.ivs[Stat::SPECIAL_ATTACK] = 31;
    bulbasaurIndividual.ivs[Stat::SPECIAL_DEFENSE] = 31;
    bulbasaurIndividual.ivs[Stat::SPEED] = 31;

    bulbasaurIndividual.evs[Stat::HP] = 0;
    bulbasaurIndividual.evs[Stat::ATTACK] = 0;
    bulbasaurIndividual.evs[Stat::DEFENSE] = 0;
    bulbasaurIndividual.evs[Stat::SPECIAL_ATTACK] = 0;
    bulbasaurIndividual.evs[Stat::SPECIAL_DEFENSE] = 0;
    bulbasaurIndividual.evs[Stat::SPEED] = 0;

    // 3. Species와 Individual 정보로 Pokemon 객체를 생성합니다!
    Pokemon bulbasaur(bulbasaurSpecies, bulbasaurIndividual);

    // 4. 생성된 포켓몬 객체에서 직접 특성 정보를 가져옵니다.
    const AbilityData* ability = bulbasaur.GetActiveAbility();

    // 5. 결과 확인
    if (ability != nullptr && ability->id != 0)
    {
        std::cout << bulbasaur.GetName() << "의 특성 정보 테스트 성공!" << std::endl;
        std::cout << "특성 이름: " << ability->name_kr << std::endl;
        std::cout << "Trigger: " << TriggerToString(ability->trigger) << std::endl;
        std::cout << "ID: " << ability->id << std::endl;
    }
    else
    {
        std::cout << bulbasaur.GetName() << "의 특성 정보 테스트 실패!" << std::endl;
    }

    return 0;
}