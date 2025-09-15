#include "BattleManager.h"

#include <iostream>

#include "Move.h"

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
        {
            Move* selectedMove = SelectMove(); // <<< SelectMove 함수 호출
            if (selectedMove != nullptr) // 기술을 선택했다면 (뒤로가기 X)
            {
                std::cout << "\n" << playerActivePokemon_->GetName() << "은(는) "
                    << selectedMove->GetName() << "을(를) 사용했다!" << std::endl;
                // TODO: 기술 사용 로직 (PP 감소 등)
                break; // 루프 종료
            }
            // 뒤로가기를 선택했다면 아무것도 하지 않고 다시 메인 메뉴로
            continue;
        }

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

Move* BattleManager::SelectMove()
{
    const auto& moveset = playerActivePokemon_->GetMoveset();

    if (moveset.empty())
    {
        std::cout << "\n사용할 수 있는 기술이 없습니다!" << std::endl;
        return nullptr;
    }

    while (true)
    {
        std::cout << "어떤 기술을 사용할까?" << std::endl;

        // 기술 목록을 번호와 함께 출력
        for (int i = 0; i < moveset.size(); ++i)
        {
            std::cout << i + 1 << ": " << moveset[i].GetName() << " (PP: " << moveset[i].GetCurrentPp() << "/" << moveset[i].GetMaxPp() << ")" << std::endl;
        }
        std::cout << moveset.size() + 1 << ": 뒤로가기" << std::endl;
        std::cout << "> ";

        int choice;
        std::cin >> choice;

        if (choice > 0 && choice <= moveset.size())
        {
            // 기술을 올바르게 선택한 경우, 해당 기술의 주소(포인터)를 반환
             // playerParty_는 std::vector<Pokemon>이므로, 포인터가 아닌 실제 객체입니다.
             // 따라서 GetMoveset()으로 얻은 벡터의 요소 주소를 안전하게 반환할 수 있습니다.
            return &playerParty_[0].GetMovesetForModify()[choice - 1];
        }
        else if (choice == moveset.size() + 1)
        {
            return nullptr; // '뒤로가기'를 선택하면 null을 반환
        }
        else
        {
            std::cout << "\n잘못된 입력입니다." << std::endl;
        }
    }
}
