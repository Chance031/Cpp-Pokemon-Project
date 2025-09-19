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
	// --- 생성자 ---
	BattleManager(std::vector<Pokemon>& playerParty, std::vector<Pokemon>& opponentParty);

	// --- 공개 인터페이스 ---	
	void Start(std::vector<TurnEvent>& events);																						// 전투 시작 시 필요한 초기 설정을 수행하고, 시작 이벤트를 생성
	TurnResult ProcessTurn(const BattleAction& playerAction);																		// 플레이어의 행동을 받아 한 턴 전체를 진행하고, 그 결과를 TurnResult로 반환
	void ExecuteSwitch(Pokemon*& activePokemon, std::vector<Pokemon>& party, int newPokemonIndex, std::vector<TurnEvent>& events);	// 포켓몬 교체를 실행하고 관련 이벤트를 기록

	// --- 정보 조회 (Getters) ---
	Pokemon* GetPlayerActivePokemon() const { return playerActivePokemon_; }
	Pokemon* GetOpponentActivePokemon() const { return opponentActivePokemon_; }
	bool IsBattleOver();

private:
	// --- 내부 로직 ---
	BattleAction SelectOpponentAction();
	void ExecuteAction(Pokemon* attacker, Pokemon* target, Move* move, std::vector<TurnEvent>& events);
	bool HandleMoveAccuracy(Pokemon* attacker, const Move* move);
	void ApplyMoveEffect(Pokemon* attacker, Pokemon* target, const Move* move, std::vector<TurnEvent>& events);
	DamageResult CalculateAndApplyDamage(Pokemon* attacker, Pokemon* target, const Move* move, std::vector<TurnEvent>& events);
	std::pair<Pokemon*, Move*> DetermineActionOrder(const BattleAction& playerAction, const BattleAction& opponentAction);
	void ProcessEndOfTurnEffects(Pokemon* pokemon, std::vector<TurnEvent>& events);
	void TriggerSwitchInAbilities(Pokemon* switchedInPokemon, Pokemon* opponent, std::vector<TurnEvent>& events);

	// --- 멤버 변수 ---
	std::vector<Pokemon>& playerParty_;
	std::vector<Pokemon>& opponentParty_;
	Pokemon* playerActivePokemon_ = nullptr;
	Pokemon* opponentActivePokemon_ = nullptr;
	std::mt19937 randomNumberEngine_;
};