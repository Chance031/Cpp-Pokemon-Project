#include "BattleManager.h"

#include <iostream>

#include "DataManager.h"
#include "Move.h"
#include "StringUtils.h"

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

    // while 루프를 아래 내용으로 교체하거나 수정해주세요.
    while (!IsBattleOver())
    {
        std::cout << "\n--- 새로운 턴 시작! ---" << std::endl;
        BattleAction playerAction = SelectPlayerAction();

        // 행동 타입에 따라 먼저 처리할 것들을 분기합니다.
        if (playerAction.type == PlayerActionType::CANCEL)
        {
            continue; // 행동을 다시 선택합니다.
        }

        if (playerAction.type == PlayerActionType::RUN)
        {
            // TODO: 나중에 여기에 스피드를 비교하여 도망 성공/실패 판정 로직 추가
            break; // 전투 루프를 완전히 빠져나갑니다.
        }

        // '싸운다'를 선택했을 때만 상대 행동을 정하고 턴을 진행합니다.
        if (playerAction.type == PlayerActionType::FIGHT)
        {
            BattleAction opponentAction = SelectOpponentAction();
            ProcessTurn(playerAction, opponentAction);
        }
        else if (playerAction.type == PlayerActionType::POKEMON)
        {
            // 플레이어가 먼저 교체
            ExecuteSwitch(playerActivePokemon_, playerParty_, playerAction.switchPokemonIndex);
            // 그 후 상대는 공격
            BattleAction opponentAction = SelectOpponentAction();
            ExecuteAction(opponentActivePokemon_, playerActivePokemon_, opponentAction.move);
        }
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

    // ▼▼▼▼▼ 여기에 특성 발동 로직 호출을 추가합니다 ▼▼▼▼▼
    // (순서는 보통 스피드가 빠른 포켓몬부터 발동하지만, 지금은 플레이어->상대 순서로 고정)
    TriggerSwitchInAbilities(playerActivePokemon_);
    TriggerSwitchInAbilities(opponentActivePokemon_);
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
        // 쓰러진 포켓몬이 플레이어의 포켓몬인지 확인
        if (first.target == playerActivePokemon_) {
            int switchIndex = SelectPokemonToSwitch(); // 교체할 포켓몬 강제 선택
            // TODO: -1 (뒤로가기) 선택 못하게 막는 로직 필요
            ExecuteSwitch(playerActivePokemon_, playerParty_, switchIndex);
        }
        // TODO: 상대 포켓몬이 쓰러졌을 때 AI가 교체하는 로직
        return; // 턴 종료
    }

    ExecuteAction(second.attacker, second.target, second.move);
    if (first.target->IsFainted())
    {
        std::cout << first.target->GetName() << "은(는) 쓰러졌다!" << std::endl;
        // 쓰러진 포켓몬이 플레이어의 포켓몬인지 확인
        if (first.target == playerActivePokemon_) {
            int switchIndex = SelectPokemonToSwitch(); // 교체할 포켓몬 강제 선택
            // TODO: -1 (뒤로가기) 선택 못하게 막는 로직 필요
            ExecuteSwitch(playerActivePokemon_, playerParty_, switchIndex);
        }
        // TODO: 상대 포켓몬이 쓰러졌을 때 AI가 교체하는 로직
        return; // 턴 종료
    }

    std::cout << "--- 턴 종료 효과 ---" << std::endl;
    ProcessEndOfTurnEffects(first.attacker);
    ProcessEndOfTurnEffects(second.attacker);
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
        {
            int switchIndex = SelectPokemonToSwitch();
            if (switchIndex != -1) { // 포켓몬을 선택했다면
                action.type = PlayerActionType::POKEMON;
                action.switchPokemonIndex = switchIndex;
                return action;
            }
            // 뒤로가기를 선택했다면 루프 계속
            continue;
        }

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

    // 로컬 난수 엔진 대신 멤버 변수인 randomNumberEngine_을 사용합니다.
    std::uniform_int_distribution<> distrib(0, moveset.size() - 1);
    int randomIndex = distrib(randomNumberEngine_);

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
    if (attacker->GetPrimaryStatus() == StatusCondition::PARALYSIS)
    {
        // 1~100 사이의 난수를 생성
        std::uniform_int_distribution<> distrib(1, 100);
        int randomValue = distrib(randomNumberEngine_);

        // 25% 확률에 당첨되면 행동 실패
        if (randomValue <= 25)
        {
            std::cout << attacker->GetName() << "은(는) 몸이 저려서 움직일 수 없다!" << std::endl;
            return; // 함수를 즉시 종료하여 기술 사용을 막음
        }
    }

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
    int effectId = move->GetEffectId();
    if (effectId == 0)
    {
        // 부가 효과가 없는 STATUS 기술일 경우 메시지 출력
        if (move->GetCategory() == MoveCategory::STATUS)
        {
            std::cout << "하지만 아무 일도 일어나지 않았다..." << std::endl;
        }
        return;
    }

    const MoveEffectData& effect = DataManager::GetInstance().GetMoveEffectData(effectId);

    int effectChance = move->GetEffectChance();
    // 부가 효과가 확률적으로 발동하는 경우 (0% 이상)
    if (effectChance > 0)
    {
        std::uniform_int_distribution<> distrib(1, 100);
        if (distrib(randomNumberEngine_) > effectChance)
        {
            return; // 확률 판정 실패 시, 아무 효과 없이 함수 종료
        }
    }

    switch (effect.category)
    {
    case EffectCategory::STAT_CHANGE:
    {
        for (const auto& change : effect.statChanges)
        {
            target->ApplyStatStageChange(change.stat, change.stages);

            std::string statName = StringUtils::StatToKorean(change.stat);
            if (change.stages < 0) {
                std::cout << target->GetName() << "의 " << statName << "이(가) 떨어졌다!" << std::endl;
            }
            else {
                std::cout << target->GetName() << "의 " << statName << "이(가) 올라갔다!" << std::endl;
            }
        }
        break;
    }

    case EffectCategory::PRIMARY_STATUS:
    {
        // 1. 대상의 현재 상태 이상을 확인합니다.
        StatusCondition currentStatus = target->GetPrimaryStatus();

        if (currentStatus != StatusCondition::NONE)
        {
            // 2. 이미 상태 이상에 걸려있다면, 메시지를 출력하고 실패 처리합니다.
            std::string statusName = StringUtils::StatusConditionToKorean(currentStatus);
            std::cout << target->GetName() << "은(는) 이미 " << statusName << " 상태다!" << std::endl;
        }
        else
        {
            // 3. 상태 이상에 걸려있지 않다면, 새로운 상태 이상을 적용합니다.
            StatusCondition newStatus = effect.primaryStatus;
            target->SetPrimaryStatus(newStatus);

            std::string statusName = StringUtils::StatusConditionToKorean(newStatus);
            std::cout << target->GetName() << "은(는) " << statusName << " 상태가 되었다!" << std::endl;
        }
        break;
    }

    default:
        // STAT_CHANGE 외 다른 효과가 없는 STATUS 기술일 경우
        if (move->GetCategory() == MoveCategory::STATUS)
        {
            std::cout << "하지만 아무 일도 일어나지 않았다..." << std::endl;
        }
        break;
    }
}

DamageResult BattleManager::CalculateAndApplyDamage(Pokemon* attacker, Pokemon* target, const Move* move)
{
    DamageResult result;

    // 1. 급소 판정
    bool isCritical = false;
    std::uniform_int_distribution<> distrib(1, 24);
    if (distrib(randomNumberEngine_) == 1) 
    { 
        isCritical = true;
    }

    int attackerLevel = attacker->GetLevel();
    int movePower = move->GetPower();
    int attackStat, defenseStat;

    auto getStageMultiplier = [](int stage) -> double 
        {
        if (stage > 0) return (2.0 + stage) / 2.0;
        if (stage < 0) return 2.0 / (2.0 - stage);
        return 1.0;
        };

    if (move->GetCategory() == MoveCategory::PHYSICAL) {
        attackStat = attacker->GetStat(Stat::ATTACK);
        defenseStat = target->GetStat(Stat::DEFENSE);

        int attackStage = attacker->GetStatStage(Stat::ATTACK);
        int defenseStage = target->GetStatStage(Stat::DEFENSE);

        // 급소일 경우, 유리한 랭크 변화만 적용
        if (isCritical) 
        {
            if (attackStage < 0) attackStage = 0; // 공격자의 랭크 하락은 무시
            if (defenseStage > 0) defenseStage = 0; // 방어자의 랭크 상승은 무시
        }

        // 물리 기술일 때는 공격/방어 랭크만 적용
        attackStat = static_cast<int>(attackStat * getStageMultiplier(attacker->GetStatStage(Stat::ATTACK)));
        defenseStat = static_cast<int>(defenseStat * getStageMultiplier(target->GetStatStage(Stat::DEFENSE)));
    }
    else { // SPECIAL
        attackStat = attacker->GetStat(Stat::SPECIAL_ATTACK);
        defenseStat = target->GetStat(Stat::SPECIAL_DEFENSE);

        // 특수 기술일 때는 특수공격/특수방어 랭크만 적용
        attackStat = static_cast<int>(attackStat * getStageMultiplier(attacker->GetStatStage(Stat::SPECIAL_ATTACK)));
        defenseStat = static_cast<int>(defenseStat * getStageMultiplier(target->GetStatStage(Stat::SPECIAL_DEFENSE)));
    }

    if (defenseStat == 0) defenseStat = 1;

    int damage = ((((attackerLevel * 2 / 5) + 2) * movePower * attackStat / defenseStat) / 50) + 2;

    double modifier = 1.0;

    // 2. 급소 데미지 보정 적용
    if (isCritical) {
        std::cout << "급소에 맞았다!" << std::endl;
        modifier *= 1.5;
    }

    if (move->GetType() == attacker->GetType1() || move->GetType() == attacker->GetType2()) {
        modifier *= 1.5;
    }

    double typeMatchup = DataManager::GetInstance().GetTypeMatchup(move->GetType(), target->GetType1());
    if (target->GetType2() != Type::NONE) {
        typeMatchup *= DataManager::GetInstance().GetTypeMatchup(move->GetType(), target->GetType2());
    }
    result.typeEffectiveness = typeMatchup;
    modifier *= typeMatchup;

    int finalDamage = static_cast<int>(damage * modifier);
    if (finalDamage < 1 && typeMatchup > 0) finalDamage = 1;

    result.damageDealt = finalDamage;
    target->TakeDamage(result.damageDealt);

    return result;
}

std::pair<TurnAction, TurnAction> BattleManager::DetermineActionOrder(const BattleAction& playerAction, const BattleAction& opponentAction)
{
    TurnAction playerTurn{ playerActivePokemon_, opponentActivePokemon_, playerAction.move };
    TurnAction opponentTurn{ opponentActivePokemon_, playerActivePokemon_, opponentAction.move };

    int playerPriority = playerAction.move->GetPriority();
    int opponentPriority = opponentAction.move->GetPriority();

    // 우선도가 같다면 스피드를 비교
    if (playerPriority == opponentPriority)
    {
        // 각 포켓몬의 스피드를 가져옴
        int playerSpeed = playerActivePokemon_->GetStat(Stat::SPEED);
        int opponentSpeed = opponentActivePokemon_->GetStat(Stat::SPEED);

        // 만약 마비 상태라면 스피드를 50%로 감소
        if (playerActivePokemon_->GetPrimaryStatus() == StatusCondition::PARALYSIS) {
            playerSpeed *= 0.5;
        }
        if (opponentActivePokemon_->GetPrimaryStatus() == StatusCondition::PARALYSIS) {
            opponentSpeed *= 0.5;
        }

        // 최종 스피드를 비교하여 순서 결정
        if (playerSpeed >= opponentSpeed) {
            return { playerTurn, opponentTurn };
        }
        else {
            return { opponentTurn, playerTurn };
        }
    }
    // 우선도가 다르다면 우선도에 따라 순서 결정
    else if (playerPriority > opponentPriority) {
        return { playerTurn, opponentTurn };
    }
    else {
        return { opponentTurn, playerTurn };
    }
}

void BattleManager::ProcessEndOfTurnEffects(Pokemon* pokemon)
{
    if (pokemon->IsFainted()) return; // 쓰러진 포켓몬은 효과를 받지 않음

    switch (pokemon->GetPrimaryStatus())
    {
    case StatusCondition::POISON:
    {
        std::cout << pokemon->GetName() << "은(는) 독의 데미지를 입었다!" << std::endl;
        int poisonDamage = pokemon->GetMaxHP() / 8;
        if (poisonDamage < 1) poisonDamage = 1; // 최소 1의 데미지
        pokemon->TakeDamage(poisonDamage);
        std::cout << pokemon->GetName() << "의 남은 HP: " << pokemon->GetCurrentHP() << "/" << pokemon->GetMaxHP() << std::endl;
        break;
    }
    // TODO: 나중에 화상(BURN) 등의 다른 턴 종료 효과를 여기에 추가
    default:
        break;
    }
}

void BattleManager::TriggerSwitchInAbilities(Pokemon* switchedInPokemon)
{
    if (switchedInPokemon->IsFainted()) return;

    const AbilityData* ability = switchedInPokemon->GetAbility();
    if (ability == nullptr) return;

    // '위협' 특성 (ID: 22)인지 확인
    if (ability->id == 22)
    {
        Pokemon* opponent = (switchedInPokemon == playerActivePokemon_) ? opponentActivePokemon_ : playerActivePokemon_;
        if (opponent->IsFainted()) return;

        std::cout << switchedInPokemon->GetName() << "의 특성 [위협]!" << std::endl;
        opponent->ApplyStatStageChange(Stat::ATTACK, -1);
        std::cout << opponent->GetName() << "의 공격이 떨어졌다!" << std::endl;
    }

    // TODO: '가뭄', '잔비' 등 다른 등장 시 발동 특성을 여기에 추가
}

int BattleManager::SelectPokemonToSwitch()
{
    while (true)
    {
        std::cout << "\n누구를 내보내시겠습니까?" << std::endl;
        for (int i = 0; i < playerParty_.size(); ++i)
        {
            const auto& p = playerParty_[i];
            std::cout << i + 1 << ": " << p.GetName() << " (HP: " << p.GetCurrentHP() << "/" << p.GetMaxHP() << ")";
            if (&p == playerActivePokemon_) std::cout << " [현재 배틀 중]";
            if (p.IsFainted()) std::cout << " [기절]";
            std::cout << std::endl;
        }
        std::cout << playerParty_.size() + 1 << ": 뒤로가기" << std::endl;
        std::cout << "> ";

        int choice;
        std::cin >> choice;
        int choiceIndex = choice - 1;

        if (choice > 0 && choice <= playerParty_.size())
        {
            if (&playerParty_[choiceIndex] == playerActivePokemon_) {
                std::cout << "이미 배틀에 나가있는 포켓몬입니다!" << std::endl;
                continue;
            }
            if (playerParty_[choiceIndex].IsFainted()) {
                std::cout << "기절한 포켓몬은 내보낼 수 없습니다!" << std::endl;
                continue;
            }
            return choiceIndex; // 유효한 선택. 인덱스 반환
        }
        else if (choice == playerParty_.size() + 1) {
            return -1; // 뒤로가기
        }
        else {
            std::cout << "잘못된 입력입니다." << std::endl;
        }
    }
}

void BattleManager::ExecuteSwitch(Pokemon*& activePokemon, std::vector<Pokemon>& party, int newPokemonIndex)
{
    // 1. 기존 포켓몬 처리
    std::cout << activePokemon->GetName() << ", 돌아와!" << std::endl;
    activePokemon->ResetStatStages();

    // 2. 새로운 포켓몬으로 교체
    activePokemon = &party[newPokemonIndex];
    std::cout << "가라! " << activePokemon->GetName() << "!" << std::endl;

    // 3. 등장 시 특성 발동
    TriggerSwitchInAbilities(activePokemon);
}
