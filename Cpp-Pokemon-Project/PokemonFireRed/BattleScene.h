#pragma once
#include "Scene.h"
#include "Pokemon.h"
#include "BattleManager.h"
#include <vector>

// ���� ����� �ð���/�Է� ó�� ���¸� �����ϱ� ���� ������
enum class BATTLE_UI_STATE
{
	INTRO_SEQUENCE,      // ���� ���� ��Ʈ�� (�޽��� ��� ��)
	MAIN_MENU,           // �� �޴� ǥ�� (�ο��, ����, ���ϸ�, ����)
	FIGHT_MENU,          // ��� ���� �޴� ǥ��
	PROCESSING_TURN,     // �� ó�� �� (�ִϸ��̼�, �޽��� ���)
	SWITCH_POKEMON_MENU, // ���ϸ� ��ü �޴� ǥ��
	BATTLE_END           // ���� ����
};

class BattleScene : public Scene
{
public:
	BattleScene(HWND hWnd, int clientWidth, int clientHeight);
	~BattleScene() override;

	// Scene�� ���� �Լ����� ��ü������ ����
	bool Init() override;
	void Update(float deltaTime) override;
	void Render(Gdiplus::Graphics& bufferGraphics) override;
	void Release() override;
	LRESULT MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;
	
private:
	// --- ��� ���� ---
	HWND m_hWnd;
	int m_clientWidth;
	int m_clientHeight;

	// --- ���� ���� ��ü ---
	BattleManager* m_pBattleManager = nullptr;
	std::vector<Pokemon> m_playerParty;
	std::vector<Pokemon> m_opponentParty;

	// --- UI �� ���� ---
	BATTLE_UI_STATE m_currentState = BATTLE_UI_STATE::INTRO_SEQUENCE;
	Gdiplus::Bitmap* m_pBackground = nullptr;
	Gdiplus::Bitmap* m_pBattleUI = nullptr;

	// �޴� ������ ���� ����
	int m_mainMenuSelection = 0; // 0: �ο��, 1: ����, 2: ���ϸ�, 3: ����
	int m_fightMenuSelection = 0; // 0-3: ��� ����

	// ȭ�� ���� ������ ���� Rect
	Gdiplus::RectF m_gameScreenRect;
	void CalculateGameScreenRect();
};