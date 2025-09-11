#include <iostream>
#include <string>
#include "DataManager.h"
#include "PokemonData.h"
#include "Enums.h"

int main()
{
    std::cout << "--- 최종 데이터 로딩 검증 ---" << std::endl;

    // 1. 모든 데이터 로드
    DataManager::GetInstance().LoadAllData();
    std::cout << "\n--- 검증 시작 ---\n" << std::endl;

    bool all_tests_ok = true;

    try
    {
        // 테스트 1: 포켓몬 데이터 검증
        const PokemonSpecies& bulbasaur = DataManager::GetInstance().GetPokemonSpecies(1);
        std::cout << "[포켓몬] ID 1번 이름: " << bulbasaur.name_kr << std::endl;
        if (bulbasaur.name_kr != "이상해씨") all_tests_ok = false;

        // [추가] 테스트 2: 기술 데이터 검증 (ID 1 - 막치기)
        const MoveData& pound = DataManager::GetInstance().GetMoveData(1);
        std::cout << "[기술] ID 1번 이름: " << pound.name_kr << " | 위력: " << pound.power << std::endl;
        if (pound.name_kr != "막치기" || pound.power != 40) all_tests_ok = false;

        // 테스트 3: 기술 데이터 검증 (ID 33 - 몸통박치기)
        const MoveData& tackle = DataManager::GetInstance().GetMoveData(33);
        std::cout << "[기술] ID 33번 이름: " << tackle.name_kr << " | 위력: " << tackle.power << std::endl;
        if (tackle.name_kr != "몸통박치기" || tackle.power != 40) all_tests_ok = false;

        // 테스트 4: 타입 상성 데이터 검증
        float fireToGrass = DataManager::GetInstance().GetTypeMatchup(Type::FIRE, Type::GRASS);
        std::cout << "[상성] 불꽃 -> 풀 배율: " << fireToGrass << " (예상: 2)" << std::endl;
        if (fireToGrass != 2.0f) all_tests_ok = false;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[실패] 데이터 조회 중 오류 발생: " << e.what() << std::endl;
        all_tests_ok = false;
    }

    std::cout << "\n--- 검증 종료 ---\n" << std::endl;

    if (all_tests_ok) {
        std::cout << ">> 최종 결과: 모든 데이터가 성공적으로 로드 및 검증되었습니다! <<" << std::endl;
    }
    else {
        std::cout << ">> 최종 결과: 일부 데이터에 문제가 있습니다. <<" << std::endl;
    }

    return 0;
}