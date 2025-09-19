#include "BattleScene.h"
#include "DataManager.h" // ���ϸ� ������ ���� �ʿ�

using namespace Gdiplus;

BattleScene::BattleScene(HWND hWnd, int clientWidth, int clientHeight)
	: m_hWnd(hWnd), m_clientWidth(clientWidth), m_clientHeight(clientHeight)
{
}

BattleScene::~BattleScene()
{
	// Release �Լ����� ��� ���� �Ҵ� ������ ó���մϴ�.
}

bool BattleScene::Init()
{
	// 1. ���ҽ� �ε�
	m_pBackground = new Bitmap(L"assets/images/battle/background_grass.png");
	m_pBattleUI = new Bitmap(L"assets/images/ui/battle_ui_sheet.png");

	if (m_pBackground->GetLastStatus() != Ok || m_pBattleUI->GetLastStatus() != Ok)
	{
		MessageBox(m_hWnd, L"��Ʋ ���ҽ��� �ҷ��� �� �����ϴ�.", L"���ҽ� ����", MB_OK);
		return false;
	}

	// 2. main.cpp�� ���ϸ� ���� ������ �̰����� ����
	// �÷��̾� ��Ƽ ���� (�̻��ؾ�, ����)
	const PokemonSpecies& bulbasaurSpecies = DataManager::GetInstance().GetPokemonSpecies(1);
	PokemonIndividual bulbasaurIndividual;
	bulbasaurIndividual.level = 5;
	// (IV/EV ������ �����ϰ� ����, ���� ���ӿ����� ���� �Ǵ� ������ ���� �ʿ�)
	Pokemon bulbasaur(bulbasaurSpecies, bulbasaurIndividual);
	bulbasaur.LearnMove(DataManager::GetInstance().GetMoveData(33)); // �����ġ��
	bulbasaur.LearnMove(DataManager::GetInstance().GetMoveData(45)); // �����Ҹ�

	const PokemonSpecies& pidgeySpecies = DataManager::GetInstance().GetPokemonSpecies(16);
	PokemonIndividual pidgeyIndividual;
	pidgeyIndividual.level = 5;
	Pokemon pidgey(pidgeySpecies, pidgeyIndividual);
	pidgey.LearnMove(DataManager::GetInstance().GetMoveData(33)); // �����ġ��

	m_playerParty.push_back(bulbasaur);
	m_playerParty.push_back(pidgey);

	// ���(�߻�) ��Ƽ ���� (�ƺ�ũ)
	const PokemonSpecies& arbokSpecies = DataManager::GetInstance().GetPokemonSpecies(24);
	PokemonIndividual arbokIndividual;
	arbokIndividual.level = 5;
	Pokemon arbok(arbokSpecies, arbokIndividual);
	arbok.LearnMove(DataManager::GetInstance().GetMoveData(33));

	m_opponentParty.push_back(arbok);

	// 3. BattleManager ���� �� �ʱ�ȭ
	m_pBattleManager = new BattleManager(m_playerParty, m_opponentParty);
	// m_pBattleManager->Start(); // <- �ܼ� ������ ���� ������ ���� ȣ������ ����

	CalculateGameScreenRect();
	m_currentState = BATTLE_UI_STATE::MAIN_MENU; // �ӽ÷� �ٷ� �޴��� ������ ����
	return true;
}

void BattleScene::Update(float deltaTime)
{
	// ����� ���������, ���߿� �ִϸ��̼�, �޽��� ��� Ÿ�̹� ���� ���⼭ �����մϴ�.
	switch (m_currentState)
	{
	case BATTLE_UI_STATE::MAIN_MENU:
		// Ű �Է� ó���� MsgProc����
		break;
	case BATTLE_UI_STATE::FIGHT_MENU:
		// Ű �Է� ó���� MsgProc����
		break;
		// ... �ٸ� ���µ� ó��
	}
}

void BattleScene::Render(Gdiplus::Graphics& bufferGraphics)
{
	// ���������� ��� �ʱ�ȭ
	bufferGraphics.Clear(Color::Black);

	// 1. ���� ��� �׸���
	bufferGraphics.DrawImage(m_pBackground, m_gameScreenRect, 0, 0, 240, 112, UnitPixel);

	// 2. UI �׸��� (���/�÷��̾� ����â, �޽��� �ڽ� ��)
	// ��� HP �ڽ�
	bufferGraphics.DrawImage(m_pBattleUI, m_gameScreenRect.X + 10, m_gameScreenRect.Y + 20, 120, 35, 0, 0, 120, 35, UnitPixel);
	// �÷��̾� HP �ڽ�
	bufferGraphics.DrawImage(m_pBattleUI, m_gameScreenRect.X + m_gameScreenRect.Width - 130, m_gameScreenRect.Y + m_gameScreenRect.Height - 80, 120, 45, 0, 35, 120, 45, UnitPixel);
	// �޽��� �ڽ�
	bufferGraphics.DrawImage(m_pBattleUI, m_gameScreenRect.X, m_gameScreenRect.Y + m_gameScreenRect.Height - 48, 240, 48, 0, 84, 240, 48, UnitPixel);

	// 3. ���� UI ���¿� ���� �޴� �׸���
	if (m_currentState == BATTLE_UI_STATE::MAIN_MENU)
	{
		// "�ο��", "����" �� �޴� �ɼ� �׸���
		bufferGraphics.DrawImage(m_pBattleUI, m_gameScreenRect.X + m_gameScreenRect.Width - 100, m_gameScreenRect.Y + m_gameScreenRect.Height - 50, 100, 50, 0, 132, 100, 50, UnitPixel);

		// ���� ���õ� �޴��� Ŀ��(ȭ��ǥ) �׸���
		// m_mainMenuSelection ���� ���� Ŀ�� ��ġ�� �ٸ��� �׷��� ��
		// ��: 0(�ο��)�� ���� 2(���ϸ�)�� �� Ŀ���� Y ��ǥ�� �ٸ�
		int cursorX = static_cast<int>(m_gameScreenRect.X + m_gameScreenRect.Width - 95);
		int cursorY = static_cast<int>(m_gameScreenRect.Y + m_gameScreenRect.Height - 38);
		if (m_mainMenuSelection >= 2) // ���ϸ�, ����
		{
			cursorY += 16;
		}
		if (m_mainMenuSelection % 2 != 0) // ����, ����
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
			case VK_RETURN: // Enter Ű
				// TODO: ������ �޴��� ���� ���� ���·� ��ȯ�ϴ� ����
				// ��: m_mainMenuSelection == 0 �̸� m_currentState = BATTLE_UI_STATE::FIGHT_MENU;
				break;
			}
			return 0;
		}
	}
	return 0;
}

void BattleScene::CalculateGameScreenRect()
{
	// IntroScene�� ������ GBA ȭ�� ���� ���� ����
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