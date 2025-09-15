#pragma once

#include <vector>

#include "Pokemon.h"

class Move;

class BattleManager
{
public:
	// ������
	BattleManager(std::vector<Pokemon>& playerParty, std::vector<Pokemon>& opponentParty);

	void Start();	// ���� ���� ����

private:
	void PlayIntroSequence();
	void ShowMainMenu();
	Move* SelectMove();

	std::vector<Pokemon>& playerParty_;
	std::vector<Pokemon>& opponentParty_;

	// ���� �ʵ忡 ���� �ִ� ���ϸ��� ����Ű�� '������'
	Pokemon* playerActivePokemon_ = nullptr;
	Pokemon* opponentActivePokemon_ = nullptr;
};

