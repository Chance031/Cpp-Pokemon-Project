#include "BattleManager.h"

#include <iostream>

BattleManager::BattleManager(std::vector<Pokemon>& playerParty, std::vector<Pokemon>& opponentParty)
	: playerParty_(playerParty), opponentParty_(opponentParty)
{

}

void BattleManager::Start()
{
	std::cout << "--- ���� ����! ---" << std::endl;

	playerActivePokemon_ = &playerParty_[0];
	opponentActivePokemon_ = &opponentParty_[0];

	PlayIntroSequence();
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
