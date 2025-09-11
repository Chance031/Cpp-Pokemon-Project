#include <iostream> // 콘솔 입출력을 위해
#include <string>   // std::string 사용을 위해
#include <vector>   // std::vector 사용을 위해

// 우리가 만든 헤더 파일들
#include "DataManager.h"
#include "PokemonData.h"
#include "MoveData.h"
#include "Enums.h"

int main()
{
    // C++의 입출력 속도를 높여주는 설정 (선택 사항)
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cout << "--- 포켓몬 파이어레드 모작: 데이터 로딩 검증 시스템 ---" << std::endl;

    // 1. 모든 게임 데이터를 로드합니다.
    // 이 한 줄만으로 모든 CSV 파일이 파싱되어 메모리에 올라갑니다.
    try
    {
        DataManager::GetInstance().LoadAllData();
    }
    catch (const std::exception& e)
    {
        std::cerr << "[치명적 오류] 데이터 로딩 중 예외 발생: " << e.what() << std::endl;
        return 1; // 오류가 발생했으므로 프로그램 비정상 종료
    }

    std::cout << "\n--- 모든 데이터 검증 시작 ---\n" << std::endl;

    // 모든 테스트가 성공했는지 추적하는 깃발
    bool all_tests_passed = true;

    // 2. try-catch 블록으로 안전하게 데이터를 검증합니다.
    // 만약 데이터베이스에 없는 ID를 요청하면 프로그램이 죽지 않고 에러를 잡아냅니다.
    try
    {
        // =================================================================
        // 테스트 1: 포켓몬 종족 데이터 (pokemon_species.csv)
        // =================================================================
        std::cout << "--- 1. 포켓몬 종족 데이터 검증 ---" << std::endl;
        const PokemonSpecies& bulbasaur = DataManager::GetInstance().GetPokemonSpecies(1);
        std::cout << "[검증] ID 1번 포켓몬: " << bulbasaur.name_kr;
        if (bulbasaur.name_kr == "이상해씨") {
            std::cout << " (OK)" << std::endl;
        }
        else {
            std::cout << " (FAIL)" << std::endl;
            all_tests_passed = false;
        }

        // =================================================================
        // 테스트 2: 기술 데이터 (moves.csv)
        // =================================================================
        std::cout << "\n--- 2. 기술 데이터 검증 ---" << std::endl;
        const MoveData& tackle = DataManager::GetInstance().GetMoveData(33);
        std::cout << "[검증] ID 33번 기술: " << tackle.name_kr << " | 위력: " << tackle.power;
        if (tackle.name_kr == "몸통박치기" && tackle.power == 40) {
            std::cout << " (OK)" << std::endl;
        }
        else {
            std::cout << " (FAIL)" << std::endl;
            all_tests_passed = false;
        }

        // =================================================================
        // 테스트 3: 타입 상성 데이터 (type_matchups.csv)
        // =================================================================
        std::cout << "\n--- 3. 타입 상성 데이터 검증 ---" << std::endl;
        float fire_vs_grass = DataManager::GetInstance().GetTypeMatchup(Type::FIRE, Type::GRASS);
        std::cout << "[검증] 불꽃 -> 풀 타입 상성 배율: " << fire_vs_grass;
        if (fire_vs_grass == 2.0f) {
            std::cout << " (OK)" << std::endl;
        }
        else {
            std::cout << " (FAIL)" << std::endl;
            all_tests_passed = false;
        }

        // =================================================================
        // 테스트 4: 기술 효과 데이터 (move_effects.csv) - ★핵심 검증★
        // =================================================================
        std::cout << "\n--- 4. 기술 효과 데이터 검증 ---" << std::endl;

        // 검증 4-1: 화상 효과 (ID: 1, Category: PRIMARY_STATUS)
        const MoveEffectData& burnEffect = DataManager::GetInstance().GetMoveEffectData(1);
        std::cout << "[검증] ID 1번 효과 (" << burnEffect.identifier << "): ";
        if (burnEffect.category == EffectCategory::PRIMARY_STATUS && burnEffect.primaryStatus == StatusCondition::BURN) {
            std::cout << "정상적으로 '화상' 효과가 로드되었습니다. (OK)" << std::endl;
        }
        else {
            std::cout << "데이터 파싱 실패! (FAIL)" << std::endl;
            all_tests_passed = false;
        }

        // 검증 4-2: 공격 1랭크 하락 (ID: 101, Category: STAT_CHANGE)
        const MoveEffectData& lowerAtkEffect = DataManager::GetInstance().GetMoveEffectData(101);
        std::cout << "[검증] ID 101번 효과 (" << lowerAtkEffect.identifier << "): ";
        if (lowerAtkEffect.category == EffectCategory::STAT_CHANGE &&
            !lowerAtkEffect.statChanges.empty() &&
            lowerAtkEffect.statChanges[0].stat == Stat::ATTACK &&
            lowerAtkEffect.statChanges[0].stages == -1) {
            std::cout << "정상적으로 '공격 -1' 효과가 로드되었습니다. (OK)" << std::endl;
        }
        else {
            std::cout << "데이터 파싱 실패! (FAIL)" << std::endl;
            all_tests_passed = false;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "\n[검증 오류] 테스트 중 데이터를 찾을 수 없습니다: " << e.what() << std::endl;
        all_tests_passed = false;
    }

    // 3. 최종 결과를 출력합니다.
    std::cout << "\n\n--- 최종 검증 결과 ---" << std::endl;
    if (all_tests_passed) {
        std::cout << "🎉 모든 데이터가 성공적으로 로드되고 검증되었습니다! 축하합니다!" << std::endl;
    }
    else {
        std::cout << "❌ 일부 데이터 검증에 실패했습니다. 위의 (FAIL) 로그를 확인해주세요." << std::endl;
    }

    return 0; // 프로그램 정상 종료
}