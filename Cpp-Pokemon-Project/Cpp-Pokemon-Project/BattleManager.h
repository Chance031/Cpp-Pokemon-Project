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
	// ���� �帧 ����
	void PlayIntroSequence();
	void ProcessTurn(const BattleAction& playerAction, const BattleAction& opponentAction);
	bool IsBattleOver();

	// ����� �Է� �� AI
	BattleAction SelectPlayerAction();
	BattleAction SelectOpponentAction();
	Move* SelectMove();
	void ShowMainMenu();

	// �ٽ� ���� ����
	void ExecuteAction(Pokemon* attacker, Pokemon* target, Move* move);
	bool HandleMoveAccuracy(Pokemon* attacker, const Move* move);
	void ApplyMoveEffect(Pokemon* attacker, Pokemon* target, const Move* move);
	DamageResult CalculateAndApplyDamage(Pokemon* attacker, Pokemon* target, const Move* move);
	std::pair<TurnAction, TurnAction> DetermineActionOrder(const BattleAction& playerAction, const BattleAction& opponentAction);
	void ProcessEndOfTurnEffects(Pokemon* pokemon);
	void TriggerSwitchInAbilities(Pokemon* switchedInPokemon);
	int SelectPokemonToSwitch(); // ��ü�� ���ϸ� ���� �޴� (��Ƽ �ε��� ��ȯ)
	void ExecuteSwitch(Pokemon*& activePokemon, std::vector<Pokemon>& party, int newPokemonIndex);

	// ��� ����
	std::vector<Pokemon> playerParty_;
	std::vector<Pokemon> opponentParty_;
	Pokemon* playerActivePokemon_ = nullptr;
	Pokemon* opponentActivePokemon_ = nullptr;

	std::mt19937 randomNumberEngine_;
};

