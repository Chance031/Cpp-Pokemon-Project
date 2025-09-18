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
	m_pIntroSheet = new Bitmap(L"../assets/images/ui/intro_sheet.png");
	m_pBuffer = new Bitmap(800, 600);
	//m_pBattleBackground = new Bitmap(L"../asset/images/backgrounds/grass_battle.png");
	//m_pUiSheet = new Bitmap(L"../assets/images/ui/battle_ui.png");

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

	m_nextGameState = GameState::Intro_GameFreak;

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
	case WM_PAINT:
	{
		// ���� �׸��� ������ Render() �Լ��� �̵�
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
	m_sceneTimer += deltaTime;

	switch (m_gameState)
	{
		// 1. ȭ���� ������� ����
	case GameState::FadingIn:
	{
		float fadeDuration = 0.2f; // 0.5�� ���� �����
		m_fadeAlpha = 255 - static_cast<int>((m_sceneTimer / fadeDuration) * 255.0f);

		if (m_fadeAlpha <= 0) {
			m_fadeAlpha = 0;
			m_gameState = m_nextGameState; // ���̵����� ������, ��ǥ�ߴ� ���� ������� ��ȯ
			m_sceneTimer = 0.0f;
		}
		break;
	}

	// 2. GameFreak �ΰ� ǥ�õǴ� ����
	case GameState::Intro_GameFreak:
	{
		// 2�ʰ� ����� ������ ��,
		if (m_sceneTimer > 2.0f) {
			m_nextGameState = GameState::Intro_BattleAnim; // ���� ����� '���� �ִϸ��̼�'���� �����ϰ�
			m_gameState = GameState::FadingOut;           // ���̵�ƿ� ����
			m_sceneTimer = 0.0f;
		}
		break;
	}

	// 3. ȭ���� ��ο����� ����
	case GameState::FadingOut:
	{
		float fadeDuration = 1.0f; // 1�� ���� ��ο���
		m_fadeAlpha = static_cast<int>((m_sceneTimer / fadeDuration) * 255.0f);

		if (m_fadeAlpha >= 255) {
			m_fadeAlpha = 255;
			m_gameState = GameState::FadingIn; // ���̵�ƿ��� ������, ���� ����� ���� ���̵��� ����
			m_sceneTimer = 0.0f;
		}
		break;
	}

	// 4. ���� �ִϸ��̼��� ǥ�õǴ� ����
	case GameState::Intro_BattleAnim:
	{
		// 5�ʰ� ����� ������ ��,
		if (m_sceneTimer > 5.0f) {
			m_nextGameState = GameState::TitleScreen; // ���� ����� 'Ÿ��Ʋ ȭ��'���� �����ϰ�
			m_gameState = GameState::FadingOut;     // ���̵�ƿ� ����
			m_sceneTimer = 0.0f;
		}
		break;
	}

	// 5. Ÿ��Ʋ ȭ���� ǥ�õǴ� ����
	case GameState::TitleScreen:
	{
		m_showBlinkingText = (static_cast<int>(m_sceneTimer * 2.0f) % 2 == 0);
		// �� ���¿����� Ű �Է��� ��ٸ��Ƿ� �ڵ����� ���� ���·� �Ѿ�� ����
		break;
	}

	case GameState::InBattle:
		// TODO: ���� ���� ������Ʈ
		break;
	}
}

void Game::Render()
{
	Graphics bufferGraphics(m_pBuffer);
	bufferGraphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	bufferGraphics.SetPixelOffsetMode(PixelOffsetModeHalf);
	bufferGraphics.Clear(Color(0, 0, 0));

	// ȭ�� ���� ���� ���
	float originalWidth = 240.0f;
	float originalHeight = 160.0f;
	float originalAspectRatio = originalWidth / originalHeight;
	float windowWidth = 800.0f;
	float windowHeight = 600.0f;
	float destHeight = windowWidth / originalAspectRatio;
	float destWidth = windowWidth;
	if (destHeight > windowHeight) {
		destHeight = windowHeight;
		destWidth = windowHeight * originalAspectRatio;
	}
	float destX = (windowWidth - destWidth) / 2.0f;
	float destY = (windowHeight - destHeight) / 2.0f;
	RectF gameScreenRect(destX, destY, destWidth, destHeight);

	GameState stateToRender = m_gameState;
	if (m_gameState == GameState::FadingIn) {
		stateToRender = m_nextGameState; // ������� ���� ��, ������ ���� ����� �̸� �׸�
	}

	// ���� ���¿� �´� ��� �׸���
	switch (m_gameState)
	{
	case GameState::Intro_GameFreak:
	{
		// [����] �˷��ֽ� ��Ȯ�� ��ǥ�� ��ü
		bufferGraphics.DrawImage(m_pIntroSheet, gameScreenRect, 8.0f, 42.0f, 245.0f, 161.5f, UnitPixel);
		break;
	}
	case GameState::Intro_BattleAnim:
	{
		// ��Ʈ�� ���� ��� �׸���
		bufferGraphics.DrawImage(m_pIntroSheet, gameScreenRect, 0, 0, 240, 160, UnitPixel);
		break;
	}
	case GameState::TitleScreen:
	{
		// Ÿ��Ʋ ȭ�� �׸���
		bufferGraphics.DrawImage(m_pIntroSheet, gameScreenRect, 251, 0, 240, 160, UnitPixel);

		float logoWidth = destWidth * 0.8f;
		float logoHeight = logoWidth / (480.0f / 288.0f);
		float logoX = destX + (destWidth - logoWidth) / 2.0f;
		float logoY = destY + destHeight * 0.1f;
		RectF logoRect(logoX, logoY, logoWidth, logoHeight);

		bufferGraphics.DrawImage(m_pIntroSheet, logoRect, 495, 0, 480, 288, UnitPixel);
		break;
	}
	case GameState::InBattle:
		// TODO: ���� ȭ�� �׸���
		break;
	}

	// ���̵� ȿ�� ���׸���
	if (m_gameState == GameState::FadingOut) {
		SolidBrush fadeBrush(Color(m_fadeAlpha, 0, 0, 0)); // ������
		bufferGraphics.FillRectangle(&fadeBrush, 0, 0, 800, 600);
	}
	else if (m_gameState == GameState::FadingIn) {
		SolidBrush fadeBrush(Color(m_fadeAlpha, 255, 255, 255)); // ���
		bufferGraphics.FillRectangle(&fadeBrush, 0, 0, 800, 600);
	}

	Graphics screenGraphics(m_hWnd);
	screenGraphics.DrawImage(m_pBuffer, 0, 0);
}

void Game::StartBattle()
{
	delete m_pBattleManager;
	m_pBattleManager = nullptr;
	m_playerParty.clear();
	m_wildParty.clear();

	// [����] PokemonIndividual ����ü�� �����ϰ� �ʱ�ȭ
	PokemonIndividual bulbasaurIndividual;
	bulbasaurIndividual.level = 5;
	bulbasaurIndividual.nature = Nature::HARDY;
	Pokemon bulbasaur(DataManager::GetInstance().GetPokemonSpecies(1), bulbasaurIndividual);
	bulbasaur.LearnMove(DataManager::GetInstance().GetMoveData(33));
	m_playerParty.push_back(bulbasaur);

	PokemonIndividual charmanderIndividual;
	charmanderIndividual.level = 5;
	charmanderIndividual.nature = Nature::HARDY;
	Pokemon charmander(DataManager::GetInstance().GetPokemonSpecies(4), charmanderIndividual);
	charmander.LearnMove(DataManager::GetInstance().GetMoveData(10));
	m_wildParty.push_back(charmander);

	m_pBattleManager = new BattleManager(m_playerParty, m_wildParty);
	m_gameState = GameState::InBattle;

	delete m_pOpponentSprite;
	// [����] �ùٸ� ���� ��� ���
	m_pOpponentSprite = new Bitmap(L"assets/images/pokemon/front/1.png");
	delete m_pPlayerSprite;
	m_pPlayerSprite = new Bitmap(L"assets/images/pokemon/back/4.png");
}