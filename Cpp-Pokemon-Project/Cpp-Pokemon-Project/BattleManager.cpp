#include "BattleManager.h"

#include <iostream>

#include "DataManager.h"
#include "Move.h"

// 생성자
BattleManager::BattleManager(std::vector<Pokemon>& playerParty, std::vector<Pokemon>& opponentParty)
	: playerParty_(playerParty), opponentParty_(opponentParty)
{
    // 예측 불가능한 시드로 난수 엔진을 초기화
    std::random_device rd;
    randomNumberEngine_.seed(rd());
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

        // 선택 결과 출력 대신 ProcessTurn 함수를 호출
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

void BattleManager::ProcessTurn(const BattleAction& playerAction, const BattleAction& opponentAction)
{
    auto turnOrder = DetermineActionOrder(playerAction, opponentAction);
    TurnAction& first = turnOrder.first;
    TurnAction& second = turnOrder.second;

    ExecuteAction(first.attacker, first.target, first.move);
    if (first.target->IsFainted()) 
    {
        std::cout << first.target->GetName() << "은(는) 쓰러졌다!" << std::endl;
        return;
    }

    ExecuteAction(second.attacker, second.target, second.move);
    if (second.target->IsFainted()) 
    {
        std::cout << second.target->GetName() << "은(는) 쓰러졌다!" << std::endl;
    }
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
            if (action.move != nullptr) 
            { // 기술을 선택했다면
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

    auto& moveset = opponentActivePokemon_->GetMovesetForModify();
    if (moveset.empty()) 
    {
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

Move* BattleManager::SelectMove()
{
    auto& moveset = playerActivePokemon_->GetMovesetForModify();
    if (moveset.empty())
    {
        std::cout << "\n사용할 수 있는 기술이 없습니다!" << std::endl;
        return nullptr;
    }
    while (true)
    {
        std::cout << "어떤 기술을 사용할까?" << std::endl;
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
            // playerParty_[0] 대신 playerActivePokemon_ 사용
            return &playerActivePokemon_->GetMovesetForModify()[choice - 1];
        }
        else if (choice == moveset.size() + 1) 
        {
            return nullptr;
        }
        else 
        {
            std::cout << "\n잘못된 입력입니다." << std::endl;
        }
    }
}

void BattleManager::ShowMainMenu()
{
    std::cout << "\n" << playerActivePokemon_->GetName() << "는(은)" << "\n무엇을 할까?" << std::endl;
    std::cout << "1: 싸운다   2: 가방" << std::endl;
    std::cout << "3: 포켓몬   4: 도망친다" << std::endl;
}

void BattleManager::ExecuteAction(Pokemon* attacker, Pokemon* target, Move* move)
{
    move->DecrementPp();
    std::cout << "\n" << attacker->GetName() << "의 " << move->GetName() << "!" << std::endl;

    if (!HandleMoveAccuracy(attacker, move))
    {
        std::cout << "하지만 기술이 빗나갔다!" << std::endl;
        return;
    }

    ApplyMoveEffect(attacker, target, move);

    if (move->GetCategory() != MoveCategory::STATUS)
    {
        DamageResult result = CalculateAndApplyDamage(attacker, target, move);

        if (result.typeEffectiveness > 1.0) std::cout << "효과가 굉장했다!" << std::endl;
        if (result.typeEffectiveness < 1.0 && result.typeEffectiveness > 0) std::cout << "효과가 별로인 듯하다..." << std::endl;
        if (result.typeEffectiveness == 0) 
        {
            std::cout << "효과가 없는 것 같다..." << std::endl;
        }

        std::cout << target->GetName() << "은(는) " << result.damageDealt << "의 데미지를 받았다!" << std::endl;
        std::cout << target->GetName() << "의 남은 HP: " << target->GetCurrentHP() << "/" << target->GetMaxHP() << std::endl;
    }
}

bool BattleManager::HandleMoveAccuracy(Pokemon* attacker, const Move* move)
{
    int moveAccuracy = move->GetAccuracy();
    if (moveAccuracy > 100) return true; // 항상 성공하는 기술 (편의상 100 초과)

    // 멤버 변수인 randomNumberEngine_ 사용
    std::uniform_int_distribution<> distrib(1, 100);
    int randomValue = distrib(randomNumberEngine_);

    // TODO: 여기에 공격자의 명중률, 대상의 회피율 랭크 보정 추가

    return randomValue <= moveAccuracy;
}

void BattleManager::ApplyMoveEffect(Pokemon* attacker, Pokemon* target, const Move* move)
{
    // 앞으로 '울음소리' 등의 효과가 여기에 구현
    if (move->GetCategory() == MoveCategory::STATUS) 
    {
        std::cout << "하지만 아무 일도 일어나지 않았다..." << std::endl;
    }
}

DamageResult BattleManager::CalculateAndApplyDamage(Pokemon* attacker, Pokemon* target, const Move* move)
{
    DamageResult result; // 반환할 결과 객체

    int attackerLevel = attacker->GetLevel();
    int movePower = move->GetPower();
    int attackStat = (move->GetCategory() == MoveCategory::PHYSICAL) ? attacker->GetStat(Stat::ATTACK) : attacker->GetStat(Stat::SPECIAL_ATTACK);
    int defenseStat = (move->GetCategory() == MoveCategory::PHYSICAL) ? target->GetStat(Stat::DEFENSE) : target->GetStat(Stat::SPECIAL_DEFENSE);

    int damage = ((((attackerLevel * 2 / 5) + 2) * movePower * attackStat / defenseStat) / 50) + 2;

    double modifier = 1.0;

    if (move->GetType() == attacker->GetType1() || move->GetType() == attacker->GetType2()) 
    {
        modifier *= 1.5;
    }

    double typeMatchup = DataManager::GetInstance().GetTypeMatchup(move->GetType(), target->GetType1());
    if (target->GetType2() != Type::NONE) 
    {
        typeMatchup *= DataManager::GetInstance().GetTypeMatchup(move->GetType(), target->GetType2());
    }
    result.typeEffectiveness = typeMatchup;
    modifier *= typeMatchup;

    int finalDamage = static_cast<int>(damage * modifier);
    if (finalDamage < 1 && typeMatchup > 0) finalDamage = 1;

    result.damageDealt = finalDamage;
    target->TakeDamage(result.damageDealt);

    return result; // 결과 반환!
}

std::pair<TurnAction, TurnAction> BattleManager::DetermineActionOrder(const BattleAction& playerAction, const BattleAction& opponentAction)
{
    TurnAction playerTurn{ playerActivePokemon_, opponentActivePokemon_, playerAction.move };
    TurnAction opponentTurn{ opponentActivePokemon_, playerActivePokemon_, opponentAction.move };

    int playerPriority = playerAction.move->GetPriority();
    int opponentPriority = opponentAction.move->GetPriority();

    bool playerGoesFirst = (playerPriority > opponentPriority) ||
        (playerPriority == opponentPriority && playerActivePokemon_->GetStat(Stat::SPEED) >= opponentActivePokemon_->GetStat(Stat::SPEED));

    if (playerGoesFirst) 
    {
        return { playerTurn, opponentTurn };
    }
    else 
    {
        return { opponentTurn, playerTurn };
    }
}
