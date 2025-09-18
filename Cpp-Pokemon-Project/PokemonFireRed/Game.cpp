#include "Game.h"

#include "DataManager.h"
#include "StringUtils.h"

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

Game::Game()
{
}

Game::~Game()
{
}

bool Game::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	// GDI+ �ʱ�ȭ
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	// ���� ������ �ε�
	DataManager::GetInstance().LoadAllData();

	// ���ҽ� �ε�
	m_pBuffer = new Bitmap(m_clientWidth, m_clientHeight);
	m_pIntroSheet = new Bitmap(L"../assets/images/ui/intro_sheet.png");
	//m_pBattleBackground = new Bitmap(L"assets/images/backgrounds/grass_battle.png");
	//m_pUiSheet = new Bitmap(L"assets/images/ui/battle_ui.png");

	// ���ҽ� �ε� ���� Ȯ��
	/*if (m_pIntroSheet->GetLastStatus() != Ok || m_pBuffer->GetLastStatus() != Ok || m_pBattleBackground->GetLastStatus() != Ok || m_pUiSheet->GetLastStatus() != Ok)
	{
		MessageBox(m_hWnd, L"�ʼ� �̹��� ���ҽ��� �ҷ����� �� �����߽��ϴ�.", L"���ҽ� ����", MB_OK);
		return false;
	}*/
	if (m_pIntroSheet->GetLastStatus() != Ok || m_pBuffer->GetLastStatus() != Ok)
	{
		MessageBox(m_hWnd, L"�ʼ� �̹��� ���ҽ��� �ҷ����� �� �����߽��ϴ�.", L"���ҽ� ����", MB_OK);
		return false;
	}

	return true;
}

void Game::Run()
{
	MSG msg = {};
	ULONGLONG lastTime = GetTickCount64();

	while (true) // 1. ���� ������ ���ϴ�.
	{
		// 2. ó���� ������ �޽����� �ִ��� 'Ȯ�θ� �ϰ�' �ٷ� �Ѿ�ϴ� (PeekMessage).
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break; // ���� �޽����� ������ ������ Ż���մϴ�.

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else // 3. ó���� �޽����� ���ٸ�, ���� ������ �����մϴ�.
		{
			ULONGLONG currentTime = GetTickCount64();
			float deltaTime = (currentTime - lastTime) / 1000.0f;
			lastTime = currentTime;

			Update(deltaTime); // ���� ���¸� ������Ʈ�ϰ�
			InvalidateRect(m_hWnd, NULL, FALSE); // ȭ���� �ٽ� �׸���� ��û�մϴ�.
		}
	}
}

void Game::Release()
{
	// �������� �Ҵ�� ��� ���ҽ��� �����մϴ�.
	delete m_pIntroSheet;
	delete m_pBuffer;
	delete m_pBattleBackground;
	delete m_pOpponentSprite;
	delete m_pPlayerSprite;
	delete m_pUiSheet;
	delete m_pBattleManager;

	// GDI+ ����
	GdiplusShutdown(m_gdiplusToken);
}

LRESULT Game::MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_SIZE:
	{
		m_clientWidth = LOWORD(lParam);
		m_clientHeight = HIWORD(lParam);
		delete m_pBuffer;
		m_pBuffer = new Bitmap(m_clientWidth, m_clientHeight);
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		Render();
		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_KEYDOWN:
	{
		if (m_gameState == GameState::TitleScreen && wParam == VK_RETURN)
		{
			StartBattle();
		}
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

void Game::Update(float deltaTime)
{
	// ���̵� ���� ó��
	if (m_fadeState != FadeState::None)
	{
		m_fadeTimer += deltaTime;
		float fadeDuration = (m_fadeState == FadeState::FadingIn) ? 0.7f : 1.0f;

		if (m_fadeState == FadeState::FadingIn) 
		{
			m_fadeAlpha = 255 - static_cast<int>((m_fadeTimer / fadeDuration) * 255.0f);
			if (m_fadeAlpha <= 0) 
			{
				m_fadeAlpha = 0;
				m_fadeState = FadeState::None;
				m_gameState = m_nextGameState;
				m_sceneTimer = 0.0f;
			}
		}
		else 
		{ // FadingOut
			m_fadeAlpha = static_cast<int>((m_fadeTimer / fadeDuration) * 255.0f);
			if (m_fadeAlpha >= 255) 
			{
				m_fadeAlpha = 255;
				m_fadeState = FadeState::None;    // [����] ���̵� ���� (�ٽ� ���̵��� ���� ����)
				m_gameState = m_nextGameState;  // [����] �ٷ� ���� ������� ��ȯ
				m_sceneTimer = 0.0f;
			}
		}
	}
	// �Ϲ� ���� ���� ������Ʈ (���̵� ���� �ƴ� ��)
	else
	{
		m_sceneTimer += deltaTime;
		switch (m_gameState)
		{
		case GameState::Intro_GameFreak:
			if (m_sceneTimer > 2.0f) 
			{
				m_nextGameState = GameState::Intro_BattleAnim;
				m_fadeState = FadeState::FadingOut; // ���̵�ƿ� ����
				m_fadeTimer = 0.0f;
			}
			break;
		case GameState::Intro_BattleAnim:
			if (m_sceneTimer > 5.0f) 
			{
				m_nextGameState = GameState::TitleScreen;
				m_fadeState = FadeState::FadingOut; // ���̵�ƿ� ����
				m_fadeTimer = 0.0f;
			}
			break;
		case GameState::TitleScreen:
			// �����̴� �ؽ�Ʈ ������ Render���� ó��
			break;
		case GameState::InBattle:
			// TODO: ���� ���� ������Ʈ
			break;
		}
	}
}

void Game::Render()
{
	Graphics bufferGraphics(m_pBuffer);
	bufferGraphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	bufferGraphics.SetPixelOffsetMode(PixelOffsetModeHalf);
	bufferGraphics.Clear(Color(0, 0, 0));

	float originalWidth = 240.0f, originalHeight = 160.0f;
	float originalAspectRatio = originalWidth / originalHeight;
	float windowWidth = static_cast<float>(m_clientWidth), windowHeight = static_cast<float>(m_clientHeight);
	float destWidth = windowWidth, destHeight = windowWidth / originalAspectRatio;
	if (destHeight > windowHeight) {
		destHeight = windowHeight;
		destWidth = windowHeight * originalAspectRatio;
	}
	float destX = (windowWidth - destWidth) / 2.0f;
	float destY = (windowHeight - destHeight) / 2.0f;
	RectF gameScreenRect(destX, destY, destWidth, destHeight);

	GameState stateToRender = (m_fadeState == FadeState::FadingIn) ? m_nextGameState : m_gameState;

	switch (stateToRender)
	{
	case GameState::Intro_GameFreak:
		bufferGraphics.DrawImage(m_pIntroSheet, gameScreenRect, 8, 42, 240, 160, UnitPixel);
		break;
	case GameState::Intro_BattleAnim:
		bufferGraphics.DrawImage(m_pIntroSheet, gameScreenRect, 8, 203, 240, 160, UnitPixel);
		break;
	case GameState::TitleScreen:
		bufferGraphics.DrawImage(m_pIntroSheet, gameScreenRect, 251, 0, 240, 160, UnitPixel);
		// Title �ΰ�� ���� ������ ȭ�� ���� �ٽ� �׸�
		break;
	case GameState::InBattle:
		// ���� ȭ�� �׸���
		break;
	}

	// ���̵� ȿ�� ���׸���
	if (m_fadeState != FadeState::None) {
		Color fadeColor = (m_fadeState == FadeState::FadingIn) ? Color(m_fadeAlpha, 255, 255, 255) : Color(m_fadeAlpha, 0, 0, 0);
		SolidBrush fadeBrush(fadeColor);
		bufferGraphics.FillRectangle(&fadeBrush, 0, 0, m_clientWidth, m_clientHeight);
	}

	Graphics screenGraphics(m_hWnd);
	screenGraphics.DrawImage(m_pBuffer, 0, 0, m_clientWidth, m_clientHeight);
}

void Game::StartBattle()
{
	m_playerParty.clear();
	m_wildParty.clear();
	delete m_pBattleManager;

	PokemonIndividual bulbasaurIndividual;
	bulbasaurIndividual.level = 5;
	Pokemon bulbasaur(DataManager::GetInstance().GetPokemonSpecies(1), bulbasaurIndividual);
	m_playerParty.push_back(bulbasaur);

	PokemonIndividual arbokIndividual;
	arbokIndividual.level = 5;
	Pokemon arbok(DataManager::GetInstance().GetPokemonSpecies(24), arbokIndividual);
	m_wildParty.push_back(arbok);

	m_pBattleManager = new BattleManager(m_playerParty, m_wildParty);
	m_nextGameState = GameState::InBattle;
	m_fadeState = FadeState::FadingOut; // ���� ���� �� ���̵�ƿ�
	m_fadeTimer = 0.0f;
}
