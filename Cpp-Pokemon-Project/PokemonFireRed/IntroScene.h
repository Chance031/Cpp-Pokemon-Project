#pragma once
#include "Scene.h"
#include "GameTypes.h"
#include <random>

class IntroScene : public Scene
{
public:
	IntroScene(HWND hWnd, int clientWidth, int clientHeight);
	~IntroScene() override;

	bool Init() override;
	void Update(float deltaTime) override;
	void Render(Gdiplus::Graphics& bufferGraphics) override;
	void Release() override;
	LRESULT MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) override;

	// �� ����� �������� Game Ŭ������ �˷��ִ� �Լ�
	bool IsFinished() const { return m_isFinished; }

private:
	void CalculateGameScreenRect();

	// --- ��� ���� ---
	HWND m_hWnd;
	int m_clientWidth;
	int m_clientHeight;

	// ���ҽ�
	Gdiplus::Bitmap* m_pIntroSheet = nullptr;

	// ����
	IntroState m_introState = IntroState::Copyright;
	FadeState m_fadeState = FadeState::FadingIn;
	float m_sceneTimer = 0.0f;
	float m_fadeTimer = 0.0f;
	int m_fadeAlpha = 255;
	bool m_isFinished = false;

	// �ִϸ��̼�
	float m_starScrollX = 0.0f;
	Gdiplus::RectF m_gameScreenRect;
};