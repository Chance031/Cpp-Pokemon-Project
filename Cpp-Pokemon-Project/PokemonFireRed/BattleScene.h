#pragma once

#include <queue>

#include "BattleManager.h"
#include "Pokemon.h"
#include "Scene.h"

// 전투 장면의 시각적/입력 처리 상태를 관리하기 위한 열거형
enum class BATTLE_UI_STATE
{
	EVENT_PROCESSING,    // 이벤트 큐에 있는 이벤트를 처리 중인 상태
	AWAITING_INPUT,      // 플레이어의 주 메뉴 입력을 기다리는 상태
	FIGHT_MENU,          // 기술 선택 메뉴 입력을 기다리는 상태
	BATTLE_END           // 전투 종료 상태
};

class BattleScene : public Scene
{
public:
	// --- 생성자 및 소멸자 ---
	BattleScene(HWND hWnd, int clientWidth, int clientHeight);
	~BattleScene() override;

	// Scene의 가상 함수들을 구체적으로 구현
	bool Init() override;
	void Update(float deltaTime) override;
	void Render(Gdiplus::Graphics& bufferGraphics) override;
	void Release() override;
	LRESULT MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;
	
private:
	// --- 연출 및 이벤트 처리 시스템 ---
	void ProcessNextEvent();					// 이벤트 큐에서 다음 이벤트를 꺼내 처리를 시작합니다.
	void UpdateTextMessage(float deltaTime);	// 메시지 출력 연출을 업데이트합니다.
	void UpdateHpAnimation(float deltaTime);	// HP 바 감소 애니메이션을 업데이트합니다.

	// --- 렌더링 도우미 함수 ---
	void RenderUI(Gdiplus::Graphics& g);
	void RenderText(Gdiplus::Graphics& g);
	void RenderHpBars(Gdiplus::Graphics& g);
	void RenderPokemonSprites(Gdiplus::Graphics& g);
	void RenderFightMenu(Gdiplus::Graphics& g);
	void RenderMainMenu(Gdiplus::Graphics& g);

	// --- 내부 상태 계산 ---
	void CalculateGameScreenRect();

	// --- 멤버 변수 ---
	HWND m_hWnd;
	int m_clientWidth;
	int m_clientHeight;

	// --- 게임 로직 객체 ---
	BattleManager* m_pBattleManager = nullptr;
	std::vector<Pokemon> m_playerParty;
	std::vector<Pokemon> m_opponentParty;

	// --- UI 및 상태 ---
	BATTLE_UI_STATE m_currentState = BATTLE_UI_STATE::EVENT_PROCESSING;
	Gdiplus::RectF m_gameScreenRect;
	Gdiplus::Bitmap* m_pBackground = nullptr;
	Gdiplus::Bitmap* m_pBattleUI = nullptr;
	int m_mainMenuSelection = 0;
	int m_fightMenuSelection = 0;

	// --- 이벤트 처리 시스템 ---
	std::queue<TurnEvent> m_eventQueue;		// 발생한 이벤트를 순서대로 저장하는 큐
	TurnEvent m_currentEvent;				// 현재 처리(연출)하고 있는 이벤트
	bool m_isProcessingEvent = false;		// 이벤트 처리 중인지 여부를 나타내는 플래그

	// --- 텍스트 연출용 변수 ---
	std::wstring m_fullMessageToShow;		// 메시지 박스에 표시할 전체 메시지
	std::wstring m_visibleMessage;			// 현재 화면에 보이는 부분 메시지
	size_t m_messageCharIndex = 0;			// 타이핑 효과를 위한 현재 글자 인덱스
	float m_textAnimationTimer = 0.0f;		// 글자 타이핑 속도 조절 타이머
	bool m_isMessageFinished = false;		// 현재 메시지 출력이 완료되었는지 여부

	// --- HP 애니메이션용 변수 ---
	float m_playerVisualHp = 0;				// 화면에 '보여지는' 플레이어 포켓몬의 HP
	float m_opponentVisualHp = 0;			// 화면에 '보여지는' 상대 포켓몬의 HP
};