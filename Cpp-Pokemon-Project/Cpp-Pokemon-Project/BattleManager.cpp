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

        // ���� ��� ��� ��� ProcessTurn �Լ��� ȣ���մϴ�.
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

void BattleManager::ShowMainMenu()
{
    std::cout << "\n" << playerActivePokemon_->GetName() << "��(��)" << "\n������ �ұ�?" << std::endl;
    std::cout << "1: �ο��   2: ����" << std::endl;
    std::cout << "3: ���ϸ�   4: ����ģ��" << std::endl;
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
            if (action.move != nullptr) { // ����� �����ߴٸ�
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
        std::cout << "\n����� �� �ִ� ����� �����ϴ�!" << std::endl;
        return nullptr;
    }

    while (true)
    {
        std::cout << "� ����� ����ұ�?" << std::endl;

        // ��� ����� ��ȣ�� �Բ� ���
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
            // ����� �ùٸ��� ������ ���, �ش� ����� �ּ�(������)�� ��ȯ
             // playerParty_�� std::vector<Pokemon>�̹Ƿ�, �����Ͱ� �ƴ� ���� ��ü�Դϴ�.
             // ���� GetMoveset()���� ���� ������ ��� �ּҸ� �����ϰ� ��ȯ�� �� �ֽ��ϴ�.
            return &playerParty_[0].GetMovesetForModify()[choice - 1];
        }
        else if (choice == moveset.size() + 1)
        {
            return nullptr; // '�ڷΰ���'�� �����ϸ� null�� ��ȯ
        }
        else
        {
            std::cout << "\n�߸��� �Է��Դϴ�." << std::endl;
        }
    }
}

void BattleManager::ProcessTurn(const BattleAction& playerAction, const BattleAction& opponentAction)
{
    Pokemon* firstAttacker = nullptr;
    Pokemon* secondAttacker = nullptr;
    const Move* firstMove = nullptr;
    const Move* secondMove = nullptr;

    // --- �ൿ ���� ���� ---
    // 1. �켱�� ��
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
    else // 2. �켱���� ������ ���ǵ� ��
    {
        int playerScore = playerActivePokemon_->GetStat(Stat::SPEED);
        int opponentScore = opponentActivePokemon_->GetStat(Stat::SPEED);

        if (playerScore >= opponentScore) // ���ǵ尡 ������ �÷��̾ ����
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

    // --- ù ��° ���ϸ� �ൿ ���� ---
    ExecuteMove(firstAttacker, firstTarget, firstMove);

    // �ൿ ���� ��밡 �����ߴ��� Ȯ��
    if (firstTarget->IsFainted())
    {
        std::cout << firstTarget->GetName() << "��(��) ��������!" << std::endl;
        // ��밡 ���������Ƿ� �� ��° ���ϸ��� �ൿ�� �ʿ� ���� �� ����
        return;
    }

    // --- �� ��° ���ϸ� �ൿ ���� ---
    ExecuteMove(secondAttacker, secondTarget, secondMove);

    // �ൿ ���� ��밡 �����ߴ��� Ȯ��
    if (secondTarget->IsFainted())
    {
        std::cout << secondTarget->GetName() << "��(��) ��������!" << std::endl;
    }
}

void BattleManager::ExecuteMove(Pokemon* attacker, Pokemon* target, const Move* move)
{
    // const Move*�� Move*�� ĳ�����Ͽ� PP�� ������ �� �ְ� �մϴ�.
    // �̴� �÷��̾� ��Ƽ�� Move ��ü�� �����ϱ� �����Դϴ�.
    // ���� ������ �� ������, ���� �ܰ迡���� ���� ������ ����Դϴ�.
    Move* modifiableMove = const_cast<Move*>(move);
    modifiableMove->DecrementPp(); // <<< PP�� 1 ���ҽ�ŵ�ϴ�.

    std::cout << "\n" << attacker->GetName() << "�� " << move->GetName() << "!" << std::endl;

    // 1. ��ȭ ������� Ȯ�� (��ȭ ����� ������ ����� ���� ����)
    if (move->GetCategory() == MoveCategory::STATUS)
    {
        std::cout << "������ �ƹ� �ϵ� �Ͼ�� �ʾҴ�..." << std::endl;
        // TODO: ��ȭ ��� ȿ�� ó�� ����
        return;
    }

    // 2. ������ ��꿡 �ʿ��� �⺻ ���� ��������
    int attackerLevel = attacker->GetLevel();
    int movePower = move->GetPower();
    int attackStat = 0;
    int defenseStat = 0;

    // 3. ����� ����(����/Ư��)�� ���� ����� �ɷ�ġ�� ����
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

    // 4. �⺻ ������ ���
    int damage = ((((attackerLevel * 2 / 5) + 2) * movePower * attackStat / defenseStat) / 50) + 2;

    // --- 5. ������ ���� ���� ---
    double modifier = 1.0;

    // 5-1. �ڼ� ���� (STAB - Same-Type Attack Bonus)
    // �����ϴ� ���ϸ��� Ÿ�԰� ����ϴ� ����� Ÿ���� ������ ������ 1.5��
    if (move->GetType() == attacker->GetType1() || move->GetType() == attacker->GetType2())
    {
        modifier *= 1.5;
    }

    // 5-2. Ÿ�� �� ����
    double typeMatchup = 1.0;
    Type moveType = move->GetType();
    Type targetType1 = target->GetType1();
    Type targetType2 = target->GetType2();

    // ù ��° Ÿ�Կ� ���� ��
    typeMatchup *= DataManager::GetInstance().GetTypeMatchup(moveType, targetType1);
    // �� ��° Ÿ���� �ִٸ� ���� ���� ����
    if (targetType2 != Type::NONE)
    {
        typeMatchup *= DataManager::GetInstance().GetTypeMatchup(moveType, targetType2);
    }

    // Ÿ�� �� �޽��� ���
    if (typeMatchup > 1.0) std::cout << "ȿ���� �����ߴ�!" << std::endl;
    if (typeMatchup < 1.0 && typeMatchup > 0) std::cout << "ȿ���� ������ ���ϴ�..." << std::endl;
    if (typeMatchup == 0) std::cout << "ȿ���� ���� �� ����..." << std::endl;

    modifier *= typeMatchup;

    // TODO: ���⿡ �޼�(Critical Hit), Ư��, ������ �� �ٸ� �������� �߰�

    // 6. ���� ������ ��� �� ����
    int finalDamage = static_cast<int>(damage * modifier);
    if (finalDamage < 1) finalDamage = 1; // �ּ� 1�� �������� �ֵ��� ����

    target->TakeDamage(finalDamage);

    // 7. ��� ���
    std::cout << target->GetName() << "��(��) " << finalDamage << "�� �������� �޾Ҵ�!" << std::endl;
    std::cout << target->GetName() << "�� ���� HP: " << target->GetCurrentHP() << "/" << target->GetMaxHP() << std::endl;
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