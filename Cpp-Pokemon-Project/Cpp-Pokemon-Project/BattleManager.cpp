#include "BattleManager.h"

#include <iostream>
#include <vector>
#include <random>

#include "DataManager.h"
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

    while (!IsBattleOver())
    {
        std::cout << "\n--- 새로운 턴 시작! ---" << std::endl;

        BattleAction playerAction = SelectPlayerAction();
        if (playerAction.type == PlayerActionType::CANCEL) continue;

        BattleAction opponentAction = SelectOpponentAction();

        // 선택 결과 출력 대신 ProcessTurn 함수를 호출합니다.
        ProcessTurn(playerAction, opponentAction);
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

BattleAction BattleManager::SelectPlayerAction()
{
    BattleAction action; // 반환할 action 객체

    while (true)
    {
        ShowMainMenu();
        std::cout << "> ";
        int choice;
        std::cin >> choice;

        switch (choice)
        {
        case 1: // 싸운다
            action.move = SelectMove();
            if (action.move != nullptr) { // 기술을 선택했다면
                action.type = PlayerActionType::FIGHT;
                return action;
            }
            // 뒤로가기를 선택했다면 루프 계속
            continue;

        case 2: // 가방
            std::cout << "\n'가방'은 아직 열 수 없습니다." << std::endl;
            continue;

        case 3: // 포켓몬
            std::cout << "\n'포켓몬'을 교체할 수 없습니다." << std::endl;
            continue;

        case 4: // 도망친다
            std::cout << "\n무사히 도망쳤다!" << std::endl;
            action.type = PlayerActionType::RUN;
            return action;

        default:
            std::cout << "\n잘못된 입력입니다. 1~4 사이의 숫자를 입력해주세요." << std::endl;
            continue;
        }
    }
}

BattleAction BattleManager::SelectOpponentAction()
{
    BattleAction action;
    action.type = PlayerActionType::FIGHT; // AI는 무조건 공격

    const auto& moveset = opponentActivePokemon_->GetMoveset();
    if (moveset.empty()) {
        action.move = nullptr;
        return action;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, moveset.size() - 1);
    int randomIndex = distrib(gen);

    action.move = &moveset[randomIndex];
    return action;
}

const Move* BattleManager::SelectMove()
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

void BattleManager::ProcessTurn(const BattleAction& playerAction, const BattleAction& opponentAction)
{
    Pokemon* firstAttacker = nullptr;
    Pokemon* secondAttacker = nullptr;
    const Move* firstMove = nullptr;
    const Move* secondMove = nullptr;

    // --- 행동 순서 결정 ---
    // 1. 우선도 비교
    int playerPriority = playerAction.move->GetPriority();
    int opponentPriority = opponentAction.move->GetPriority();

    if (playerPriority > opponentPriority)
    {
        firstAttacker = playerActivePokemon_;
        firstMove = playerAction.move;
        secondAttacker = opponentActivePokemon_;
        secondMove = opponentAction.move;
    }
    else if (opponentPriority > playerPriority)
    {
        firstAttacker = opponentActivePokemon_;
        firstMove = opponentAction.move;
        secondAttacker = playerActivePokemon_;
        secondMove = playerAction.move;
    }
    else // 2. 우선도가 같으면 스피드 비교
    {
        int playerScore = playerActivePokemon_->GetStat(Stat::SPEED);
        int opponentScore = opponentActivePokemon_->GetStat(Stat::SPEED);

        if (playerScore >= opponentScore) // 스피드가 같으면 플레이어가 먼저
        {
            firstAttacker = playerActivePokemon_;
            firstMove = playerAction.move;
            secondAttacker = opponentActivePokemon_;
            secondMove = opponentAction.move;
        }
        else
        {
            firstAttacker = opponentActivePokemon_;
            firstMove = opponentAction.move;
            secondAttacker = playerActivePokemon_;
            secondMove = playerAction.move;
        }
    }

    Pokemon* firstTarget = (firstAttacker == playerActivePokemon_) ? opponentActivePokemon_ : playerActivePokemon_;
    Pokemon* secondTarget = (secondAttacker == playerActivePokemon_) ? opponentActivePokemon_ : playerActivePokemon_;

    // --- 첫 번째 포켓몬 행동 실행 ---
    ExecuteMove(firstAttacker, firstTarget, firstMove);

    // 행동 직후 상대가 기절했는지 확인
    if (firstTarget->IsFainted())
    {
        std::cout << firstTarget->GetName() << "은(는) 쓰러졌다!" << std::endl;
        // 상대가 쓰러졌으므로 두 번째 포켓몬은 행동할 필요 없이 턴 종료
        return;
    }

    // --- 두 번째 포켓몬 행동 실행 ---
    ExecuteMove(secondAttacker, secondTarget, secondMove);

    // 행동 직후 상대가 기절했는지 확인
    if (secondTarget->IsFainted())
    {
        std::cout << secondTarget->GetName() << "은(는) 쓰러졌다!" << std::endl;
    }
}

void BattleManager::ExecuteMove(Pokemon* attacker, Pokemon* target, const Move* move)
{
    // const Move*를 Move*로 캐스팅하여 PP를 수정할 수 있게 합니다.
    // 이는 플레이어 파티의 Move 객체를 수정하기 위함입니다.
    // 조금 위험할 수 있지만, 지금 단계에서는 가장 간단한 방법입니다.
    Move* modifiableMove = const_cast<Move*>(move);
    modifiableMove->DecrementPp(); // <<< PP를 1 감소시킵니다.

    std::cout << "\n" << attacker->GetName() << "의 " << move->GetName() << "!" << std::endl;

    // 1. 변화 기술인지 확인 (변화 기술은 데미지 계산을 하지 않음)
    if (move->GetCategory() == MoveCategory::STATUS)
    {
        std::cout << "하지만 아무 일도 일어나지 않았다..." << std::endl;
        // TODO: 변화 기술 효과 처리 로직
        return;
    }

    // 2. 데미지 계산에 필요한 기본 정보 가져오기
    int attackerLevel = attacker->GetLevel();
    int movePower = move->GetPower();
    int attackStat = 0;
    int defenseStat = 0;

    // 3. 기술의 종류(물리/특수)에 따라 사용할 능력치를 결정
    if (move->GetCategory() == MoveCategory::PHYSICAL)
    {
        attackStat = attacker->GetStat(Stat::ATTACK);
        defenseStat = target->GetStat(Stat::DEFENSE);
    }
    else // MoveCategory::SPECIAL
    {
        attackStat = attacker->GetStat(Stat::SPECIAL_ATTACK);
        defenseStat = target->GetStat(Stat::SPECIAL_DEFENSE);
    }

    // 4. 기본 데미지 계산
    int damage = ((((attackerLevel * 2 / 5) + 2) * movePower * attackStat / defenseStat) / 50) + 2;

    // --- 5. 데미지 보정 적용 ---
    double modifier = 1.0;

    // 5-1. 자속 보정 (STAB - Same-Type Attack Bonus)
    // 공격하는 포켓몬의 타입과 사용하는 기술의 타입이 같으면 데미지 1.5배
    if (move->GetType() == attacker->GetType1() || move->GetType() == attacker->GetType2())
    {
        modifier *= 1.5;
    }

    // 5-2. 타입 상성 보정
    double typeMatchup = 1.0;
    Type moveType = move->GetType();
    Type targetType1 = target->GetType1();
    Type targetType2 = target->GetType2();

    // 첫 번째 타입에 대한 상성
    typeMatchup *= DataManager::GetInstance().GetTypeMatchup(moveType, targetType1);
    // 두 번째 타입이 있다면 상성을 마저 곱함
    if (targetType2 != Type::NONE)
    {
        typeMatchup *= DataManager::GetInstance().GetTypeMatchup(moveType, targetType2);
    }

    // 타입 상성 메시지 출력
    if (typeMatchup > 1.0) std::cout << "효과가 굉장했다!" << std::endl;
    if (typeMatchup < 1.0 && typeMatchup > 0) std::cout << "효과가 별로인 듯하다..." << std::endl;
    if (typeMatchup == 0) std::cout << "효과가 없는 것 같다..." << std::endl;

    modifier *= typeMatchup;

    // TODO: 여기에 급소(Critical Hit), 특성, 아이템 등 다른 보정들을 추가

    // 6. 최종 데미지 계산 및 적용
    int finalDamage = static_cast<int>(damage * modifier);
    if (finalDamage < 1) finalDamage = 1; // 최소 1의 데미지는 주도록 보정

    target->TakeDamage(finalDamage);

    // 7. 결과 출력
    std::cout << target->GetName() << "은(는) " << finalDamage << "의 데미지를 받았다!" << std::endl;
    std::cout << target->GetName() << "의 남은 HP: " << target->GetCurrentHP() << "/" << target->GetMaxHP() << std::endl;
}

bool BattleManager::IsBattleOver()
{
    // 플레이어 파티가 전멸했는지 확인
    bool playerAllFainted = true;
    for (const auto& pokemon : playerParty_)
    {
        if (!pokemon.IsFainted()) // 아직 쓰러지지 않은 포켓몬이 있다면
        {
            playerAllFainted = false; // 전멸이 아님
            break;
        }
    }

    // 상대 파티가 전멸했는지 확인
    bool opponentAllFainted = true;
    for (const auto& pokemon : opponentParty_)
    {
        if (!pokemon.IsFainted()) // 아직 쓰러지지 않은 포켓몬이 있다면
        {
            opponentAllFainted = false; // 전멸이 아님
            break;
        }
    }

    // 둘 중 한쪽이라도 전멸했다면 true를 반환하여 전투 종료
    return playerAllFainted || opponentAllFainted;
}