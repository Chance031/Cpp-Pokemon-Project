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
        std::cout << "����: ���ϸ� ��Ƽ�� ����־� ������ ������ �� �����ϴ�." << std::endl;
        return;
    }

    playerActivePokemon_ = &playerParty_[0];
    opponentActivePokemon_ = &opponentParty_[0];

    PlayIntroSequence();

    // ���� ��Ʋ ���� (������ �� ���� ����)
    while (true)
    {
        ShowMainMenu(); // <<< �޴� ��� �Լ� ȣ��

        std::cout << "> ";
        int choice;
        std::cin >> choice; // �÷��̾��� �Է��� �޽��ϴ�.

        switch (choice)
        {
        case 1: // �ο��
        {
            Move* selectedMove = SelectMove(); // <<< SelectMove �Լ� ȣ��
            if (selectedMove != nullptr) // ����� �����ߴٸ� (�ڷΰ��� X)
            {
                std::cout << "\n" << playerActivePokemon_->GetName() << "��(��) "
                    << selectedMove->GetName() << "��(��) ����ߴ�!" << std::endl;
                // TODO: ��� ��� ���� (PP ���� ��)
                break; // ���� ����
            }
            // �ڷΰ��⸦ �����ߴٸ� �ƹ��͵� ���� �ʰ� �ٽ� ���� �޴���
            continue;
        }

        case 2: // ����
            std::cout << "\n'����'�� ���� �� �� �����ϴ�." << std::endl;
            continue;

        case 3: // ���ϸ�
            std::cout << "\n'���ϸ�'�� ��ü�� �� �����ϴ�." << std::endl;
            continue;

        case 4: // ����ģ��
            std::cout << "\n������ �����ƴ�!" << std::endl;
            break;

        default:
            std::cout << "\n�߸��� �Է��Դϴ�. 1~4 ������ ���ڸ� �Է����ּ���." << std::endl;
            continue;
        }
        break;
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

Move* BattleManager::SelectMove()
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
