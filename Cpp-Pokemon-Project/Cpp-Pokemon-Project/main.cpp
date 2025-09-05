#include <iostream>
#include "DataManager.h"
#include "PokemonData.h" // PokemonSpecies 구조체의 정의를 알기 위해
#include "Enums.h"       // Stat 열거형을 사용하기 위해

// (테스트용) Type 열거형을 문자열로 바꿔주는 간단한 함수
// 실제 게임에서는 StringUtils 같은 곳에 만드는 것이 좋습니다.
std::string TypeToString(Type type) {
    switch (type) {
    case Type::GRASS:   return "GRASS";
    case Type::POISON:  return "POISON";
    case Type::FIRE:    return "FIRE";
    case Type::WATER:   return "WATER";
    case Type::FLYING:  return "FLYING";
    default:            return "NONE";
    }
}

int main()
{
    std::cout << "--- 데이터 출력 테스트 시작 ---" << std::endl;

    // 1. 모든 데이터를 메모리로 로드합니다.
    DataManager::GetInstance().LoadAllData();
    std::cout << std::endl;

    // 2. ID 1번 포켓몬 데이터를 로드 시도합니다.
    try
    {
        // DataManager에서 ID로 PokemonSpecies 데이터를 가져옵니다.
        const PokemonSpecies& bulbasaurData = DataManager::GetInstance().GetPokemonSpecies(1);

        // 3. 성공 시, 가져온 데이터를 출력합니다.
        std::cout << "[성공] ID " << bulbasaurData.id << "번 포켓몬 데이터 로드 완료!" << std::endl;
        std::cout << "  이름: " << bulbasaurData.name << std::endl;
        std::cout << "  타입1: " << TypeToString(bulbasaurData.type1) << std::endl;
        std::cout << "  타입2: " << TypeToString(bulbasaurData.type2) << std::endl;
        std::cout << "  HP 종족값: " << bulbasaurData.baseStats.at(Stat::HP) << std::endl;
        std::cout << "  공격 종족값: " << bulbasaurData.baseStats.at(Stat::ATTACK) << std::endl;
    }
    catch (const std::out_of_range& e)
    {
        // 3. 실패 시 (해당 ID의 데이터가 없을 경우), 오류 메시지를 출력합니다.
        std::cerr << "[실패] ID 1번 포켓몬 데이터를 찾을 수 없습니다." << std::endl;
        std::cerr << "       오류 내용: " << e.what() << std::endl;
    }

    std::cout << "\n--- 테스트 종료 ---" << std::endl;
    return 0;
}