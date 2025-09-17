#pragma once

#include <random>
#include <vector>
#include <string>
#include <utility>

#include "BattleTypes.h"
#include "Pokemon.h"

class BattleManager
{
public:
	BattleManager(std::vector<Pokemon>& playerParty, std::vector<Pokemon>& opponentParty);
	void Start();

private:
	// 전투 흐름 관리
	void PlayIntroSequence();
	void ProcessTurn(const BattleAction& playerAction, const BattleAction& opponentAction);
	bool IsBattleOver();

	// 사용자 입력 및 AI
	BattleAction SelectPlayerAction();
	BattleAction SelectOpponentAction();
	Move* SelectMove();
	void ShowMainMenu();

	// 핵심 실행 로직
	void ExecuteAction(Pokemon* attacker, Pokemon* target, Move* move);
	bool HandleMoveAccuracy(Pokemon* attacker, const Move* move);
	void ApplyMoveEffect(Pokemon* attacker, Pokemon* target, const Move* move);
	DamageResult CalculateAndApplyDamage(Pokemon* attacker, Pokemon* target, const Move* move);
	std::pair<TurnAction, TurnAction> DetermineActionOrder(const BattleAction& playerAction, const BattleAction& opponentAction);
	void ProcessEndOfTurnEffects(Pokemon* pokemon);
	void TriggerSwitchInAbilities(Pokemon* switchedInPokemon);
	int SelectPokemonToSwitch(); // 교체할 포켓몬 선택 메뉴 (파티 인덱스 반환)
	void ExecuteSwitch(Pokemon*& activePokemon, std::vector<Pokemon>& party, int newPokemonIndex);

	// 멤버 변수
	std::vector<Pokemon> playerParty_;
	std::vector<Pokemon> opponentParty_;
	Pokemon* playerActivePokemon_ = nullptr;
	Pokemon* opponentActivePokemon_ = nullptr;

	std::mt19937 randomNumberEngine_;
};

