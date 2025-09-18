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
	m_pIntroSheet = new Bitmap(L"../assets/images/ui/intro_sheet.png");
	m_pBuffer = new Bitmap(800, 600);
	//m_pBattleBackground = new Bitmap(L"../asset/images/backgrounds/grass_battle.png");
	//m_pUiSheet = new Bitmap(L"../assets/images/ui/battle_ui.png");

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

	m_nextGameState = GameState::Intro_GameFreak;

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
	case WM_PAINT:
	{
		// 직접 그리기 로직은 Render() 함수로 이동
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
		// 1. 화면이 밝아지는 상태
	case GameState::FadingIn:
	{
		float fadeDuration = 0.2f; // 0.5초 동안 밝아짐
		m_fadeAlpha = 255 - static_cast<int>((m_sceneTimer / fadeDuration) * 255.0f);

		if (m_fadeAlpha <= 0) {
			m_fadeAlpha = 0;
			m_gameState = m_nextGameState; // 페이드인이 끝나면, 목표했던 다음 장면으로 전환
			m_sceneTimer = 0.0f;
		}
		break;
	}

	// 2. GameFreak 로고가 표시되는 상태
	case GameState::Intro_GameFreak:
	{
		// 2초간 장면을 보여준 뒤,
		if (m_sceneTimer > 2.0f) {
			m_nextGameState = GameState::Intro_BattleAnim; // 다음 장면을 '전투 애니메이션'으로 설정하고
			m_gameState = GameState::FadingOut;           // 페이드아웃 시작
			m_sceneTimer = 0.0f;
		}
		break;
	}

	// 3. 화면이 어두워지는 상태
	case GameState::FadingOut:
	{
		float fadeDuration = 1.0f; // 1초 동안 어두워짐
		m_fadeAlpha = static_cast<int>((m_sceneTimer / fadeDuration) * 255.0f);

		if (m_fadeAlpha >= 255) {
			m_fadeAlpha = 255;
			m_gameState = GameState::FadingIn; // 페이드아웃이 끝나면, 다음 장면을 위해 페이드인 시작
			m_sceneTimer = 0.0f;
		}
		break;
	}

	// 4. 전투 애니메이션이 표시되는 상태
	case GameState::Intro_BattleAnim:
	{
		// 5초간 장면을 보여준 뒤,
		if (m_sceneTimer > 5.0f) {
			m_nextGameState = GameState::TitleScreen; // 다음 장면을 '타이틀 화면'으로 설정하고
			m_gameState = GameState::FadingOut;     // 페이드아웃 시작
			m_sceneTimer = 0.0f;
		}
		break;
	}

	// 5. 타이틀 화면이 표시되는 상태
	case GameState::TitleScreen:
	{
		m_showBlinkingText = (static_cast<int>(m_sceneTimer * 2.0f) % 2 == 0);
		// 이 상태에서는 키 입력을 기다리므로 자동으로 다음 상태로 넘어가지 않음
		break;
	}

	case GameState::InBattle:
		// TODO: 전투 로직 업데이트
		break;
	}
}

void Game::Render()
{
	Graphics bufferGraphics(m_pBuffer);
	bufferGraphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	bufferGraphics.SetPixelOffsetMode(PixelOffsetModeHalf);
	bufferGraphics.Clear(Color(0, 0, 0));

	// 화면 비율 유지 계산
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
		stateToRender = m_nextGameState; // 밝아지는 중일 땐, 다음에 나올 장면을 미리 그림
	}

	// 현재 상태에 맞는 장면 그리기
	switch (m_gameState)
	{
	case GameState::Intro_GameFreak:
	{
		// [수정] 알려주신 정확한 좌표로 교체
		bufferGraphics.DrawImage(m_pIntroSheet, gameScreenRect, 8.0f, 42.0f, 245.0f, 161.5f, UnitPixel);
		break;
	}
	case GameState::Intro_BattleAnim:
	{
		// 인트로 전투 배경 그리기
		bufferGraphics.DrawImage(m_pIntroSheet, gameScreenRect, 0, 0, 240, 160, UnitPixel);
		break;
	}
	case GameState::TitleScreen:
	{
		// 타이틀 화면 그리기
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
		// TODO: 전투 화면 그리기
		break;
	}

	// 페이드 효과 덧그리기
	if (m_gameState == GameState::FadingOut) {
		SolidBrush fadeBrush(Color(m_fadeAlpha, 0, 0, 0)); // 검은색
		bufferGraphics.FillRectangle(&fadeBrush, 0, 0, 800, 600);
	}
	else if (m_gameState == GameState::FadingIn) {
		SolidBrush fadeBrush(Color(m_fadeAlpha, 255, 255, 255)); // 흰색
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

	// [수정] PokemonIndividual 구조체를 안전하게 초기화
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
	// [수정] 올바른 파일 경로 사용
	m_pOpponentSprite = new Bitmap(L"assets/images/pokemon/front/1.png");
	delete m_pPlayerSprite;
	m_pPlayerSprite = new Bitmap(L"assets/images/pokemon/back/4.png");
}