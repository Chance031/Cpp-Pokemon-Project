#include "IntroScene.h"

using namespace Gdiplus;

IntroScene::IntroScene(HWND hWnd, int clientWidth, int clientHeight)
	: m_hWnd(hWnd), m_clientWidth(clientWidth), m_clientHeight(clientHeight)
{
}

IntroScene::~IntroScene()
{
}

bool IntroScene::Init()
{
	m_pIntroSheet = new Bitmap(L"assets/images/ui/intro_sheet.png");
	if (m_pIntroSheet->GetLastStatus() != Ok)
	{
		MessageBox(m_hWnd, L"인트로 이미지를 불러올 수 없습니다.", L"리소스 오류", MB_OK);
		return false;
	}
	CalculateGameScreenRect();
	return true;
}

void IntroScene::Update(float deltaTime)
{
	if (m_fadeState != FadeState::None)
	{
		m_fadeTimer += deltaTime;
		float fadeDuration = (m_fadeState == FadeState::FadingIn) ? 0.7f : 1.0f;
		if (m_fadeState == FadeState::FadingIn) {
			m_fadeAlpha = 255 - static_cast<int>((m_fadeTimer / fadeDuration) * 255.0f);
			if (m_fadeAlpha <= 0) {
				m_fadeAlpha = 0;
				m_fadeState = FadeState::None;
				m_sceneTimer = 0.0f;
			}
		}
		else { // FadingOut
			m_fadeAlpha = static_cast<int>((m_fadeTimer / fadeDuration) * 255.0f);
			if (m_fadeAlpha >= 255) {
				m_fadeAlpha = 255;
				m_fadeState = FadeState::FadingIn;
				m_fadeTimer = 0.0f;

				// 다음 인트로 단계로 전환
				if (m_introState == IntroState::Copyright) m_introState = IntroState::GameFreakPresents;
				else if (m_introState == IntroState::GameFreakPresents) m_introState = IntroState::GengarVsNidorino;
				else if (m_introState == IntroState::GengarVsNidorino) m_introState = IntroState::TitleScreen;
				else if (m_introState == IntroState::TitleScreen) m_isFinished = true; // 인트로 끝!
			}
		}
	}
	else
	{
		m_sceneTimer += deltaTime;
		switch (m_introState)
		{
		case IntroState::Copyright:
			if (m_sceneTimer > 2.0f) { m_fadeState = FadeState::FadingOut; m_fadeTimer = 0.0f; }
			break;
		case IntroState::GameFreakPresents:
			m_starScrollX -= 50.0f * deltaTime;
			if (m_starScrollX < -m_gameScreenRect.Width) { m_starScrollX += m_gameScreenRect.Width; }
			if (m_sceneTimer > 3.0f) { m_fadeState = FadeState::FadingOut; m_fadeTimer = 0.0f; }
			break;
		case IntroState::GengarVsNidorino:
			if (m_sceneTimer > 5.0f) { m_fadeState = FadeState::FadingOut; m_fadeTimer = 0.0f; }
			break;
		case IntroState::TitleScreen:
			// 키 입력 대기
			break;
		}
	}
}

void IntroScene::Render(Gdiplus::Graphics& bufferGraphics)
{
	IntroState stateToRender = m_introState;
	if (m_fadeState == FadeState::FadingIn) {
		if (m_introState == IntroState::Copyright) stateToRender = IntroState::Copyright;
		else if (m_introState == IntroState::GameFreakPresents) stateToRender = IntroState::GameFreakPresents;
		else if (m_introState == IntroState::GengarVsNidorino) stateToRender = IntroState::GengarVsNidorino;
		else if (m_introState == IntroState::TitleScreen) stateToRender = IntroState::TitleScreen;
	}

	switch (stateToRender)
	{
	case IntroState::Copyright:
		bufferGraphics.DrawImage(m_pIntroSheet, m_gameScreenRect, 8, 42, 240, 160, UnitPixel);
		break;
	case IntroState::GameFreakPresents:
		bufferGraphics.DrawImage(m_pIntroSheet, m_gameScreenRect, 8, 203, 240, 160, UnitPixel);
		// 별 그리기 로직
		break;
	case IntroState::GengarVsNidorino:
		bufferGraphics.DrawImage(m_pIntroSheet, m_gameScreenRect, 0, 0, 240, 160, UnitPixel);
		break;
	case IntroState::TitleScreen:
		bufferGraphics.DrawImage(m_pIntroSheet, m_gameScreenRect, 251, 0, 240, 160, UnitPixel);
		break;
	}

	if (m_fadeState != FadeState::None) {
		Color fadeColor = (m_fadeState == FadeState::FadingIn) ? Color(m_fadeAlpha, 255, 255, 255) : Color(m_fadeAlpha, 0, 0, 0);
		SolidBrush fadeBrush(fadeColor);
		bufferGraphics.FillRectangle(&fadeBrush, 0, 0, m_clientWidth, m_clientHeight);
	}
}

void IntroScene::Release()
{
	delete m_pIntroSheet;
	m_pIntroSheet = nullptr;
}

LRESULT IntroScene::MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
		if (m_introState == IntroState::TitleScreen && wParam == VK_RETURN)
		{
			m_isFinished = true;
		}
		return 0;
	}
	return 0;
}

void IntroScene::CalculateGameScreenRect()
{
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