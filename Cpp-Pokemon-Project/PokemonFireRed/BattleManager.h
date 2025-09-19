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
	// --- ������ ---
	BattleManager(std::vector<Pokemon>& playerParty, std::vector<Pokemon>& opponentParty);

	// --- ���� �������̽� ---	
	void Start(std::vector<TurnEvent>& events);																						// ���� ���� �� �ʿ��� �ʱ� ������ �����ϰ�, ���� �̺�Ʈ�� ����
	TurnResult ProcessTurn(const BattleAction& playerAction);																		// �÷��̾��� �ൿ�� �޾� �� �� ��ü�� �����ϰ�, �� ����� TurnResult�� ��ȯ
	void ExecuteSwitch(Pokemon*& activePokemon, std::vector<Pokemon>& party, int newPokemonIndex, std::vector<TurnEvent>& events);	// ���ϸ� ��ü�� �����ϰ� ���� �̺�Ʈ�� ���

	// --- ���� ��ȸ (Getters) ---
	Pokemon* GetPlayerActivePokemon() const { return playerActivePokemon_; }
	Pokemon* GetOpponentActivePokemon() const { return opponentActivePokemon_; }
	bool IsBattleOver();

private:
	// --- ���� ���� ---
	BattleAction SelectOpponentAction();
	void ExecuteAction(Pokemon* attacker, Pokemon* target, Move* move, std::vector<TurnEvent>& events);
	bool HandleMoveAccuracy(Pokemon* attacker, const Move* move);
	void ApplyMoveEffect(Pokemon* attacker, Pokemon* target, const Move* move, std::vector<TurnEvent>& events);
	DamageResult CalculateAndApplyDamage(Pokemon* attacker, Pokemon* target, const Move* move, std::vector<TurnEvent>& events);
	std::pair<Pokemon*, Move*> DetermineActionOrder(const BattleAction& playerAction, const BattleAction& opponentAction);
	void ProcessEndOfTurnEffects(Pokemon* pokemon, std::vector<TurnEvent>& events);
	void TriggerSwitchInAbilities(Pokemon* switchedInPokemon, Pokemon* opponent, std::vector<TurnEvent>& events);

	// --- ��� ���� ---
	std::vector<Pokemon>& playerParty_;
	std::vector<Pokemon>& opponentParty_;
	Pokemon* playerActivePokemon_ = nullptr;
	Pokemon* opponentActivePokemon_ = nullptr;
	std::mt19937 randomNumberEngine_;
};