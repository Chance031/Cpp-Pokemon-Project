#include "framework.h"
#include "BattleScene.h"
#include "DataManager.h"
#include <gdiplus.h>
#include <string>

using namespace Gdiplus;

BattleScene::BattleScene(HWND hWnd, int clientWidth, int clientHeight)
	: m_hWnd(hWnd), m_clientWidth(clientWidth), m_clientHeight(clientHeight) {
}

BattleScene::~BattleScene()
{
	Release();
}

bool BattleScene::Init()
{
	// 에셋 파일의 상대 경로 설정
	m_pBackground = new Bitmap(L"../assets/images/battle/background_grass.png");
	m_pBattleUI = new Bitmap(L"../assets/images/ui/battle_ui_sheet.png");
	if (m_pBackground->GetLastStatus() != Ok || m_pBattleUI->GetLastStatus() != Ok)
	{
		MessageBox(m_hWnd, L"배틀 리소스를 불러올 수 없습니다.", L"리소스 오류", MB_OK);
		return false;
	}

	// --- 커스텀 폰트 로드 ---
	m_pFontCollection = new PrivateFontCollection();
	m_pFontCollection->AddFontFile(L"../assets/fonts/Galmuri14.ttf");

	// --- 폰트 객체 생성 ---
	FontFamily fontFamily;
	int numFound = 0;
	m_pFontCollection->GetFamilies(1, &fontFamily, &numFound);
	if (numFound > 0)
	{
		m_pMainFont = new Font(&fontFamily, 14, FontStyleRegular, UnitPixel);
	}
	else // 폰트 로드 실패 시 대체 폰트 사용
	{
		m_pMainFont = new Font(L"돋움", 14, FontStyleRegular, UnitPixel);
	}

	// 테스트용 포켓몬 파티 생성
	const PokemonSpecies& species1 = DataManager::GetInstance().GetPokemonSpecies(1);
	Pokemon p1(species1, { 5 });
	p1.LearnMove(DataManager::GetInstance().GetMoveData(33));
	p1.LearnMove(DataManager::GetInstance().GetMoveData(45));
	m_playerParty.push_back(p1);

	const PokemonSpecies& species2 = DataManager::GetInstance().GetPokemonSpecies(25);
	Pokemon p2(species2, { 5 });
	p2.LearnMove(DataManager::GetInstance().GetMoveData(85));
	m_opponentParty.push_back(p2);

	// BattleManager 생성
	m_pBattleManager = new BattleManager(m_playerParty, m_opponentParty);

	// --- 포켓몬 스프라이트 로드 ---
	Pokemon* playerMon = m_pBattleManager->GetPlayerActivePokemon();
	Pokemon* opponentMon = m_pBattleManager->GetOpponentActivePokemon();

	// 포켓몬 ID를 이용해 동적으로 파일 경로 생성
	// std::to_wstring 함수는 숫자를 와이드 문자열로 변환합니다.
	std::wstring playerSpritePath = L"../assets/images/pokemon/back/" + std::to_wstring(playerMon->GetSpecies().id) + L".png";
	std::wstring opponentSpritePath = L"../assets/images/pokemon/front/" + std::to_wstring(opponentMon->GetSpecies().id) + L".png";

	m_pPlayerPokemonSprite = new Bitmap(playerSpritePath.c_str());
	m_pOpponentPokemonSprite = new Bitmap(opponentSpritePath.c_str());

	// 이미지 로드 실패 시 에러 처리
	if (m_pPlayerPokemonSprite->GetLastStatus() != Ok || m_pOpponentPokemonSprite->GetLastStatus() != Ok)
	{
		MessageBox(m_hWnd, L"포켓몬 스프라이트 이미지를 불러올 수 없습니다.", L"리소스 오류", MB_OK);
		return false;
	}

	// 전투 시작 이벤트를 생성하고 큐에 삽입
	std::vector<TurnEvent> startEvents;
	m_pBattleManager->Start(startEvents);
	for (const auto& event : startEvents) 
	{
		m_eventQueue.push(event);
	}
	std::string startMessage = "야생의 " + m_pBattleManager->GetOpponentActivePokemon()->GetName() + "이(가) 나타났다!";
	m_eventQueue.push({ TurnEventType::TEXT_MESSAGE, nullptr, startMessage });

	// HP 애니메이션을 위한 시각적 HP 변수 초기화
	m_playerVisualHp = (float)m_pBattleManager->GetPlayerActivePokemon()->GetCurrentHP();
	m_opponentVisualHp = (float)m_pBattleManager->GetOpponentActivePokemon()->GetCurrentHP();

	CalculateGameScreenRect();
	ProcessNextEvent();
	return true;
}

void BattleScene::Release()
{
	delete m_pBattleManager; m_pBattleManager = nullptr;
	delete m_pBackground; m_pBackground = nullptr;
	delete m_pBattleUI; m_pBattleUI = nullptr;

	// --- 스프라이트 리소스 해제 ---
	delete m_pPlayerPokemonSprite; m_pPlayerPokemonSprite = nullptr;
	delete m_pOpponentPokemonSprite; m_pOpponentPokemonSprite = nullptr;

	// --- 폰트 컬렉션 리소스 해제 ---
	delete m_pMainFont; m_pMainFont = nullptr;
	delete m_pFontCollection; m_pFontCollection = nullptr;
}

void BattleScene::Update(float deltaTime)
{
	// 현재 이벤트를 처리 중이면, 해당 연출 업데이트 함수 호출
	if (m_isProcessingEvent)
	{
		switch (m_currentEvent.type)
		{
		case TurnEventType::TEXT_MESSAGE:
		case TurnEventType::FAINT:
		case TurnEventType::STAT_CHANGE:
		case TurnEventType::STATUS_EFFECT:
			UpdateTextMessage(deltaTime);
			break;
		case TurnEventType::DAMAGE:
			// 데미지 이벤트 자체는 연출이 없으므로 바로 플래그를 false로 설정.
			// 실제 HP 감소 애니메이션은 아래의 UpdateHpAnimation에서 항상 처리됨.
			m_isProcessingEvent = false;
			break;
		}
	}
	// 처리 중인 이벤트가 없고 큐에 다음 이벤트가 있다면, 다음 이벤트 처리 시작
	else if (!m_eventQueue.empty())
	{
		ProcessNextEvent();
	}
	// 모든 이벤트 처리가 끝나면, 플레이어의 입력을 기다리는 상태로 전환
	else
	{
		m_currentState = BATTLE_UI_STATE::AWAITING_INPUT;
	}

	// HP 애니메이션은 이벤트 처리와 별개로 매 프레임 부드럽게 갱신
	UpdateHpAnimation(deltaTime);
}

void BattleScene::Render(Gdiplus::Graphics& bufferGraphics)
{
	bufferGraphics.Clear(Color::Black);
	bufferGraphics.SetInterpolationMode(InterpolationModeNearestNeighbor);	// 픽셀 아트용 설정
	bufferGraphics.SetPixelOffsetMode(PixelOffsetModeHalf);

	// 각 부분을 그리는 헬퍼 함수 호출
	RenderUI(bufferGraphics);
	RenderPokemonSprites(bufferGraphics);
	RenderHpBars(bufferGraphics);
	RenderText(bufferGraphics);

	// 기술 선택 메뉴 상태일 때만 추가로 그림
	if (m_currentState == BATTLE_UI_STATE::FIGHT_MENU)
	{
		RenderFightMenu(bufferGraphics);
	}
}

LRESULT BattleScene::MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (iMessage != WM_KEYDOWN) return 0;

	// --- 엔터(확인) 키 처리 ---
	if (wParam == VK_RETURN)
	{
		// 이벤트 연출 중에 엔터를 누른 경우
		if (m_isProcessingEvent)
		{
			// 메세지 출력이 끝나지 않았다면, 즉시 전체 메시지를 보여줌 (스킵)
			if (!m_isMessageFinished)
			{
				m_isMessageFinished = true;
				m_visibleMessage = m_fullMessageToShow;
			}
			// 메시지 출력이 끝났다면, 현재 이벤트 처리를 종료하고 다음으로 넘어갈 준비
			else
			{
				m_isProcessingEvent = false;
			}
		}
		// 플레이어의 주 메뉴 입력을 기다릴 때
		else if (m_currentState == BATTLE_UI_STATE::AWAITING_INPUT)
		{
			// '싸우다' 선택
			if (m_mainMenuSelection == 0)
			{
				m_currentState = BATTLE_UI_STATE::FIGHT_MENU;
				m_fightMenuSelection = 0;
			}
		}
		// 기술 선택 메뉴 입력을 기다릴 때
		else if (m_currentState == BATTLE_UI_STATE::FIGHT_MENU)
		{
			Pokemon* playerMon = m_pBattleManager->GetPlayerActivePokemon();
			const auto& moves = playerMon->GetMoveset();
			if (m_fightMenuSelection < moves.size())
			{
				// 선택한 기술로 BattleAction을 만들고 BattleManager에 턴 처리를 요청
				BattleAction action = { PlayerActionType::FIGHT, &playerMon->GetMovesetForModify()[m_fightMenuSelection] };
				TurnResult turnResult = m_pBattleManager->ProcessTurn(action);
				
				// 반환된 모든 이벤트를 큐에 추가
				for (const auto& event : turnResult.events) {
					m_eventQueue.push(event);
				}

				// 다시 이벤트 처리 상태로 전환
				m_currentState = BATTLE_UI_STATE::EVENT_PROCESSING;
			}
		}
		return 0;
	}
	// --- 방향키 및 기타 키 처리 ---
	else
	{
		if (m_currentState == BATTLE_UI_STATE::AWAITING_INPUT) 
		{
			switch (wParam) 
			{
			case VK_UP: if (m_mainMenuSelection >= 2)
			{
				m_mainMenuSelection -= 2; 
				break;
			}
			case VK_DOWN: if (m_mainMenuSelection < 2)
			{
				m_mainMenuSelection += 2; 
				break;
			}
			case VK_LEFT: if (m_mainMenuSelection % 2 != 0)
			{
				m_mainMenuSelection--; 
				break;
			}
			case VK_RIGHT: if (m_mainMenuSelection % 2 == 0)
			{
				m_mainMenuSelection++;
				break;
			}
			}
		}
		else if (m_currentState == BATTLE_UI_STATE::FIGHT_MENU) {
			switch (wParam) 
			{
			case VK_UP: if (m_fightMenuSelection >= 2)
			{
				m_fightMenuSelection -= 2;
				break;
			}
			case VK_DOWN: if (m_fightMenuSelection < 2)
			{
				m_fightMenuSelection += 2;
				break;
			}
			case VK_LEFT: if (m_fightMenuSelection % 2 != 0)
			{
				m_fightMenuSelection--;
				break;
			}
			case VK_RIGHT: if (m_fightMenuSelection % 2 == 0)
			{
				m_fightMenuSelection++;
				break;
			}
			case VK_BACK:
			{
				m_currentState = BATTLE_UI_STATE::AWAITING_INPUT;
				break;
			}
			}
		}
	}
	return 0;
}

void BattleScene::ProcessNextEvent()
{
	// 큐가 비었다면, 모든 연출이 끝났으므로 입력 대기 상태로
	if (m_eventQueue.empty())
	{
		m_isProcessingEvent = false;
		m_currentState = BATTLE_UI_STATE::AWAITING_INPUT;
		return;
	}

	// 큐에서 다음 이벤트를 꺼냄
	m_currentEvent = m_eventQueue.front();
	m_eventQueue.pop();
	m_isProcessingEvent = true;
	m_currentState = BATTLE_UI_STATE::EVENT_PROCESSING;

	// 이벤트 타입에 따라 연출에 필요한 변수들을 초기화
	switch (m_currentEvent.type)
	{
	case TurnEventType::TEXT_MESSAGE:
	case TurnEventType::FAINT:
	case TurnEventType::STAT_CHANGE:
	case TurnEventType::STATUS_EFFECT:
	{
		m_fullMessageToShow = std::wstring(m_currentEvent.message.begin(), m_currentEvent.message.end());
		m_visibleMessage.clear();
		m_messageCharIndex = 0;
		m_textAnimationTimer = 0.0f;
		m_isMessageFinished = false;
		break;
	}
	case TurnEventType::DAMAGE:
	{
		// 데미지 이벤트는 메시지 출력 없이 HP 애니메이션만 있으므로, 바로 처리를 끝냄
		m_isProcessingEvent = false;
		break;
	}
	}
}

void BattleScene::UpdateTextMessage(float deltaTime)
{
	if (m_isMessageFinished) return;
	m_textAnimationTimer += deltaTime;

	// 0.05초마다 한 글자씩
	if (m_textAnimationTimer > 0.05f) 
	{
		m_textAnimationTimer = 0.0f;
		if (m_messageCharIndex < m_fullMessageToShow.length()) 
		{
			m_messageCharIndex++;
			m_visibleMessage = m_fullMessageToShow.substr(0, m_messageCharIndex);
		}
		else 
		{
			// 메시지 출력이 모두 끝나면 플래그 설정
			m_isMessageFinished = true;
		}
	}
}

void BattleScene::UpdateHpAnimation(float deltaTime)
{
	Pokemon* playerMon = m_pBattleManager->GetPlayerActivePokemon();
	Pokemon* opponentMon = m_pBattleManager->GetOpponentActivePokemon();
	float playerTargetHp = (float)playerMon->GetCurrentHP();
	float opponentTargetHp = (float)opponentMon->GetCurrentHP();
	
	// 초당 40 HP의 속도로 변화
	float speed = 40.0f * deltaTime;

	if (m_playerVisualHp > playerTargetHp)
	{
		m_playerVisualHp -= speed;
		if (m_playerVisualHp < playerTargetHp)
		{
			m_playerVisualHp = playerTargetHp;
		}
	}
	if (m_opponentVisualHp > opponentTargetHp)
	{
		m_opponentVisualHp -= speed;
		if (m_opponentVisualHp < opponentTargetHp)
		{
			m_opponentVisualHp = opponentTargetHp;
		}
	}
}

void BattleScene::RenderUI(Gdiplus::Graphics& g)
{
	RectF destBackRect = m_gameScreenRect;
	g.DrawImage(m_pBackground, destBackRect, 0.0f, 0.0f, 240.0f, 112.0f, UnitPixel);

	RectF destOpponentHpBox(m_gameScreenRect.X + 10.0f, m_gameScreenRect.Y + 20.0f, 120.0f, 35.0f);
	g.DrawImage(m_pBattleUI, destOpponentHpBox, 0.0f, 0.0f, 120.0f, 35.0f, UnitPixel);

	RectF destPlayerHpBox(m_gameScreenRect.X + m_gameScreenRect.Width - 130.0f, m_gameScreenRect.Y + m_gameScreenRect.Height - 80.0f, 120.0f, 45.0f);
	g.DrawImage(m_pBattleUI, destPlayerHpBox, 0.0f, 35.0f, 120.0f, 45.0f, UnitPixel);

	if (m_currentState == BATTLE_UI_STATE::AWAITING_INPUT) {
		RenderMainMenu(g);
	}
	else {
		RectF destMsgBox(m_gameScreenRect.X, m_gameScreenRect.Y + m_gameScreenRect.Height - 48.0f, 240.0f, 48.0f);
		g.DrawImage(m_pBattleUI, destMsgBox, 0.0f, 84.0f, 240.0f, 48.0f, UnitPixel);
	}
}

void BattleScene::RenderText(Gdiplus::Graphics& g)
{
	// 멤버 변수에 저장된 폰트 객체를 사용
	SolidBrush brush(Color(255, 80, 80, 80));
	StringFormat format;
	format.SetAlignment(StringAlignmentNear);
	PointF pointF(m_gameScreenRect.X + 20.0f, m_gameScreenRect.Y + m_gameScreenRect.Height - 38.0f);

	if (m_currentState == BATTLE_UI_STATE::AWAITING_INPUT) {
		g.DrawString(L"무엇을 할까?", -1, m_pMainFont, pointF, &format, &brush);
	}
	else if (m_isProcessingEvent && !m_visibleMessage.empty()) {
		g.DrawString(m_visibleMessage.c_str(), -1, m_pMainFont, pointF, &format, &brush);
	}
}

void BattleScene::RenderHpBars(Gdiplus::Graphics& g)
{
	Pokemon* playerMon = m_pBattleManager->GetPlayerActivePokemon();
	Pokemon* opponentMon = m_pBattleManager->GetOpponentActivePokemon();
	float playerHpRatio = m_playerVisualHp / playerMon->GetMaxHP();
	float opponentHpRatio = m_opponentVisualHp / opponentMon->GetMaxHP();

	SolidBrush greenBrush(Color::Green), yellowBrush(Color::Yellow), redBrush(Color::Red);
	Brush* playerBrush = (playerHpRatio < 0.2f) ? &redBrush : (playerHpRatio < 0.5f) ? &yellowBrush : &greenBrush;
	Brush* opponentBrush = (opponentHpRatio < 0.2f) ? &redBrush : (opponentHpRatio < 0.5f) ? &yellowBrush : &greenBrush;

	float barMaxWidth = 64.0f;

	RectF opponentHpBarRect(m_gameScreenRect.X + 48.0f, m_gameScreenRect.Y + 34.0f, barMaxWidth * opponentHpRatio, 4.0f);
	g.FillRectangle(opponentBrush, opponentHpBarRect);

	RectF playerHpBarRect(m_gameScreenRect.X + m_gameScreenRect.Width - 80.0f, m_gameScreenRect.Y + m_gameScreenRect.Height - 66.0f, barMaxWidth * playerHpRatio, 4.0f);
	g.FillRectangle(playerBrush, playerHpBarRect);
}

void BattleScene::RenderPokemonSprites(Gdiplus::Graphics& g)
{
	if (!m_pPlayerPokemonSprite || !m_pOpponentPokemonSprite)
	{
		return;
	}

	Pokemon* playerMon = m_pBattleManager->GetPlayerActivePokemon();
	Pokemon* opponentMon = m_pBattleManager->GetOpponentActivePokemon();

	// 상대 포켓몬 (앞모습) 그리기 - 기절하지 않았을 때만!
	if (opponentMon && !opponentMon->IsFainted())
	{
		float spriteWidth = (float)m_pOpponentPokemonSprite->GetWidth();
		float spriteHeight = (float)m_pOpponentPokemonSprite->GetHeight();
		RectF destRect(
			m_gameScreenRect.X + m_gameScreenRect.Width - spriteWidth - 55.0f,
			m_gameScreenRect.Y + 25.0f,
			spriteWidth,
			spriteHeight
		);
		g.DrawImage(m_pOpponentPokemonSprite, destRect, 0.0f, 0.0f, spriteWidth, spriteHeight, UnitPixel);
	}

	// 플레이어 포켓몬 (뒷모습) 그리기 - 기절하지 않았을 때만!
	if (playerMon && !playerMon->IsFainted())
	{
		float spriteWidth = (float)m_pPlayerPokemonSprite->GetWidth();
		float spriteHeight = (float)m_pPlayerPokemonSprite->GetHeight();
		RectF destRect(
			m_gameScreenRect.X + 30.0f,
			m_gameScreenRect.Y + m_gameScreenRect.Height - spriteHeight - 32.0f,
			spriteWidth,
			spriteHeight
		);
		g.DrawImage(m_pPlayerPokemonSprite, destRect, 0.0f, 0.0f, spriteWidth, spriteHeight, UnitPixel);
	}
}

void BattleScene::RenderFightMenu(Gdiplus::Graphics& g)
{
	RectF destFightMenuBox(m_gameScreenRect.X, m_gameScreenRect.Y + m_gameScreenRect.Height - 48.0f, 240.0f, 48.0f);
	g.DrawImage(m_pBattleUI, destFightMenuBox, 122.0f, 0.0f, 118.0f, 48.0f, UnitPixel);

	// [수정] 매번 폰트를 생성하는 대신, 멤버 변수에 저장된 폰트 객체를 사용
	SolidBrush brush(Color(255, 80, 80, 80));
	StringFormat format;
	format.SetAlignment(StringAlignmentNear);
	PointF pointF(m_gameScreenRect.X + 20.0f, m_gameScreenRect.Y + m_gameScreenRect.Height - 38.0f);

	float cursorX = m_gameScreenRect.X + 15.0f + (m_fightMenuSelection % 2) * 80.0f;
	float cursorY = m_gameScreenRect.Y + m_gameScreenRect.Height - 36.0f + (m_fightMenuSelection / 2) * 16.0f;
	RectF destCursorRect(cursorX, cursorY, 7.0f, 10.0f);
	g.DrawImage(m_pBattleUI, destCursorRect, 101.0f, 132.0f, 7.0f, 10.0f, UnitPixel);
}

void BattleScene::RenderMainMenu(Gdiplus::Graphics& g)
{
	RectF destMainMenuBox(m_gameScreenRect.X + m_gameScreenRect.Width - 100.0f, m_gameScreenRect.Y + m_gameScreenRect.Height - 50.0f, 100.0f, 50.0f);
	g.DrawImage(m_pBattleUI, destMainMenuBox, 0.0f, 132.0f, 100.0f, 50.0f, UnitPixel);

	float cursorX = m_gameScreenRect.X + m_gameScreenRect.Width - 95.0f;
	float cursorY = m_gameScreenRect.Y + m_gameScreenRect.Height - 38.0f;
	
	if (m_mainMenuSelection >= 2)
	{
		cursorY += 16.0f;
	}
	if (m_mainMenuSelection % 2 != 0)
	{
		cursorX += 48.0f;
	}

	RectF destCursorRect(cursorX, cursorY, 7.0f, 10.0f);
	g.DrawImage(m_pBattleUI, destCursorRect, 101.0f, 132.0f, 7.0f, 10.0f, UnitPixel);
}

void BattleScene::CalculateGameScreenRect()
{
	float originalWidth = 240.0f, originalHeight = 160.0f;
	float originalAspectRatio = originalWidth / originalHeight;
	float windowWidth = static_cast<float>(m_clientWidth), windowHeight = static_cast<float>(m_clientHeight);
	float destWidth = windowWidth, destHeight = windowWidth / originalAspectRatio;
	if (destHeight > windowHeight) 
	{
		destHeight = windowHeight;
		destWidth = windowHeight * originalAspectRatio;
	}
	float destX = (windowWidth - destWidth) / 2.0f;
	float destY = (windowHeight - destHeight) / 2.0f;
	m_gameScreenRect = RectF(destX, destY, destWidth, destHeight);
}