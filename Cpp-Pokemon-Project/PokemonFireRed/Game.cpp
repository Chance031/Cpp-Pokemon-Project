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

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			ULONGLONG currentTime = GetTickCount64();
			float deltaTime = (currentTime - lastTime) / 1000.0f;
			lastTime = currentTime;

			Update(deltaTime);
			InvalidateRect(m_hWnd, NULL, FALSE);
		}
	}
}

void Game::Release()
{
	// �������� �Ҵ�� ��� ���ҽ��� ����
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
		Render(hdc);
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
		float fadeDuration = (m_fadeState == FadeState::FadingIn) ? 0.2f : 1.0f;

		if (m_fadeState == FadeState::FadingIn) 
		{
			m_fadeAlpha = 255 - static_cast<int>((m_fadeTimer / fadeDuration) * 255.0f);
			if (m_fadeAlpha <= 0) 
			{
				m_fadeAlpha = 0;
				m_fadeState = FadeState::None;
				m_gameState = m_nextGameState;
				m_sceneTimer = 0.0f;
				
				// Intro_BattleAnim ������� ��ȯ�� ���� ���� �ִϸ��̼� ����
				if (m_gameState == GameState::Intro_BattleAnim) 
				{
					m_isRevealing = true;
					m_revealTimer = 0.0f;
				}
			}
		}
		else 
		{ // FadingOut
			m_fadeAlpha = static_cast<int>((m_fadeTimer / fadeDuration) * 255.0f);
			if (m_fadeAlpha >= 255) 
			{
				m_fadeAlpha = 255;
				m_nextGameState = (m_gameState == GameState::Intro_GameFreak) ? GameState::Intro_BattleAnim : GameState::TitleScreen;
				m_gameState = m_nextGameState; // [����] ���̵�ƿ� �� �ٷ� ���� ������� ��ȯ (���� ȭ�� ����)
				m_fadeState = FadeState::None;
				m_sceneTimer = 0.0f;

				// [����] ���� ����� ���� �ִϸ��̼��� ���� ���� ����
				if (m_nextGameState == GameState::Intro_BattleAnim)
				{
					m_isRevealing = true;
					m_revealTimer = 0.0f;
				}

			}
		}
	}
	// â�� ������ �ִϸ��̼� ����
	else if (m_isRevealing)
	{
		m_revealTimer += deltaTime;
		float revealDuration = 0.5f; // [����] �ӵ��� 0.5�ʷ� ������ ����

		if (m_revealTimer >= revealDuration) {
			m_isRevealing = false; // [����] �� ���� ����ǵ��� �ִϸ��̼� ����
		}
		else
		{
			m_revealOffset = (m_revealTimer / revealDuration) * (m_clientHeight / 2.0f);
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
				// [�߰�] �� ��ũ�� ��ġ ������Ʈ
				m_starScrollX -= 50.0f * deltaTime; // 1�ʿ� 50�ȼ��� �������� �̵� (�ӵ� ���� ����)

				// �� ������ ���� ũ�� (ȭ�� ������ ���� ���)
				float starPatternWidth = 42.0f * (gameScreenRect.Width / 240.0f);
				if (m_starScrollX < -starPatternWidth) 
				{
					m_starScrollX += starPatternWidth; // ������ ȭ�� ������ ������ �ٽ� ����ġ
				}
				m_fadeState = FadeState::FadingOut;
				m_fadeTimer = 0.0f;
			}
			break;
		case GameState::Intro_BattleAnim:
			if (m_sceneTimer > 5.0f) {
				m_nextGameState = GameState::TitleScreen;
				m_fadeState = FadeState::FadingOut;
				m_fadeTimer = 0.0f;
			}
			break;
		case GameState::TitleScreen:
			// �����̴� �ؽ�Ʈ ����
			break;
		case GameState::InBattle:
			// ���� ����
			break;
		}
	}
}

void Game::Render(HDC hdc)
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

	// 1. ���� ���¿� �´� ��� �׸���
	// ���̵���/���� �߿��� ������ ���� ����� �̸� �׷��ݴϴ�.
	GameState stateToRender = m_gameState;
	if (m_fadeState == FadeState::FadingIn || m_isRevealing) {
		stateToRender = m_nextGameState;
	}
	// ���� ȿ���� Intro_BattleAnim ���� �Ͼ�Ƿ�, nextGameState�� Intro_BattleAnim�� ��
	// Intro_BattleAnim�� �̸� �׷��ֵ��� �����մϴ�.
	if (m_gameState == GameState::Intro_GameFreak && m_fadeState == FadeState::FadingOut)
	{
		stateToRender = GameState::Intro_GameFreak;
	}

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

	// 2. [�߰�] �� ��ũ�� ȿ�� �׸���
	if (m_pIntroSheet)
	{
		// ��������Ʈ ��Ʈ���� �� �̹����� ����
		float starSourceX = 515.0f;
		float starSourceY = 177.0f;
		float starSourceWidth = 42.0f;
		float starSourceHeight = 14.0f;

		// ȭ�� ������ ���� ���� �׷��� ũ��� Y ��ġ ���
		float scale = gameScreenRect.Width / 240.0f; // ���� ��� ȭ�� Ȯ�� ����
		float renderStarHeight = starSourceHeight * scale;
		float renderStarY = gameScreenRect.Y + (42.0f * scale); // �ΰ� ����� Y��ǥ(42)�� ���� ��ġ

		// �� ������ ȭ��� �ʺ�
		float renderStarPatternWidth = starSourceWidth * scale;

		// ȭ���� �� ä��� ������ ���� ���� �� �׷��� ���� ���� ��ũ�� ȿ�� ����
		for (int i = -1; i < (m_clientWidth / renderStarPatternWidth) + 2; ++i)
		{
			RectF starDestRect(
				gameScreenRect.X + m_starScrollX + (i * renderStarPatternWidth),
				renderStarY,
				renderStarPatternWidth,
				renderStarHeight
			);

			// DrawImage�� �� �κи� �߶󳻾� �׸��ϴ�.
			bufferGraphics.DrawImage(m_pIntroSheet, starDestRect,
				starSourceX, starSourceY,
				starSourceWidth, starSourceHeight, UnitPixel);
		}
	}

	// 2. ��� ���� ������ ���� ���� �׸���
	if (m_isRevealing)
	{
		SolidBrush blackBrush(Color(255, 0, 0, 0));
		// ���� ����
		RectF topRect(0.0f, 0.0f, (float)m_clientWidth, (m_clientHeight / 2.0f) - m_revealOffset);
		bufferGraphics.FillRectangle(&blackBrush, topRect);
		// �Ʒ��� ����
		RectF bottomRect(0.0f, (m_clientHeight / 2.0f) + m_revealOffset, (float)m_clientWidth, (m_clientHeight / 2.0f));
		bufferGraphics.FillRectangle(&blackBrush, bottomRect);
	}

	// ���̵� ȿ�� ���׸���
	if (m_fadeState != FadeState::None) {
		Color fadeColor = (m_fadeState == FadeState::FadingIn) ? Color(m_fadeAlpha, 255, 255, 255) : Color(m_fadeAlpha, 0, 0, 0);
		SolidBrush fadeBrush(fadeColor);
		bufferGraphics.FillRectangle(&fadeBrush, 0, 0, m_clientWidth, m_clientHeight);
	}

	Graphics screenGraphics(hdc);
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
