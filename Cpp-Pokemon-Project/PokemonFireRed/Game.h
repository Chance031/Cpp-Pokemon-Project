#pragma once

#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>

#include "BattleManager.h"
#include "GameTypes.h"

namespace Gdiplus
{
	class Bitmap;
}

// 페이드 상태를 관리하기 위한 enum
enum class FadeState
{
	None,
	FadingOut,
	FadingIn
};

class Game
{
public:
	Game();
	~Game();

	bool Init(HWND hWnd); // 초기화
	void Run();           // 메인 게임 루프
	void Release();       // 리소스 해제

	LRESULT MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);	// 윈도우 메시지 처리

private:
	void Update(float deltaTime);
	void Render();
	void StartBattle();

	// --- 멤버 변수 ---
	HWND m_hWnd = NULL;	// 윈도우 핸들
	int m_clientWidth = 800;
	int m_clientHeight = 600;

	// GDI+ 관련
	ULONG_PTR m_gdiplusToken = 0;
	Gdiplus::Bitmap* m_pBuffer = nullptr;	// 더블 버퍼링용

	// 리소스
	Gdiplus::Bitmap* m_pIntroSheet = nullptr;
	Gdiplus::Bitmap* m_pBattleBackground = nullptr;
	Gdiplus::Bitmap* m_pOpponentSprite = nullptr;
	Gdiplus::Bitmap* m_pPlayerSprite = nullptr;
	Gdiplus::Bitmap* m_pUiSheet = nullptr;

	// 게임 상태
	GameState m_gameState = GameState::Intro_GameFreak;
	GameState m_nextGameState = GameState::Intro_GameFreak;

	BattleManager* m_pBattleManager = nullptr;
	float m_sceneTimer = 0.0f;
    
	// 페이드 효과
	FadeState m_fadeState = FadeState::FadingIn; // 게임 시작 시 페이드인
	float m_fadeTimer = 0.0f;
	int m_fadeAlpha = 255;

	// 포켓몬 파티
	std::vector<Pokemon> m_playerParty;
	std::vector<Pokemon> m_wildParty;
};