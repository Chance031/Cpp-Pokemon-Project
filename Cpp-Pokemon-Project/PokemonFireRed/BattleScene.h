#pragma once
#include "Scene.h"
#include "Pokemon.h"
#include "BattleManager.h"
#include <vector>

// 전투 장면의 시각적/입력 처리 상태를 관리하기 위한 열거형
enum class BATTLE_UI_STATE
{
	INTRO_SEQUENCE,      // 전투 시작 인트로 (메시지 출력 등)
	MAIN_MENU,           // 주 메뉴 표시 (싸운다, 가방, 포켓몬, 도망)
	FIGHT_MENU,          // 기술 선택 메뉴 표시
	PROCESSING_TURN,     // 턴 처리 중 (애니메이션, 메시지 출력)
	SWITCH_POKEMON_MENU, // 포켓몬 교체 메뉴 표시
	BATTLE_END           // 전투 종료
};

class BattleScene : public Scene
{
public:
	BattleScene(HWND hWnd, int clientWidth, int clientHeight);
	~BattleScene() override;

	// Scene의 가상 함수들을 구체적으로 구현
	bool Init() override;
	void Update(float deltaTime) override;
	void Render(Gdiplus::Graphics& bufferGraphics) override;
	void Release() override;
	LRESULT MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;
	
private:
	// --- 멤버 변수 ---
	HWND m_hWnd;
	int m_clientWidth;
	int m_clientHeight;

	// --- 게임 로직 객체 ---
	BattleManager* m_pBattleManager = nullptr;
	std::vector<Pokemon> m_playerParty;
	std::vector<Pokemon> m_opponentParty;

	// --- UI 및 상태 ---
	BATTLE_UI_STATE m_currentState = BATTLE_UI_STATE::INTRO_SEQUENCE;
	Gdiplus::Bitmap* m_pBackground = nullptr;
	Gdiplus::Bitmap* m_pBattleUI = nullptr;

	// 메뉴 선택을 위한 변수
	int m_mainMenuSelection = 0; // 0: 싸운다, 1: 가방, 2: 포켓몬, 3: 도망
	int m_fightMenuSelection = 0; // 0-3: 기술 선택

	// 화면 비율 유지를 위한 Rect
	Gdiplus::RectF m_gameScreenRect;
	void CalculateGameScreenRect();
};