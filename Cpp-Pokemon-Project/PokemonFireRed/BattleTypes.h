#pragma once

#include "Enums.h"

// 전방 선언
class Move;
class Pokemon;

// =================================================================
// 플레이어의 행동 선택 정보를 담는 구조체
// =================================================================

enum class PlayerActionType
{
	FIGHT,		// 싸우다
	BAG,		// 가방
	POKEMON,	// 포켓몬
	RUN,		// 도망치다
	CANCEL
};

struct BattleAction
{
	PlayerActionType type = PlayerActionType::CANCEL;	// 어떤 종류의 행동을 선택했는가
	Move* move = nullptr;								// '싸우다'를 선택했다면, 어떤 기술인가
	int switchPokemonIndex = -1;						// '포켓몬'을 선택했다면, 몇 번째 포켓몬으로 교체할 것인가
};

// =================================================================
// 턴 진행 결과(이벤트)를 BattleScene에 전달하기 위한 구조체
// =================================================================

enum class TurnEventType
{
	TEXT_MESSAGE,		// 단순 텍스트 메시지
	DAMAGE,				// 포켓몬이 데미지를 입는 이벤트
	FAINT,				// 포켓몬이 기절하는 이벤트
	STAT_CHANGE,		// 능력치 랭크 변화 이벤트
	STATUS_EFFECT,		// 상태 이상 발생 이벤트
};

struct TurnEvent
{
	TurnEventType type = TurnEventType::TEXT_MESSAGE;	// 이 이벤트의 종류
	Pokemon* target = nullptr;							// 이벤트의 대상이 되는 포켓몬
	std::string message = "";							// 화면에 표시될 데미지
	int damage = 0;										// 데미지 량
	Stat stat = Stat::HP;								// 변화한 능력치
	int stages = 0;										// 능력치 변화량
	StatusCondition status = StatusCondition::NONE;		// 적용된 상태 이상
};

struct TurnResult
{
	std::vector<TurnEvent> events;
};

// =================================================================
// 데미지 계산 결과를 담는 내부용 구조체
// =================================================================

struct DamageResult
{
	int damageDealt = 0;
	double typeEffectiveness = 1.0;
};