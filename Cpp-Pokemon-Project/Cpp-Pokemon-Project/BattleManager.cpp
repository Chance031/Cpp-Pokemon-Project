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
            std::cout << "\n'�ο��'�� �����߽��ϴ�!" << std::endl;
            // TODO: ��� ���� ���� ȣ��
            break;

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