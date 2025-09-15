#pragma once

#include <vector>

#include "Pokemon.h"

class BattleManager
{
public:
	// ������
	BattleManager(std::vector<Pokemon>& playerParty, std::vector<Pokemon>& opponentParty);

	void Start();	// ���� ���� ����

private:
	void PlayIntroSequence();

	std::vector<Pokemon>& playerParty_;
	std::vector<Pokemon>& opponentParty_;

	// ���� �ʵ忡 ���� �ִ� ���ϸ��� ����Ű�� '������'
	Pokemon* playerActivePokemon_ = nullptr;
	Pokemon* opponentActivePokemon_ = nullptr;
};

