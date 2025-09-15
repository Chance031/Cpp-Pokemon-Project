#pragma once

#include <vector>

#include "Pokemon.h"

class Move;

class BattleManager
{
public:
	// 생성자
	BattleManager(std::vector<Pokemon>& playerParty, std::vector<Pokemon>& opponentParty);

	void Start();	// 전투 시작 로직

private:
	void PlayIntroSequence();
	void ShowMainMenu();
	Move* SelectMove();

	std::vector<Pokemon>& playerParty_;
	std::vector<Pokemon>& opponentParty_;

	// 현재 필드에 나와 있는 포켓몬을 가리키는 '포인터'
	Pokemon* playerActivePokemon_ = nullptr;
	Pokemon* opponentActivePokemon_ = nullptr;
};

