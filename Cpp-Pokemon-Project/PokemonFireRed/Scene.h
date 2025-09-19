#pragma once
#include <windows.h>
#include <gdiplus.h>

class Scene
{
public:
	// ���� �Ҹ���: �ڽ� Ŭ������ �Ҹ�� �� �����ϰ� ȣ��˴ϴ�.
	virtual ~Scene() {}

	// ���� ���� �Լ�: �� Ŭ������ ��ӹ޴� ��� �ڽ� Ŭ������
	// �Ʒ� �Լ����� �ݵ�� ���� �����ؾ� �մϴ�.
	virtual bool Init() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(Gdiplus::Graphics& bufferGraphics) = 0;
	virtual void Release() = 0;
	virtual LRESULT MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) = 0;
};