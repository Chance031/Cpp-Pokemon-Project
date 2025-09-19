#pragma once

#include "Scene.h"

#include <memory>

class Game
{
public:
	// --- ������ �� �Ҹ��� ---
	Game();
	~Game();

	// --- ���� �������̽� ---
	bool Init(HWND hWnd);	// ���ӿ� �ʿ��� ��� ���ҽ��� GDI+�� �ʱ�ȭ
	void Run();				// ���� ���� ������ �����ϰ� ����
	void Release();			// ������ ����� �� ���� ��� ���ҽ��� ����
	LRESULT MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);	// ������ �޽����� ó���ϴ� ���ν���

private:
	// --- ���� ���� �Լ� ---
	void Update(float deltaTime);			// ������ ���¸� ������Ʈ (�� ������ ȣ��)
	void Render(HDC hdc);					// ���� ȭ���� �׸� (�� ������ ȣ��)
	void ChangeScene(Scene* newScene);		// ���� ����� �ٸ� ������� ��ü

	// --- ��� ���� ---
	HWND m_hWnd = NULL;						// ���� ������ �ڵ�
	int m_clientWidth = 800;				// Ŭ���̾�Ʈ ������ �ʺ�
	int m_clientHeight = 600;				// Ŭ���̾�Ʈ ������ ����

	ULONG_PTR m_gdiplusToken = 0;			// GDI+ ���̺귯�� ����� ���� ��ū
	Gdiplus::Bitmap* m_pBuffer = nullptr;	// ���� ���۸��� ���� �� ���� ��Ʈ��
	
	std::unique_ptr<Scene> m_pCurrentScene;	// ���� Ȱ��ȭ�� ���(Scene)�� ����Ű�� ������
};