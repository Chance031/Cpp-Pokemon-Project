#include "BattleManager.h"

#include <iostream>

#include "DataManager.h"
#include "Move.h"

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

    while (!IsBattleOver())
    {
        std::cout << "\n--- ���ο� �� ����! ---" << std::endl;

        BattleAction playerAction = SelectPlayerAction();
        if (playerAction.type == PlayerActionType::CANCEL) continue;

        BattleAction opponentAction = SelectOpponentAction();

        // ���� ��� ��� ��� ProcessTurn �Լ��� ȣ��
        ProcessTurn(playerAction, opponentAction);
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
        return;
    }

    ExecuteAction(second.attacker, second.target, second.move);
    if (second.target->IsFainted()) 
    {
        std::cout << second.target->GetName() << "��(��) ��������!" << std::endl;
    }
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
            std::cout << "\n'���ϸ�'�� ��ü�� �� �����ϴ�." << std::endl;
            continue;

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
    // ������ '�����Ҹ�' ���� ȿ���� ���⿡ ����
    if (move->GetCategory() == MoveCategory::STATUS) 
    {
        std::cout << "������ �ƹ� �ϵ� �Ͼ�� �ʾҴ�..." << std::endl;
    }
}

DamageResult BattleManager::CalculateAndApplyDamage(Pokemon* attacker, Pokemon* target, const Move* move)
{
    DamageResult result; // ��ȯ�� ��� ��ü

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

    return result; // ��� ��ȯ!
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
