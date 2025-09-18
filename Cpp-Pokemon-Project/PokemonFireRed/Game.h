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

class Game
{
public:
	Game();
	~Game();

	bool Init(HWND hWnd); // �ʱ�ȭ
	void Run();           // ���� ���� ����
	void Release();       // ���ҽ� ����

	LRESULT MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);	// ������ �޽��� ó��

private:
	void Update(float deltaTime);
	void Render();
	void StartBattle();

	// --- ��� ���� ---
	HWND m_hWnd = NULL;	// ������ �ڵ�

	// GDI+ ����
	ULONG_PTR m_gdiplusToken = 0;
	Gdiplus::Bitmap* m_pBuffer = nullptr;	// ���� ���۸���

	// ���ҽ�
	Gdiplus::Bitmap* m_pIntroSheet = nullptr;
	Gdiplus::Bitmap* m_pBattleBackground = nullptr;
	Gdiplus::Bitmap* m_pOpponentSprite = nullptr;
	Gdiplus::Bitmap* m_pPlayerSprite = nullptr;
	Gdiplus::Bitmap* m_pUiSheet = nullptr;

	// ���� ����
	GameState m_gameState = GameState::FadingIn;
	GameState m_nextGameState = GameState::Intro_GameFreak;
	BattleManager* m_pBattleManager = nullptr;
	float m_sceneTimer = 0.0f;

	// ���̵� ȿ��
	bool m_isFadingOut = false;
	int m_fadeAlpha = 255;
	bool m_showBlinkingText = true;

	// ���ϸ� ��Ƽ
	std::vector<Pokemon> m_playerParty;
	std::vector<Pokemon> m_wildParty;
};