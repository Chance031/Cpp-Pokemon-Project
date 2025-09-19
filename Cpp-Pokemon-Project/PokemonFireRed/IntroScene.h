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

	// 이 장면이 끝났는지 Game 클래스에 알려주는 함수
	bool IsFinished() const { return m_isFinished; }

private:
	void CalculateGameScreenRect();

	// --- 멤버 변수 ---
	HWND m_hWnd;
	int m_clientWidth;
	int m_clientHeight;

	// 리소스
	Gdiplus::Bitmap* m_pIntroSheet = nullptr;

	// 상태
	IntroState m_introState = IntroState::Copyright;
	FadeState m_fadeState = FadeState::FadingIn;
	float m_sceneTimer = 0.0f;
	float m_fadeTimer = 0.0f;
	int m_fadeAlpha = 255;
	bool m_isFinished = false;

	// 애니메이션
	float m_starScrollX = 0.0f;
	Gdiplus::RectF m_gameScreenRect;
};