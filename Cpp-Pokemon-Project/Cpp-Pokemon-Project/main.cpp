#include <iostream>
#include <string>
#include <map>
#include "DataManager.h"
#include "PokemonData.h"
#include "Enums.h"
#include "StringUtils.h" // StringTo... 함수들을 사용하기 위해

// --- 테스트를 위한 Enum to String 변환 함수들 ---
// 실제 게임에서는 UI 관리 클래스 등에서 더 체계적으로 관리하는 것이 좋습니다.
std::string TypeToString(Type type) {
    // 이 함수는 StringUtils에 반대 방향의 변환 함수를 만들거나,
    // 간단한 switch-case로 구현할 수 있습니다.
    switch (type) {
    case Type::GRASS: return "GRASS";
    case Type::POISON: return "POISON";
    case Type::FIRE: return "FIRE";
    case Type::WATER: return "WATER";
    case Type::FLYING: return "FLYING";
    case Type::NORMAL: return "NORMAL";
        // ... 기타 타입들 ...
    default: return "NONE";
    }
}

std::string ExpGroupToString(ExpGroup group) {
    switch (group) {
    case ExpGroup::ERRATIC: return "ERRATIC";
    case ExpGroup::FAST: return "FAST";
    case ExpGroup::MEDIUM_FAST: return "MEDIUM_FAST";
    case ExpGroup::MEDIUM_SLOW: return "MEDIUM_SLOW";
    case ExpGroup::SLOW: return "SLOW";
    case ExpGroup::FLUCTUATING: return "FLUCTUATING";
    default: return "UNKNOWN";
    }
}


int main()
{
    std::cout << "--- 전체 데이터 로드 및 출력 테스트 ---" << std::endl;

    // 1. 모든 CSV 데이터 로드
    DataManager::GetInstance().LoadAllData();
    std::cout << "\n=======================================\n" << std::endl;

    try
    {
        // 2. ID 1번 포켓몬 데이터 가져오기
        const PokemonSpecies& data = DataManager::GetInstance().GetPokemonSpecies(1);

        // 3. 가져온 모든 데이터 출력
        std::cout << "### ID 1번 포켓몬 데이터 검증 ###" << std::endl;

        std::cout << "\n--- 기본 정보 ---" << std::endl;
        std::cout << "ID: " << data.id << std::endl;
        std::cout << "한글 이름: " << data.name_kr << std::endl;
        std::cout << "영어 이름: " << data.name_en << std::endl;

        std::cout << "\n--- 능력치 (종족값) ---" << std::endl;
        std::cout << "HP: " << data.baseStats.at(Stat::HP) << std::endl;
        std::cout << "공격: " << data.baseStats.at(Stat::ATTACK) << std::endl;
        std::cout << "방어: " << data.baseStats.at(Stat::DEFENSE) << std::endl;
        std::cout << "특수공격: " << data.baseStats.at(Stat::SPECIAL_ATTACK) << std::endl;
        std::cout << "특수방어: " << data.baseStats.at(Stat::SPECIAL_DEFENSE) << std::endl;
        std::cout << "스피드: " << data.baseStats.at(Stat::SPEED) << std::endl;

        std::cout << "\n--- 진화 정보 ---" << std::endl;
        std::cout << "진화 레벨: " << data.evolutionLevel << std::endl;
        std::cout << "진화 ID: " << data.evolutionTargetId << std::endl;

        std::cout << "\n--- 타입 정보 ---" << std::endl;
        std::cout << "타입1: " << TypeToString(data.type1) << std::endl;
        std::cout << "타입2: " << TypeToString(data.type2) << std::endl;

        std::cout << "\n--- 교배 및 알 정보 ---" << std::endl;
        std::cout << "수컷 비율(%): " << data.male_ratio_pct << std::endl;
        std::cout << "알 그룹 1: " << data.egg_group_1 << std::endl;
        std::cout << "알 그룹 2: " << data.egg_group_2 << std::endl;
        std::cout << "부화 카운트: " << data.hatch_counter << std::endl;

        std::cout << "\n--- 특성 정보 ---" << std::endl;
        std::cout << "특성1: " << data.ability1 << std::endl;
        std::cout << "특성2: " << data.ability2 << std::endl;
        std::cout << "숨겨진 특성: " << data.hidden_ability << std::endl;

        std::cout << "\n--- 기타 정보 ---" << std::endl;
        std::cout << "경험치 그룹: " << ExpGroupToString(data.exp_group) << std::endl;
        std::cout << "포획률: " << data.catch_rate << std::endl;

        std::cout << "\n--- 도감 정보 ---" << std::endl;
        std::cout << "분류(한글): " << data.category_kr << std::endl;
        std::cout << "분류(영어): " << data.category_en << std::endl;
        std::cout << "키(m): " << data.height_m << std::endl;
        std::cout << "몸무게(kg): " << data.weight_kg << std::endl;
        std::cout << "도감 설명(한글): " << data.pokedex_entry_kr << std::endl;
        std::cout << "도감 설명(영어): " << data.pokedex_entry_en << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[실패] 데이터를 가져오는 중 오류 발생: " << e.what() << std::endl;
    }

    std::cout << "\n=======================================\n" << std::endl;
    std::cout << "--- 테스트 종료 ---" << std::endl;
    return 0;
}