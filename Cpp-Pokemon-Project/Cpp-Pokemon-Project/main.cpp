#include <iostream>
#include "DataManager.h"

int main() {
    std::cout << "--- 최소 기능 테스트 시작 ---" << std::endl;

    DataManager::GetInstance()->LoadPokemonNames();

    try {
        std::string pokemonName = DataManager::GetInstance()->GetPokemonName(1);
        std::cout << "\n[성공] ID 1번 포켓몬의 이름은 '" << pokemonName << "' 입니다." << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "\n[실패] ID 1번 데이터를 찾을 수 없습니다." << std::endl;
    }

    std::cout << "\n--- 테스트 종료 ---" << std::endl;
    return 0;
}