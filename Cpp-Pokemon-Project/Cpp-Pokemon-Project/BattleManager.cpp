#include "BattleManager.h"

#include <iostream>

#include "DataManager.h"
#include "Move.h"
#include "StringUtils.h"

// ������
BattleManager::BattleManager(std::vector<Pokemon>& playerParty, std::vector<Pokemon>& opponentParty)
	: playerParty_(playerParty), opponentParty_(opponentParty)
{
    // ���� �Ұ����� �õ�� ���� ������ �ʱ�ȭ
    std::random_device rd;
    randomNumberEngine_.seed(rd());
}

void BattleManager::Start()
{
    if (playerParty_.empty() || opponentParty_.empty())
    {
        std::cout << "����: ���ϸ� ��Ƽ�� ����־� ������ ������ �� �����ϴ�." << std::endl;
        return;
    }

    playerActivePokemon_ = &playerParty_[0];
    opponentActivePokemon_ = &opponentParty_[0];

    PlayIntroSequence();

    // while ������ �Ʒ� �������� ��ü�ϰų� �������ּ���.
    while (!IsBattleOver())
    {
        std::cout << "\n--- ���ο� �� ����! ---" << std::endl;
        BattleAction playerAction = SelectPlayerAction();

        // �ൿ Ÿ�Կ� ���� ���� ó���� �͵��� �б��մϴ�.
        if (playerAction.type == PlayerActionType::CANCEL)
        {
            continue; // �ൿ�� �ٽ� �����մϴ�.
        }

        if (playerAction.type == PlayerActionType::RUN)
        {
            // TODO: ���߿� ���⿡ ���ǵ带 ���Ͽ� ���� ����/���� ���� ���� �߰�
            break; // ���� ������ ������ ���������ϴ�.
        }

        // '�ο��'�� �������� ���� ��� �ൿ�� ���ϰ� ���� �����մϴ�.
        if (playerAction.type == PlayerActionType::FIGHT)
        {
            BattleAction opponentAction = SelectOpponentAction();
            ProcessTurn(playerAction, opponentAction);
        }
        else if (playerAction.type == PlayerActionType::POKEMON)
        {
            // �÷��̾ ���� ��ü
            ExecuteSwitch(playerActivePokemon_, playerParty_, playerAction.switchPokemonIndex);
            // �� �� ���� ����
            BattleAction opponentAction = SelectOpponentAction();
            ExecuteAction(opponentActivePokemon_, playerActivePokemon_, opponentAction.move);
        }
    }

    std::cout << "\n--- ���� ����! ---" << std::endl;
}

void BattleManager::PlayIntroSequence()
{
    // TODO: �߻� ���ϸ�����, Ʈ���̳� ������� �����ϴ� ���� �ʿ�
    bool isWildBattle = true; // ������ �߻� ���ϸ� ��Ʋ�̶�� ����

    if (isWildBattle)
    {
        std::cout << "��! �߻��� " << opponentActivePokemon_->GetName()
            << "(��)�� ��Ÿ����!" << std::endl;
    }
    // else { /* Ʈ���̳� ��Ʋ �޽��� */ }

    std::cout << "����! " << playerActivePokemon_->GetName() << "!" << std::endl;

    // ������ ���⿡ Ư�� �ߵ� ���� ȣ���� �߰��մϴ� ������
    // (������ ���� ���ǵ尡 ���� ���ϸ���� �ߵ�������, ������ �÷��̾�->��� ������ ����)
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
        std::cout << first.target->GetName() << "��(��) ��������!" << std::endl;
        // ������ ���ϸ��� �÷��̾��� ���ϸ����� Ȯ��
        if (first.target == playerActivePokemon_) {
            int switchIndex = SelectPokemonToSwitch(); // ��ü�� ���ϸ� ���� ����
            // TODO: -1 (�ڷΰ���) ���� ���ϰ� ���� ���� �ʿ�
            ExecuteSwitch(playerActivePokemon_, playerParty_, switchIndex);
        }
        // TODO: ��� ���ϸ��� �������� �� AI�� ��ü�ϴ� ����
        return; // �� ����
    }

    ExecuteAction(second.attacker, second.target, second.move);
    if (first.target->IsFainted())
    {
        std::cout << first.target->GetName() << "��(��) ��������!" << std::endl;
        // ������ ���ϸ��� �÷��̾��� ���ϸ����� Ȯ��
        if (first.target == playerActivePokemon_) {
            int switchIndex = SelectPokemonToSwitch(); // ��ü�� ���ϸ� ���� ����
            // TODO: -1 (�ڷΰ���) ���� ���ϰ� ���� ���� �ʿ�
            ExecuteSwitch(playerActivePokemon_, playerParty_, switchIndex);
        }
        // TODO: ��� ���ϸ��� �������� �� AI�� ��ü�ϴ� ����
        return; // �� ����
    }

    std::cout << "--- �� ���� ȿ�� ---" << std::endl;
    ProcessEndOfTurnEffects(first.attacker);
    ProcessEndOfTurnEffects(second.attacker);
}

bool BattleManager::IsBattleOver()
{
    // �÷��̾� ��Ƽ�� �����ߴ��� Ȯ��
    bool playerAllFainted = true;
    for (const auto& pokemon : playerParty_)
    {
        if (!pokemon.IsFainted()) // ���� �������� ���� ���ϸ��� �ִٸ�
        {
            playerAllFainted = false; // ������ �ƴ�
            break;
        }
    }

    // ��� ��Ƽ�� �����ߴ��� Ȯ��
    bool opponentAllFainted = true;
    for (const auto& pokemon : opponentParty_)
    {
        if (!pokemon.IsFainted()) // ���� �������� ���� ���ϸ��� �ִٸ�
        {
            opponentAllFainted = false; // ������ �ƴ�
            break;
        }
    }

    // �� �� �����̶� �����ߴٸ� true�� ��ȯ�Ͽ� ���� ����
    return playerAllFainted || opponentAllFainted;
}

BattleAction BattleManager::SelectPlayerAction()
{
    BattleAction action; // ��ȯ�� action ��ü

    while (true)
    {
        ShowMainMenu();
        std::cout << "> ";
        int choice;
        std::cin >> choice;

        switch (choice)
        {
        case 1: // �ο��
            action.move = SelectMove();
            if (action.move != nullptr) 
            { // ����� �����ߴٸ�
                action.type = PlayerActionType::FIGHT;
                return action;
            }
            // �ڷΰ��⸦ �����ߴٸ� ���� ���
            continue;

        case 2: // ����
            std::cout << "\n'����'�� ���� �� �� �����ϴ�." << std::endl;
            continue;

        case 3: // ���ϸ�
        {
            int switchIndex = SelectPokemonToSwitch();
            if (switchIndex != -1) { // ���ϸ��� �����ߴٸ�
                action.type = PlayerActionType::POKEMON;
                action.switchPokemonIndex = switchIndex;
                return action;
            }
            // �ڷΰ��⸦ �����ߴٸ� ���� ���
            continue;
        }

        case 4: // ����ģ��
            std::cout << "\n������ �����ƴ�!" << std::endl;
            action.type = PlayerActionType::RUN;
            return action;

        default:
            std::cout << "\n�߸��� �Է��Դϴ�. 1~4 ������ ���ڸ� �Է����ּ���." << std::endl;
            continue;
        }
    }
}

BattleAction BattleManager::SelectOpponentAction()
{
    BattleAction action;
    action.type = PlayerActionType::FIGHT; // AI�� ������ ����

    auto& moveset = opponentActivePokemon_->GetMovesetForModify();
    if (moveset.empty())
    {
        action.move = nullptr;
        return action;
    }

    // ���� ���� ���� ��� ��� ������ randomNumberEngine_�� ����մϴ�.
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
        std::cout << "\n����� �� �ִ� ����� �����ϴ�!" << std::endl;
        return nullptr;
    }
    while (true)
    {
        std::cout << "� ����� ����ұ�?" << std::endl;
        for (int i = 0; i < moveset.size(); ++i)
        {
            std::cout << i + 1 << ": " << moveset[i].GetName() << " (PP: " << moveset[i].GetCurrentPp() << "/" << moveset[i].GetMaxPp() << ")" << std::endl;
        }
        std::cout << moveset.size() + 1 << ": �ڷΰ���" << std::endl;
        std::cout << "> ";
        int choice;
        std::cin >> choice;
        if (choice > 0 && choice <= moveset.size())
        {
            // playerParty_[0] ��� playerActivePokemon_ ���
            return &playerActivePokemon_->GetMovesetForModify()[choice - 1];
        }
        else if (choice == moveset.size() + 1) 
        {
            return nullptr;
        }
        else 
        {
            std::cout << "\n�߸��� �Է��Դϴ�." << std::endl;
        }
    }
}

void BattleManager::ShowMainMenu()
{
    std::cout << "\n" << playerActivePokemon_->GetName() << "��(��)" << "\n������ �ұ�?" << std::endl;
    std::cout << "1: �ο��   2: ����" << std::endl;
    std::cout << "3: ���ϸ�   4: ����ģ��" << std::endl;
}

void BattleManager::ExecuteAction(Pokemon* attacker, Pokemon* target, Move* move)
{
    if (attacker->GetPrimaryStatus() == StatusCondition::PARALYSIS)
    {
        // 1~100 ������ ������ ����
        std::uniform_int_distribution<> distrib(1, 100);
        int randomValue = distrib(randomNumberEngine_);

        // 25% Ȯ���� ��÷�Ǹ� �ൿ ����
        if (randomValue <= 25)
        {
            std::cout << attacker->GetName() << "��(��) ���� ������ ������ �� ����!" << std::endl;
            return; // �Լ��� ��� �����Ͽ� ��� ����� ����
        }
    }

    move->DecrementPp();
    std::cout << "\n" << attacker->GetName() << "�� " << move->GetName() << "!" << std::endl;

    if (!HandleMoveAccuracy(attacker, move))
    {
        std::cout << "������ ����� ��������!" << std::endl;
        return;
    }

    ApplyMoveEffect(attacker, target, move);

    if (move->GetCategory() != MoveCategory::STATUS)
    {
        DamageResult result = CalculateAndApplyDamage(attacker, target, move);

        if (result.typeEffectiveness > 1.0) std::cout << "ȿ���� �����ߴ�!" << std::endl;
        if (result.typeEffectiveness < 1.0 && result.typeEffectiveness > 0) std::cout << "ȿ���� ������ ���ϴ�..." << std::endl;
        if (result.typeEffectiveness == 0) 
        {
            std::cout << "ȿ���� ���� �� ����..." << std::endl;
        }

        std::cout << target->GetName() << "��(��) " << result.damageDealt << "�� �������� �޾Ҵ�!" << std::endl;
        std::cout << target->GetName() << "�� ���� HP: " << target->GetCurrentHP() << "/" << target->GetMaxHP() << std::endl;
    }
}

bool BattleManager::HandleMoveAccuracy(Pokemon* attacker, const Move* move)
{
    int moveAccuracy = move->GetAccuracy();
    if (moveAccuracy > 100) return true; // �׻� �����ϴ� ��� (���ǻ� 100 �ʰ�)

    // ��� ������ randomNumberEngine_ ���
    std::uniform_int_distribution<> distrib(1, 100);
    int randomValue = distrib(randomNumberEngine_);

    // TODO: ���⿡ �������� ���߷�, ����� ȸ���� ��ũ ���� �߰�

    return randomValue <= moveAccuracy;
}

void BattleManager::ApplyMoveEffect(Pokemon* attacker, Pokemon* target, const Move* move)
{
    int effectId = move->GetEffectId();
    if (effectId == 0)
    {
        // �ΰ� ȿ���� ���� STATUS ����� ��� �޽��� ���
        if (move->GetCategory() == MoveCategory::STATUS)
        {
            std::cout << "������ �ƹ� �ϵ� �Ͼ�� �ʾҴ�..." << std::endl;
        }
        return;
    }

    const MoveEffectData& effect = DataManager::GetInstance().GetMoveEffectData(effectId);

    int effectChance = move->GetEffectChance();
    // �ΰ� ȿ���� Ȯ�������� �ߵ��ϴ� ��� (0% �̻�)
    if (effectChance > 0)
    {
        std::uniform_int_distribution<> distrib(1, 100);
        if (distrib(randomNumberEngine_) > effectChance)
        {
            return; // Ȯ�� ���� ���� ��, �ƹ� ȿ�� ���� �Լ� ����
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
                std::cout << target->GetName() << "�� " << statName << "��(��) ��������!" << std::endl;
            }
            else {
                std::cout << target->GetName() << "�� " << statName << "��(��) �ö󰬴�!" << std::endl;
            }
        }
        break;
    }

    case EffectCategory::PRIMARY_STATUS:
    {
        // 1. ����� ���� ���� �̻��� Ȯ���մϴ�.
        StatusCondition currentStatus = target->GetPrimaryStatus();

        if (currentStatus != StatusCondition::NONE)
        {
            // 2. �̹� ���� �̻� �ɷ��ִٸ�, �޽����� ����ϰ� ���� ó���մϴ�.
            std::string statusName = StringUtils::StatusConditionToKorean(currentStatus);
            std::cout << target->GetName() << "��(��) �̹� " << statusName << " ���´�!" << std::endl;
        }
        else
        {
            // 3. ���� �̻� �ɷ����� �ʴٸ�, ���ο� ���� �̻��� �����մϴ�.
            StatusCondition newStatus = effect.primaryStatus;
            target->SetPrimaryStatus(newStatus);

            std::string statusName = StringUtils::StatusConditionToKorean(newStatus);
            std::cout << target->GetName() << "��(��) " << statusName << " ���°� �Ǿ���!" << std::endl;
        }
        break;
    }

    default:
        // STAT_CHANGE �� �ٸ� ȿ���� ���� STATUS ����� ���
        if (move->GetCategory() == MoveCategory::STATUS)
        {
            std::cout << "������ �ƹ� �ϵ� �Ͼ�� �ʾҴ�..." << std::endl;
        }
        break;
    }
}

DamageResult BattleManager::CalculateAndApplyDamage(Pokemon* attacker, Pokemon* target, const Move* move)
{
    DamageResult result;

    // 1. �޼� ����
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

        // �޼��� ���, ������ ��ũ ��ȭ�� ����
        if (isCritical) 
        {
            if (attackStage < 0) attackStage = 0; // �������� ��ũ �϶��� ����
            if (defenseStage > 0) defenseStage = 0; // ������� ��ũ ����� ����
        }

        // ���� ����� ���� ����/��� ��ũ�� ����
        attackStat = static_cast<int>(attackStat * getStageMultiplier(attacker->GetStatStage(Stat::ATTACK)));
        defenseStat = static_cast<int>(defenseStat * getStageMultiplier(target->GetStatStage(Stat::DEFENSE)));
    }
    else { // SPECIAL
        attackStat = attacker->GetStat(Stat::SPECIAL_ATTACK);
        defenseStat = target->GetStat(Stat::SPECIAL_DEFENSE);

        // Ư�� ����� ���� Ư������/Ư����� ��ũ�� ����
        attackStat = static_cast<int>(attackStat * getStageMultiplier(attacker->GetStatStage(Stat::SPECIAL_ATTACK)));
        defenseStat = static_cast<int>(defenseStat * getStageMultiplier(target->GetStatStage(Stat::SPECIAL_DEFENSE)));
    }

    if (defenseStat == 0) defenseStat = 1;

    int damage = ((((attackerLevel * 2 / 5) + 2) * movePower * attackStat / defenseStat) / 50) + 2;

    double modifier = 1.0;

    // 2. �޼� ������ ���� ����
    if (isCritical) {
        std::cout << "�޼ҿ� �¾Ҵ�!" << std::endl;
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

    // �켱���� ���ٸ� ���ǵ带 ��
    if (playerPriority == opponentPriority)
    {
        // �� ���ϸ��� ���ǵ带 ������
        int playerSpeed = playerActivePokemon_->GetStat(Stat::SPEED);
        int opponentSpeed = opponentActivePokemon_->GetStat(Stat::SPEED);

        // ���� ���� ���¶�� ���ǵ带 50%�� ����
        if (playerActivePokemon_->GetPrimaryStatus() == StatusCondition::PARALYSIS) {
            playerSpeed *= 0.5;
        }
        if (opponentActivePokemon_->GetPrimaryStatus() == StatusCondition::PARALYSIS) {
            opponentSpeed *= 0.5;
        }

        // ���� ���ǵ带 ���Ͽ� ���� ����
        if (playerSpeed >= opponentSpeed) {
            return { playerTurn, opponentTurn };
        }
        else {
            return { opponentTurn, playerTurn };
        }
    }
    // �켱���� �ٸ��ٸ� �켱���� ���� ���� ����
    else if (playerPriority > opponentPriority) {
        return { playerTurn, opponentTurn };
    }
    else {
        return { opponentTurn, playerTurn };
    }
}

void BattleManager::ProcessEndOfTurnEffects(Pokemon* pokemon)
{
    if (pokemon->IsFainted()) return; // ������ ���ϸ��� ȿ���� ���� ����

    switch (pokemon->GetPrimaryStatus())
    {
    case StatusCondition::POISON:
    {
        std::cout << pokemon->GetName() << "��(��) ���� �������� �Ծ���!" << std::endl;
        int poisonDamage = pokemon->GetMaxHP() / 8;
        if (poisonDamage < 1) poisonDamage = 1; // �ּ� 1�� ������
        pokemon->TakeDamage(poisonDamage);
        std::cout << pokemon->GetName() << "�� ���� HP: " << pokemon->GetCurrentHP() << "/" << pokemon->GetMaxHP() << std::endl;
        break;
    }
    // TODO: ���߿� ȭ��(BURN) ���� �ٸ� �� ���� ȿ���� ���⿡ �߰�
    default:
        break;
    }
}

void BattleManager::TriggerSwitchInAbilities(Pokemon* switchedInPokemon)
{
    if (switchedInPokemon->IsFainted()) return;

    const AbilityData* ability = switchedInPokemon->GetAbility();
    if (ability == nullptr) return;

    // '����' Ư�� (ID: 22)���� Ȯ��
    if (ability->id == 22)
    {
        Pokemon* opponent = (switchedInPokemon == playerActivePokemon_) ? opponentActivePokemon_ : playerActivePokemon_;
        if (opponent->IsFainted()) return;

        std::cout << switchedInPokemon->GetName() << "�� Ư�� [����]!" << std::endl;
        opponent->ApplyStatStageChange(Stat::ATTACK, -1);
        std::cout << opponent->GetName() << "�� ������ ��������!" << std::endl;
    }

    // TODO: '����', '�ܺ�' �� �ٸ� ���� �� �ߵ� Ư���� ���⿡ �߰�
}

int BattleManager::SelectPokemonToSwitch()
{
    while (true)
    {
        std::cout << "\n������ �������ðڽ��ϱ�?" << std::endl;
        for (int i = 0; i < playerParty_.size(); ++i)
        {
            const auto& p = playerParty_[i];
            std::cout << i + 1 << ": " << p.GetName() << " (HP: " << p.GetCurrentHP() << "/" << p.GetMaxHP() << ")";
            if (&p == playerActivePokemon_) std::cout << " [���� ��Ʋ ��]";
            if (p.IsFainted()) std::cout << " [����]";
            std::cout << std::endl;
        }
        std::cout << playerParty_.size() + 1 << ": �ڷΰ���" << std::endl;
        std::cout << "> ";

        int choice;
        std::cin >> choice;
        int choiceIndex = choice - 1;

        if (choice > 0 && choice <= playerParty_.size())
        {
            if (&playerParty_[choiceIndex] == playerActivePokemon_) {
                std::cout << "�̹� ��Ʋ�� �����ִ� ���ϸ��Դϴ�!" << std::endl;
                continue;
            }
            if (playerParty_[choiceIndex].IsFainted()) {
                std::cout << "������ ���ϸ��� ������ �� �����ϴ�!" << std::endl;
                continue;
            }
            return choiceIndex; // ��ȿ�� ����. �ε��� ��ȯ
        }
        else if (choice == playerParty_.size() + 1) {
            return -1; // �ڷΰ���
        }
        else {
            std::cout << "�߸��� �Է��Դϴ�." << std::endl;
        }
    }
}

void BattleManager::ExecuteSwitch(Pokemon*& activePokemon, std::vector<Pokemon>& party, int newPokemonIndex)
{
    // 1. ���� ���ϸ� ó��
    std::cout << activePokemon->GetName() << ", ���ƿ�!" << std::endl;
    activePokemon->ResetStatStages();

    // 2. ���ο� ���ϸ����� ��ü
    activePokemon = &party[newPokemonIndex];
    std::cout << "����! " << activePokemon->GetName() << "!" << std::endl;

    // 3. ���� �� Ư�� �ߵ�
    TriggerSwitchInAbilities(activePokemon);
}
