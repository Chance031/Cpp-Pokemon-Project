#pragma once

#include <vector>

#include "BattleAction.h"
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

	BattleAction SelectPlayerAction();
	BattleAction SelectOpponentAction();
	const Move* SelectMove();

	void ProcessTurn(const BattleAction& playerAction, const BattleAction& opponentAction);
	void ExecuteMove(Pokemon* attacker, Pokemon* target, const Move* move);
	bool IsBattleOver();

	std::vector<Pokemon>& playerParty_;
	std::vector<Pokemon>& opponentParty_;

	// ���� �ʵ忡 ���� �ִ� ���ϸ��� ����Ű�� '������'
	Pokemon* playerActivePokemon_ = nullptr;
	Pokemon* opponentActivePokemon_ = nullptr;
};

