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

	// GDI+ 초기화
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	// 게임 데이터 로드
	DataManager::GetInstance().LoadAllData();

	// 리소스 로드
	m_pBuffer = new Bitmap(m_clientWidth, m_clientHeight);
	m_pIntroSheet = new Bitmap(L"../assets/images/ui/intro_sheet.png");
	//m_pBattleBackground = new Bitmap(L"assets/images/backgrounds/grass_battle.png");
	//m_pUiSheet = new Bitmap(L"assets/images/ui/battle_ui.png");

	// 리소스 로드 실패 확인
	/*if (m_pIntroSheet->GetLastStatus() != Ok || m_pBuffer->GetLastStatus() != Ok || m_pBattleBackground->GetLastStatus() != Ok || m_pUiSheet->GetLastStatus() != Ok)
	{
		MessageBox(m_hWnd, L"필수 이미지 리소스를 불러오는 데 실패했습니다.", L"리소스 오류", MB_OK);
		return false;
	}*/
	if (m_pIntroSheet->GetLastStatus() != Ok || m_pBuffer->GetLastStatus() != Ok)
	{
		MessageBox(m_hWnd, L"필수 이미지 리소스를 불러오는 데 실패했습니다.", L"리소스 오류", MB_OK);
		return false;
	}

	return true;
}

void Game::Run()
{
	MSG msg = {};
	ULONGLONG lastTime = GetTickCount64();

	while (true) // 1. 무한 루프를 돕니다.
	{
		// 2. 처리할 윈도우 메시지가 있는지 '확인만 하고' 바로 넘어갑니다 (PeekMessage).
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break; // 종료 메시지를 받으면 루프를 탈출합니다.

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else // 3. 처리할 메시지가 없다면, 게임 로직을 실행합니다.
		{
			ULONGLONG currentTime = GetTickCount64();
			float deltaTime = (currentTime - lastTime) / 1000.0f;
			lastTime = currentTime;

			Update(deltaTime); // 게임 상태를 업데이트하고
			InvalidateRect(m_hWnd, NULL, FALSE); // 화면을 다시 그리라고 요청합니다.
		}
	}
}

void Game::Release()
{
	// 동적으로 할당된 모든 리소스를 해제합니다.
	delete m_pIntroSheet;
	delete m_pBuffer;
	delete m_pBattleBackground;
	delete m_pOpponentSprite;
	delete m_pPlayerSprite;
	delete m_pUiSheet;
	delete m_pBattleManager;

	// GDI+ 종료
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
	// 페이드 로직 처리
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
				m_fadeState = FadeState::None;    // [수정] 페이드 종료 (다시 페이드인 하지 않음)
				m_gameState = m_nextGameState;  // [수정] 바로 다음 장면으로 전환
				m_sceneTimer = 0.0f;
			}
		}
	}
	// 일반 게임 상태 업데이트 (페이드 중이 아닐 때)
	else
	{
		m_sceneTimer += deltaTime;
		switch (m_gameState)
		{
		case GameState::Intro_GameFreak:
			if (m_sceneTimer > 2.0f) 
			{
				m_nextGameState = GameState::Intro_BattleAnim;
				m_fadeState = FadeState::FadingOut; // 페이드아웃 시작
				m_fadeTimer = 0.0f;
			}
			break;
		case GameState::Intro_BattleAnim:
			if (m_sceneTimer > 5.0f) 
			{
				m_nextGameState = GameState::TitleScreen;
				m_fadeState = FadeState::FadingOut; // 페이드아웃 시작
				m_fadeTimer = 0.0f;
			}
			break;
		case GameState::TitleScreen:
			// 깜빡이는 텍스트 로직은 Render에서 처리
			break;
		case GameState::InBattle:
			// TODO: 전투 로직 업데이트
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
		// Title 로고는 비율 유지된 화면 위에 다시 그림
		break;
	case GameState::InBattle:
		// 전투 화면 그리기
		break;
	}

	// 페이드 효과 덧그리기
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
	m_fadeState = FadeState::FadingOut; // 전투 시작 시 페이드아웃
	m_fadeTimer = 0.0f;
}
