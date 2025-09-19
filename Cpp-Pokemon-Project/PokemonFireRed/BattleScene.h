#pragma once

#include <queue>

#include "BattleManager.h"
#include "Pokemon.h"
#include "Scene.h"

// ���� ����� �ð���/�Է� ó�� ���¸� �����ϱ� ���� ������
enum class BATTLE_UI_STATE
{
	EVENT_PROCESSING,    // �̺�Ʈ ť�� �ִ� �̺�Ʈ�� ó�� ���� ����
	AWAITING_INPUT,      // �÷��̾��� �� �޴� �Է��� ��ٸ��� ����
	FIGHT_MENU,          // ��� ���� �޴� �Է��� ��ٸ��� ����
	BATTLE_END           // ���� ���� ����
};

class BattleScene : public Scene
{
public:
	// --- ������ �� �Ҹ��� ---
	BattleScene(HWND hWnd, int clientWidth, int clientHeight);
	~BattleScene() override;

	// Scene�� ���� �Լ����� ��ü������ ����
	bool Init() override;
	void Update(float deltaTime) override;
	void Render(Gdiplus::Graphics& bufferGraphics) override;
	void Release() override;
	LRESULT MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;
	
private:
	// --- ���� �� �̺�Ʈ ó�� �ý��� ---
	void ProcessNextEvent();					// �̺�Ʈ ť���� ���� �̺�Ʈ�� ���� ó���� �����մϴ�.
	void UpdateTextMessage(float deltaTime);	// �޽��� ��� ������ ������Ʈ�մϴ�.
	void UpdateHpAnimation(float deltaTime);	// HP �� ���� �ִϸ��̼��� ������Ʈ�մϴ�.

	// --- ������ ����� �Լ� ---
	void RenderUI(Gdiplus::Graphics& g);
	void RenderText(Gdiplus::Graphics& g);
	void RenderHpBars(Gdiplus::Graphics& g);
	void RenderPokemonSprites(Gdiplus::Graphics& g);
	void RenderFightMenu(Gdiplus::Graphics& g);
	void RenderMainMenu(Gdiplus::Graphics& g);

	// --- ���� ���� ��� ---
	void CalculateGameScreenRect();

	// --- ��� ���� ---
	HWND m_hWnd;
	int m_clientWidth;
	int m_clientHeight;

	// --- ���� ���� ��ü ---
	BattleManager* m_pBattleManager = nullptr;
	std::vector<Pokemon> m_playerParty;
	std::vector<Pokemon> m_opponentParty;

	// --- UI �� ���� ---
	BATTLE_UI_STATE m_currentState = BATTLE_UI_STATE::EVENT_PROCESSING;
	Gdiplus::RectF m_gameScreenRect;
	Gdiplus::Bitmap* m_pBackground = nullptr;
	Gdiplus::Bitmap* m_pBattleUI = nullptr;
	int m_mainMenuSelection = 0;
	int m_fightMenuSelection = 0;

	// --- �̺�Ʈ ó�� �ý��� ---
	std::queue<TurnEvent> m_eventQueue;		// �߻��� �̺�Ʈ�� ������� �����ϴ� ť
	TurnEvent m_currentEvent;				// ���� ó��(����)�ϰ� �ִ� �̺�Ʈ
	bool m_isProcessingEvent = false;		// �̺�Ʈ ó�� ������ ���θ� ��Ÿ���� �÷���

	// --- �ؽ�Ʈ ����� ���� ---
	std::wstring m_fullMessageToShow;		// �޽��� �ڽ��� ǥ���� ��ü �޽���
	std::wstring m_visibleMessage;			// ���� ȭ�鿡 ���̴� �κ� �޽���
	size_t m_messageCharIndex = 0;			// Ÿ���� ȿ���� ���� ���� ���� �ε���
	float m_textAnimationTimer = 0.0f;		// ���� Ÿ���� �ӵ� ���� Ÿ�̸�
	bool m_isMessageFinished = false;		// ���� �޽��� ����� �Ϸ�Ǿ����� ����

	// --- HP �ִϸ��̼ǿ� ���� ---
	float m_playerVisualHp = 0;				// ȭ�鿡 '��������' �÷��̾� ���ϸ��� HP
	float m_opponentVisualHp = 0;			// ȭ�鿡 '��������' ��� ���ϸ��� HP
};