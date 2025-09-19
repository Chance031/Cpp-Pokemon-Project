#include "BattleScene.h"
#include "DataManager.h" // 포켓몬 생성을 위해 필요

using namespace Gdiplus;

BattleScene::BattleScene(HWND hWnd, int clientWidth, int clientHeight)
	: m_hWnd(hWnd), m_clientWidth(clientWidth), m_clientHeight(clientHeight)
{
}

BattleScene::~BattleScene()
{
	// Release 함수에서 모든 동적 할당 해제를 처리합니다.
}

bool BattleScene::Init()
{
	// 1. 리소스 로드
	m_pBackground = new Bitmap(L"assets/images/battle/background_grass.png");
	m_pBattleUI = new Bitmap(L"assets/images/ui/battle_ui_sheet.png");

	if (m_pBackground->GetLastStatus() != Ok || m_pBattleUI->GetLastStatus() != Ok)
	{
		MessageBox(m_hWnd, L"배틀 리소스를 불러올 수 없습니다.", L"리소스 오류", MB_OK);
		return false;
	}

	// 2. main.cpp의 포켓몬 생성 로직을 이곳으로 이전
	// 플레이어 파티 생성 (이상해씨, 구구)
	const PokemonSpecies& bulbasaurSpecies = DataManager::GetInstance().GetPokemonSpecies(1);
	PokemonIndividual bulbasaurIndividual;
	bulbasaurIndividual.level = 5;
	// (IV/EV 설정은 간단하게 생략, 실제 게임에서는 랜덤 또는 고정값 설정 필요)
	Pokemon bulbasaur(bulbasaurSpecies, bulbasaurIndividual);
	bulbasaur.LearnMove(DataManager::GetInstance().GetMoveData(33)); // 몸통박치기
	bulbasaur.LearnMove(DataManager::GetInstance().GetMoveData(45)); // 울음소리

	const PokemonSpecies& pidgeySpecies = DataManager::GetInstance().GetPokemonSpecies(16);
	PokemonIndividual pidgeyIndividual;
	pidgeyIndividual.level = 5;
	Pokemon pidgey(pidgeySpecies, pidgeyIndividual);
	pidgey.LearnMove(DataManager::GetInstance().GetMoveData(33)); // 몸통박치기

	m_playerParty.push_back(bulbasaur);
	m_playerParty.push_back(pidgey);

	// 상대(야생) 파티 생성 (아보크)
	const PokemonSpecies& arbokSpecies = DataManager::GetInstance().GetPokemonSpecies(24);
	PokemonIndividual arbokIndividual;
	arbokIndividual.level = 5;
	Pokemon arbok(arbokSpecies, arbokIndividual);
	arbok.LearnMove(DataManager::GetInstance().GetMoveData(33));

	m_opponentParty.push_back(arbok);

	// 3. BattleManager 생성 및 초기화
	m_pBattleManager = new BattleManager(m_playerParty, m_opponentParty);
	// m_pBattleManager->Start(); // <- 콘솔 루프가 돌기 때문에 아직 호출하지 않음

	CalculateGameScreenRect();
	m_currentState = BATTLE_UI_STATE::MAIN_MENU; // 임시로 바로 메뉴를 보도록 설정
	return true;
}

void BattleScene::Update(float deltaTime)
{
	// 현재는 비워두지만, 나중에 애니메이션, 메시지 출력 타이밍 등을 여기서 제어합니다.
	switch (m_currentState)
	{
	case BATTLE_UI_STATE::MAIN_MENU:
		// 키 입력 처리는 MsgProc에서
		break;
	case BATTLE_UI_STATE::FIGHT_MENU:
		// 키 입력 처리는 MsgProc에서
		break;
		// ... 다른 상태들 처리
	}
}

void BattleScene::Render(Gdiplus::Graphics& bufferGraphics)
{
	// 검은색으로 배경 초기화
	bufferGraphics.Clear(Color::Black);

	// 1. 전투 배경 그리기
	bufferGraphics.DrawImage(m_pBackground, m_gameScreenRect, 0, 0, 240, 112, UnitPixel);

	// 2. UI 그리기 (상대/플레이어 상태창, 메시지 박스 등)
	// 상대 HP 박스
	bufferGraphics.DrawImage(m_pBattleUI, m_gameScreenRect.X + 10, m_gameScreenRect.Y + 20, 120, 35, 0, 0, 120, 35, UnitPixel);
	// 플레이어 HP 박스
	bufferGraphics.DrawImage(m_pBattleUI, m_gameScreenRect.X + m_gameScreenRect.Width - 130, m_gameScreenRect.Y + m_gameScreenRect.Height - 80, 120, 45, 0, 35, 120, 45, UnitPixel);
	// 메시지 박스
	bufferGraphics.DrawImage(m_pBattleUI, m_gameScreenRect.X, m_gameScreenRect.Y + m_gameScreenRect.Height - 48, 240, 48, 0, 84, 240, 48, UnitPixel);

	// 3. 현재 UI 상태에 따라 메뉴 그리기
	if (m_currentState == BATTLE_UI_STATE::MAIN_MENU)
	{
		// "싸운다", "가방" 등 메뉴 옵션 그리기
		bufferGraphics.DrawImage(m_pBattleUI, m_gameScreenRect.X + m_gameScreenRect.Width - 100, m_gameScreenRect.Y + m_gameScreenRect.Height - 50, 100, 50, 0, 132, 100, 50, UnitPixel);

		// 현재 선택된 메뉴에 커서(화살표) 그리기
		// m_mainMenuSelection 값에 따라 커서 위치를 다르게 그려야 함
		// 예: 0(싸운다)일 때와 2(포켓몬)일 때 커서의 Y 좌표가 다름
		int cursorX = static_cast<int>(m_gameScreenRect.X + m_gameScreenRect.Width - 95);
		int cursorY = static_cast<int>(m_gameScreenRect.Y + m_gameScreenRect.Height - 38);
		if (m_mainMenuSelection >= 2) // 포켓몬, 도망
		{
			cursorY += 16;
		}
		if (m_mainMenuSelection % 2 != 0) // 가방, 도망
		{
			cursorX += 48;
		}
		bufferGraphics.DrawImage(m_pBattleUI, cursorX, cursorY, 7, 10, 101, 132, 7, 10, UnitPixel);
	}
}

void BattleScene::Release()
{
	delete m_pBattleManager;
	m_pBattleManager = nullptr;

	delete m_pBackground;
	m_pBackground = nullptr;

	delete m_pBattleUI;
	m_pBattleUI = nullptr;
}

LRESULT BattleScene::MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (iMessage == WM_KEYDOWN)
	{
		if (m_currentState == BATTLE_UI_STATE::MAIN_MENU)
		{
			switch (wParam)
			{
			case VK_UP:
				if (m_mainMenuSelection >= 2) m_mainMenuSelection -= 2;
				break;
			case VK_DOWN:
				if (m_mainMenuSelection < 2) m_mainMenuSelection += 2;
				break;
			case VK_LEFT:
				if (m_mainMenuSelection % 2 != 0) m_mainMenuSelection--;
				break;
			case VK_RIGHT:
				if (m_mainMenuSelection % 2 == 0) m_mainMenuSelection++;
				break;
			case VK_RETURN: // Enter 키
				// TODO: 선택한 메뉴에 따라 다음 상태로 전환하는 로직
				// 예: m_mainMenuSelection == 0 이면 m_currentState = BATTLE_UI_STATE::FIGHT_MENU;
				break;
			}
			return 0;
		}
	}
	return 0;
}

void BattleScene::CalculateGameScreenRect()
{
	// IntroScene과 동일한 GBA 화면 비율 유지 로직
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
	m_gameScreenRect = RectF(destX, destY, destWidth, destHeight);
}