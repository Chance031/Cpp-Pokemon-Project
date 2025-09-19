#pragma once
#include <windows.h>
#include <gdiplus.h>

class Scene
{
public:
	// 가상 소멸자: 자식 클래스가 소멸될 때 안전하게 호출됩니다.
	virtual ~Scene() {}

	// 순수 가상 함수: 이 클래스를 상속받는 모든 자식 클래스는
	// 아래 함수들을 반드시 직접 구현해야 합니다.
	virtual bool Init() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(Gdiplus::Graphics& bufferGraphics) = 0;
	virtual void Release() = 0;
	virtual LRESULT MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) = 0;
};