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
	// 동적으로 할당된 모든 리소스를 해제
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
	// 페이드 로직 처리
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
				
				// Intro_BattleAnim 장면으로 전환될 때만 리빌 애니메이션 시작
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
				m_gameState = m_nextGameState; // [수정] 페이드아웃 후 바로 다음 장면으로 전환 (검은 화면 상태)
				m_fadeState = FadeState::None;
				m_sceneTimer = 0.0f;

				// [수정] 다음 장면이 전투 애니메이션일 때만 리빌 시작
				if (m_nextGameState == GameState::Intro_BattleAnim)
				{
					m_isRevealing = true;
					m_revealTimer = 0.0f;
				}

			}
		}
	}
	// 창이 열리는 애니메이션 로직
	else if (m_isRevealing)
	{
		m_revealTimer += deltaTime;
		float revealDuration = 0.5f; // [수정] 속도를 0.5초로 빠르게 조절

		if (m_revealTimer >= revealDuration) {
			m_isRevealing = false; // [수정] 한 번만 실행되도록 애니메이션 종료
		}
		else
		{
			m_revealOffset = (m_revealTimer / revealDuration) * (m_clientHeight / 2.0f);
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
				// [추가] 별 스크롤 위치 업데이트
				m_starScrollX -= 50.0f * deltaTime; // 1초에 50픽셀씩 왼쪽으로 이동 (속도 조절 가능)

				// 별 패턴의 가로 크기 (화면 비율에 맞춰 계산)
				float starPatternWidth = 42.0f * (gameScreenRect.Width / 240.0f);
				if (m_starScrollX < -starPatternWidth) 
				{
					m_starScrollX += starPatternWidth; // 완전히 화면 밖으로 나가면 다시 원위치
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
			// 깜빡이는 텍스트 로직
			break;
		case GameState::InBattle:
			// 전투 로직
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

	// 1. 현재 상태에 맞는 장면 그리기
	// 페이드인/리빌 중에는 다음에 나올 장면을 미리 그려줍니다.
	GameState stateToRender = m_gameState;
	if (m_fadeState == FadeState::FadingIn || m_isRevealing) {
		stateToRender = m_nextGameState;
	}
	// 리빌 효과가 Intro_BattleAnim 에서 일어나므로, nextGameState가 Intro_BattleAnim일 때
	// Intro_BattleAnim을 미리 그려주도록 수정합니다.
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
		// Title 로고는 비율 유지된 화면 위에 다시 그림
		break;
	case GameState::InBattle:
		// 전투 화면 그리기
		break;
	}

	// 2. [추가] 별 스크롤 효과 그리기
	if (m_pIntroSheet)
	{
		// 스프라이트 시트에서 별 이미지의 정보
		float starSourceX = 515.0f;
		float starSourceY = 177.0f;
		float starSourceWidth = 42.0f;
		float starSourceHeight = 14.0f;

		// 화면 비율에 맞춰 별이 그려질 크기와 Y 위치 계산
		float scale = gameScreenRect.Width / 240.0f; // 원본 대비 화면 확대 비율
		float renderStarHeight = starSourceHeight * scale;
		float renderStarY = gameScreenRect.Y + (42.0f * scale); // 로고 배경의 Y좌표(42)에 맞춰 위치

		// 별 패턴의 화면상 너비
		float renderStarPatternWidth = starSourceWidth * scale;

		// 화면을 꽉 채우고도 남도록 별을 여러 번 그려서 끊김 없는 스크롤 효과 생성
		for (int i = -1; i < (m_clientWidth / renderStarPatternWidth) + 2; ++i)
		{
			RectF starDestRect(
				gameScreenRect.X + m_starScrollX + (i * renderStarPatternWidth),
				renderStarY,
				renderStarPatternWidth,
				renderStarHeight
			);

			// DrawImage로 별 부분만 잘라내어 그립니다.
			bufferGraphics.DrawImage(m_pIntroSheet, starDestRect,
				starSourceX, starSourceY,
				starSourceWidth, starSourceHeight, UnitPixel);
		}
	}

	// 2. 장면 위에 열리는 검은 막대 그리기
	if (m_isRevealing)
	{
		SolidBrush blackBrush(Color(255, 0, 0, 0));
		// 위쪽 막대
		RectF topRect(0.0f, 0.0f, (float)m_clientWidth, (m_clientHeight / 2.0f) - m_revealOffset);
		bufferGraphics.FillRectangle(&blackBrush, topRect);
		// 아래쪽 막대
		RectF bottomRect(0.0f, (m_clientHeight / 2.0f) + m_revealOffset, (float)m_clientWidth, (m_clientHeight / 2.0f));
		bufferGraphics.FillRectangle(&blackBrush, bottomRect);
	}

	// 페이드 효과 덧그리기
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
	m_fadeState = FadeState::FadingOut; // 전투 시작 시 페이드아웃
	m_fadeTimer = 0.0f;
}
