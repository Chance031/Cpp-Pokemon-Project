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
	// ���� ������ ��� ��� ����
	m_pBackground = new Bitmap(L"../assets/images/battle/background_grass.png");
	m_pBattleUI = new Bitmap(L"../assets/images/ui/battle_ui_sheet.png");
	if (m_pBackground->GetLastStatus() != Ok || m_pBattleUI->GetLastStatus() != Ok)
	{
		MessageBox(m_hWnd, L"��Ʋ ���ҽ��� �ҷ��� �� �����ϴ�.", L"���ҽ� ����", MB_OK);
		return false;
	}

	// �׽�Ʈ�� ���ϸ� ��Ƽ ����
	const PokemonSpecies& species1 = DataManager::GetInstance().GetPokemonSpecies(1);
	Pokemon p1(species1, { 5 });
	p1.LearnMove(DataManager::GetInstance().GetMoveData(33));
	p1.LearnMove(DataManager::GetInstance().GetMoveData(45));
	m_playerParty.push_back(p1);

	const PokemonSpecies& species2 = DataManager::GetInstance().GetPokemonSpecies(25);
	Pokemon p2(species2, { 5 });
	p2.LearnMove(DataManager::GetInstance().GetMoveData(85));
	m_opponentParty.push_back(p2);

	// BattleManager ����
	m_pBattleManager = new BattleManager(m_playerParty, m_opponentParty);

	// ���� ���� �̺�Ʈ�� �����ϰ� ť�� ����
	std::vector<TurnEvent> startEvents;
	m_pBattleManager->Start(startEvents);
	for (const auto& event : startEvents) 
	{
		m_eventQueue.push(event);
	}
	std::string startMessage = "�߻��� " + m_pBattleManager->GetOpponentActivePokemon()->GetName() + "��(��) ��Ÿ����!";
	m_eventQueue.push({ TurnEventType::TEXT_MESSAGE, nullptr, startMessage });

	// HP �ִϸ��̼��� ���� �ð��� HP ���� �ʱ�ȭ
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
}

void BattleScene::Update(float deltaTime)
{
	// ���� �̺�Ʈ�� ó�� ���̸�, �ش� ���� ������Ʈ �Լ� ȣ��
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
			// ������ �̺�Ʈ ��ü�� ������ �����Ƿ� �ٷ� �÷��׸� false�� ����.
			// ���� HP ���� �ִϸ��̼��� �Ʒ��� UpdateHpAnimation���� �׻� ó����.
			m_isProcessingEvent = false;
			break;
		}
	}
	// ó�� ���� �̺�Ʈ�� ���� ť�� ���� �̺�Ʈ�� �ִٸ�, ���� �̺�Ʈ ó�� ����
	else if (!m_eventQueue.empty())
	{
		ProcessNextEvent();
	}
	// ��� �̺�Ʈ ó���� ������, �÷��̾��� �Է��� ��ٸ��� ���·� ��ȯ
	else
	{
		m_currentState = BATTLE_UI_STATE::AWAITING_INPUT;
	}

	// HP �ִϸ��̼��� �̺�Ʈ ó���� ������ �� ������ �ε巴�� ����
	UpdateHpAnimation(deltaTime);
}

void BattleScene::Render(Gdiplus::Graphics& bufferGraphics)
{
	bufferGraphics.Clear(Color::Black);
	bufferGraphics.SetInterpolationMode(InterpolationModeNearestNeighbor);	// �ȼ� ��Ʈ�� ����
	bufferGraphics.SetPixelOffsetMode(PixelOffsetModeHalf);

	// �� �κ��� �׸��� ���� �Լ� ȣ��
	RenderUI(bufferGraphics);
	RenderPokemonSprites(bufferGraphics);
	RenderHpBars(bufferGraphics);
	RenderText(bufferGraphics);

	// ��� ���� �޴� ������ ���� �߰��� �׸�
	if (m_currentState == BATTLE_UI_STATE::FIGHT_MENU)
	{
		RenderFightMenu(bufferGraphics);
	}
}

LRESULT BattleScene::MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (iMessage != WM_KEYDOWN) return 0;

	// --- ����(Ȯ��) Ű ó�� ---
	if (wParam == VK_RETURN)
	{
		// �̺�Ʈ ���� �߿� ���͸� ���� ���
		if (m_isProcessingEvent)
		{
			// �޼��� ����� ������ �ʾҴٸ�, ��� ��ü �޽����� ������ (��ŵ)
			if (!m_isMessageFinished)
			{
				m_isMessageFinished = true;
				m_visibleMessage = m_fullMessageToShow;
			}
			// �޽��� ����� �����ٸ�, ���� �̺�Ʈ ó���� �����ϰ� �������� �Ѿ �غ�
			else
			{
				m_isProcessingEvent = false;
			}
		}
		// �÷��̾��� �� �޴� �Է��� ��ٸ� ��
		else if (m_currentState == BATTLE_UI_STATE::AWAITING_INPUT)
		{
			// '�ο��' ����
			if (m_mainMenuSelection == 0)
			{
				m_currentState = BATTLE_UI_STATE::FIGHT_MENU;
				m_fightMenuSelection = 0;
			}
		}
		// ��� ���� �޴� �Է��� ��ٸ� ��
		else if (m_currentState == BATTLE_UI_STATE::FIGHT_MENU)
		{
			Pokemon* playerMon = m_pBattleManager->GetPlayerActivePokemon();
			const auto& moves = playerMon->GetMoveset();
			if (m_fightMenuSelection < moves.size())
			{
				// ������ ����� BattleAction�� ����� BattleManager�� �� ó���� ��û
				BattleAction action = { PlayerActionType::FIGHT, &playerMon->GetMovesetForModify()[m_fightMenuSelection] };
				TurnResult turnResult = m_pBattleManager->ProcessTurn(action);
				
				// ��ȯ�� ��� �̺�Ʈ�� ť�� �߰�
				for (const auto& event : turnResult.events) {
					m_eventQueue.push(event);
				}

				// �ٽ� �̺�Ʈ ó�� ���·� ��ȯ
				m_currentState = BATTLE_UI_STATE::EVENT_PROCESSING;
			}
		}
		return 0;
	}
	// --- ����Ű �� ��Ÿ Ű ó�� ---
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
	// ť�� ����ٸ�, ��� ������ �������Ƿ� �Է� ��� ���·�
	if (m_eventQueue.empty())
	{
		m_isProcessingEvent = false;
		m_currentState = BATTLE_UI_STATE::AWAITING_INPUT;
		return;
	}

	// ť���� ���� �̺�Ʈ�� ����
	m_currentEvent = m_eventQueue.front();
	m_eventQueue.pop();
	m_isProcessingEvent = true;
	m_currentState = BATTLE_UI_STATE::EVENT_PROCESSING;

	// �̺�Ʈ Ÿ�Կ� ���� ���⿡ �ʿ��� �������� �ʱ�ȭ
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
		// ������ �̺�Ʈ�� �޽��� ��� ���� HP �ִϸ��̼Ǹ� �����Ƿ�, �ٷ� ó���� ����
		m_isProcessingEvent = false;
		break;
	}
	}
}

void BattleScene::UpdateTextMessage(float deltaTime)
{
	if (m_isMessageFinished) return;
	m_textAnimationTimer += deltaTime;

	// 0.05�ʸ��� �� ���ھ�
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
			// �޽��� ����� ��� ������ �÷��� ����
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
	
	// �ʴ� 40 HP�� �ӵ��� ��ȭ
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
	FontFamily fontFamily(L"����");
	Font font(&fontFamily, 10, FontStyleRegular, UnitPixel);
	SolidBrush brush(Color(255, 80, 80, 80));
	StringFormat format;
	format.SetAlignment(StringAlignmentNear);
	PointF pointF(m_gameScreenRect.X + 20.0f, m_gameScreenRect.Y + m_gameScreenRect.Height - 35.0f);

	if (m_currentState == BATTLE_UI_STATE::AWAITING_INPUT) 
	{
		g.DrawString(L"������ �ұ�?", -1, &font, pointF, &format, &brush);
	}
	else if (m_isProcessingEvent && !m_visibleMessage.empty()) 
	{
		g.DrawString(m_visibleMessage.c_str(), -1, &font, pointF, &format, &brush);
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
	// TODO: ���ϸ� ��������Ʈ �׸���
}

void BattleScene::RenderFightMenu(Gdiplus::Graphics& g)
{
	RectF destFightMenuBox(m_gameScreenRect.X, m_gameScreenRect.Y + m_gameScreenRect.Height - 48.0f, 240.0f, 48.0f);
	g.DrawImage(m_pBattleUI, destFightMenuBox, 122.0f, 0.0f, 118.0f, 48.0f, UnitPixel);

	FontFamily fontFamily(L"����");
	Font font(&fontFamily, 10, FontStyleRegular, UnitPixel);
	SolidBrush brush(Color(255, 80, 80, 80));
	const auto& moves = m_pBattleManager->GetPlayerActivePokemon()->GetMoveset();
	for (size_t i = 0; i < moves.size(); ++i) 
	{
		std::wstring moveName(moves[i].GetName().begin(), moves[i].GetName().end());
		float x = m_gameScreenRect.X + 25.0f + (i % 2) * 80.0f;
		float y = m_gameScreenRect.Y + m_gameScreenRect.Height - 35.0f + (i / 2) * 16.0f;
		g.DrawString(moveName.c_str(), -1, &font, PointF(x, y), &brush);
	}

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