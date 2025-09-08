#include <iostream>
#include <vector>
#include <string>
#include "DataManager.h"
#include "Enums.h"

// 테스트 출력을 위한 임시 함수
std::string TypeToString(Type type) {
    // ... (이전과 동일한 TypeToString 함수)
    switch (type) {
    case Type::NORMAL: return "노말"; case Type::FIRE: return "불꽃"; case Type::WATER: return "물";
    case Type::GRASS: return "풀"; case Type::ELECTRIC: return "전기"; case Type::ICE: return "얼음";
    case Type::FIGHTING: return "격투"; case Type::POISON: return "독"; case Type::GROUND: return "땅";
    case Type::FLYING: return "비행"; case Type::PSYCHIC: return "에스퍼"; case Type::BUG: return "벌레";
    case Type::ROCK: return "바위"; case Type::GHOST: return "고스트"; case Type::DRAGON: return "드래곤";
    case Type::DARK: return "악"; case Type::STEEL: return "강철"; case Type::FAIRY: return "요정";
    default: return "???";
    }
}

int main()
{
    // --- 1. 정답 데이터 준비 ---
    // Type enum 순서(NORMAL=1 ~ FAIRY=18)에 맞춘 18x18 상성표
    const float correctMatchups[18][18] = {
        //   NOR  FIR  WAT  GRS  ELE  ICE  FIG  POI  GRO  FLY  PSY  BUG  ROC  GHO  DRA  DRK  STE  FAI
            {1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,  0.5,  0,   1,   1,  0.5,  1},  // 공격: NORMAL
            {1,  0.5, 0.5,  2,   1,   2,   1,   1,   1,   1,   1,   2,  0.5,  1,  0.5,  1,   2,   1},  // 공격: FIRE
            {1,   2,  0.5, 0.5,  1,   1,   1,   1,   2,   1,   1,   1,   2,   1,  0.5,  1,   1,   1},  // 공격: WATER
            {1,  0.5,  2,  0.5,  1,   1,   1,  0.5,  2,  0.5,  1,  0.5,  2,   1,  0.5,  1,  0.5,  1},  // 공격: GRASS
            {1,   1,   2,  0.5, 0.5,  1,   1,   1,   0,   2,   1,   1,   1,   1,  0.5,  1,   1,   1},  // 공격: ELECTRIC
            {1,  0.5, 0.5,  2,   1,  0.5,  1,   1,   2,   2,   1,   1,   1,   1,   2,   1,  0.5,  1},  // 공격: ICE
            {2,   1,   1,   1,   1,   2,   1,  0.5,  1,  0.5, 0.5, 0.5,  2,   0,   1,   2,   2,  0.5}, // 공격: FIGHTING
            {1,   1,   1,   2,   1,   1,   1,  0.5, 0.5,  1,   1,   1,  0.5, 0.5,  1,   1,   0,   2},  // 공격: POISON
            {1,   2,   1,  0.5,  2,   1,   1,   2,   1,   0,   1,  0.5,  2,   1,   1,   1,   2,   1},  // 공격: GROUND
            {1,   1,   1,   2,  0.5,  1,   2,   1,   1,   1,   1,   2,  0.5,  1,   1,   1,  0.5,  1},  // 공격: FLYING
            {1,   1,   1,   1,   1,   1,   2,   2,   1,   1,  0.5,  1,   1,   1,   1,   0,  0.5,  1},  // 공격: PSYCHIC
            {1,  0.5,  1,   2,   1,   1,  0.5, 0.5,  1,  0.5,  2,   1,   1,  0.5,  1,   2,  0.5, 0.5}, // 공격: BUG
            {1,   2,   1,   1,   1,   2,  0.5,  1,  0.5,  2,   1,   2,   1,   1,   1,   1,  0.5,  1},  // 공격: ROCK
            {0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   1,   1,   2,   1,  0.5,  1,   1},  // 공격: GHOST
            {1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   1,  0.5,  0},  // 공격: DRAGON
            {1,   1,   1,   1,   1,   1,  0.5,  1,   1,   1,   2,   1,   1,   2,   1,  0.5,  1,  0.5}, // 공격: DARK
            {1,  0.5, 0.5,  1,  0.5,  2,   1,   1,   1,   1,   1,   1,   2,   1,   1,   1,  0.5,  2},  // 공격: STEEL
            {1,  0.5,  1,   1,   1,   1,   2,  0.5,  1,   1,   1,   1,   1,   1,   2,   2,  0.5,  1}   // 공격: FAIRY
    };

    const std::vector<Type> allTypes = {
        Type::NORMAL, Type::FIRE, Type::WATER, Type::GRASS, Type::ELECTRIC, Type::ICE,
        Type::FIGHTING, Type::POISON, Type::GROUND, Type::FLYING, Type::PSYCHIC,
        Type::BUG, Type::ROCK, Type::GHOST, Type::DRAGON, Type::DARK, Type::STEEL, Type::FAIRY
    };

    std::cout << "--- 타입 상성 전체 데이터 검증 시작 ---" << std::endl;
    DataManager::GetInstance().LoadAllData();
    std::cout << std::endl;

    int errorCount = 0;

    try {
        // --- 2. 모든 조합을 반복하며 데이터 비교 ---
        for (Type attackingType : allTypes) {
            for (Type defendingType : allTypes) {
                // Type enum은 NONE=0 부터 시작하므로, 인덱스로 사용하기 위해 -1 해줍니다.
                int atkIndex = static_cast<int>(attackingType) - 1;
                int defIndex = static_cast<int>(defendingType) - 1;

                float expected = correctMatchups[atkIndex][defIndex];
                float actual = DataManager::GetInstance().GetTypeMatchup(attackingType, defendingType);

                if (actual != expected) {
                    errorCount++;
                    std::cout << "[FAIL] 공격: " << TypeToString(attackingType)
                        << ", 방어: " << TypeToString(defendingType)
                        << " | CSV 값: " << actual << ", 정답: " << expected << std::endl;
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[오류] 테스트 중 예외 발생: " << e.what() << std::endl;
        return 1; // 오류 발생 시 종료
    }

    // --- 3. 최종 결과 보고 ---
    std::cout << "\n--- 검증 완료 ---" << std::endl;
    if (errorCount == 0) {
        std::cout << "축하합니다! 324개의 모든 타입 상성이 완벽하게 일치합니다." << std::endl;
    }
    else {
        std::cout << "총 " << errorCount << "개의 오류를 발견했습니다. CSV 파일을 확인해주세요." << std::endl;
    }

    return 0;
}