#include "BattleManager.h"

#include <iostream>

BattleManager::BattleManager(std::vector<Pokemon>& playerParty, std::vector<Pokemon>& opponentParty)
	: playerParty_(playerParty), opponentParty_(opponentParty)
{

}

void BattleManager::Start()
{
    if (playerParty_.empty() || opponentParty_.empty())
    {
        std::cout << "오류: 포켓몬 파티가 비어있어 전투를 시작할 수 없습니다." << std::endl;
        return;
    }

    playerActivePokemon_ = &playerParty_[0];
    opponentActivePokemon_ = &opponentParty_[0];

    PlayIntroSequence();

    // 메인 배틀 루프 (지금은 한 번만 실행)
    while (true)
    {
        ShowMainMenu(); // <<< 메뉴 출력 함수 호출

        std::cout << "> ";
        int choice;
        std::cin >> choice; // 플레이어의 입력을 받습니다.

        switch (choice)
        {
        case 1: // 싸운다
            std::cout << "\n'싸운다'를 선택했습니다!" << std::endl;
            // TODO: 기술 선택 로직 호출
            break;

        case 2: // 가방
            std::cout << "\n'가방'은 아직 열 수 없습니다." << std::endl;
            continue;

        case 3: // 포켓몬
            std::cout << "\n'포켓몬'을 교체할 수 없습니다." << std::endl;
            continue;

        case 4: // 도망친다
            std::cout << "\n무사히 도망쳤다!" << std::endl;
            break;

        default:
            std::cout << "\n잘못된 입력입니다. 1~4 사이의 숫자를 입력해주세요." << std::endl;
            continue;
        }
        break;
    }

    std::cout << "\n--- 전투 종료! ---" << std::endl;
}

void BattleManager::PlayIntroSequence()
{
    // TODO: 야생 포켓몬인지, 트레이너 대결인지 구분하는 로직 필요
    bool isWildBattle = true; // 지금은 야생 포켓몬 배틀이라고 가정

    if (isWildBattle)
    {
        std::cout << "앗! 야생의 " << opponentActivePokemon_->GetName()
            << "(이)가 나타났다!" << std::endl;
    }
    // else { /* 트레이너 배틀 메시지 */ }

    std::cout << "가라! " << playerActivePokemon_->GetName() << "!" << std::endl;
}

void BattleManager::ShowMainMenu()
{
    std::cout << "\n" << playerActivePokemon_->GetName() << "는(은)" << "\n무엇을 할까?" << std::endl;
    std::cout << "1: 싸운다   2: 가방" << std::endl;
    std::cout << "3: 포켓몬   4: 도망친다" << std::endl;
}