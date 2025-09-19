#include "framework.h"
#include "Game.h"
#include "IntroScene.h"
#include "BattleScene.h"
#include "DataManager.h"

using namespace Gdiplus;

Game::Game() {}
Game::~Game() {}

bool Game::Init(HWND hWnd)
{
	m_hWnd = hWnd;	// ������ �ڵ� ����

	// GDI+ �ʱ�ȭ
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	
	// ȭ�� ũ��� ������ �� ���� ��Ʈ�� ���� (���� ���۸���)
	m_pBuffer = new Bitmap(m_clientWidth, m_clientHeight);

	// ���� ���� �� �ʿ��� ��� CSV �����͸� �̸� �ε�
	DataManager::GetInstance().LoadAllData();

	// ������ ù ������� IntroScene�� ����
	m_pCurrentScene = std::make_unique<IntroScene>(m_hWnd, m_clientWidth, m_clientHeight);

	// ù ����� �ʱ�ȭ �Լ��� ȣ��, ���н� false�� ��ȯ
	if (!m_pCurrentScene->Init()) 
	{
		return false;
	}
	return true;
}

void Game::Release()
{
	// �� ���ۿ� GDI+�� ����
	delete m_pBuffer;
	GdiplusShutdown(m_gdiplusToken);
}

void Game::Run()
{
	MSG msg = {};
	ULONGLONG lastTime = GetTickCount64();	// ������ ������Ʈ �ð��� ���

	// ������ ������ ����Ǵ� ���� ��� �ݺ�
	while (true) 
	{
		// ó���� ������ �޽����� �ִ��� Ȯ��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			// WM_QUIT �޽����� ������ ������ ����
			if (msg.message == WM_QUIT)
			{
				break;
			}
			// �� �� �޽����� WndProc���� �����Ͽ� ó��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// ó���� �޽����� ���� �� ���� ������ ����
		else 
		{
			ULONGLONG currentTime = GetTickCount64();
			// ���� ª�� �ð�(10ms) ���� ��ȭ�� �����Ͽ� CPU ��뷮�� ����
			if (currentTime - lastTime > 10)
			{
				// ���� �����Ӱ� ���� ������ ������ �ð� ����(deltaTime)�� ���
				float deltaTime = (currentTime - lastTime) / 1000.0f;
				lastTime = currentTime;

				Update(deltaTime);						// ���� ���� ������Ʈ
				InvalidateRect(m_hWnd, NULL, FALSE);	// ȭ���� �ٽ� �׸����� ��û (WM_PAINT �߻�)
			}
		}
	}
}

void Game::Update(float deltaTime)
{
	// ���� Scene�� ������ ���, �ش� Scene�� Update �Լ��� ȣ��
	if (m_pCurrentScene) 
	{
		m_pCurrentScene->Update(deltaTime);

		// --- ��� ��ȯ ���� ---
		IntroScene* intro = dynamic_cast<IntroScene*>(m_pCurrentScene.get());
		// IntroScene�� �°�, IsFinished()�� ture�� ��ȯ�ߴٸ�
		if (intro && intro->IsFinished()) 
		{
			// ���� IntroScene�� �޸𸮿��� ����, ���ο� BattleScene���� ��ü
			ChangeScene(new BattleScene(m_hWnd, m_clientWidth, m_clientHeight));
		}
	}
}

void Game::Render(HDC hdc)
{
	// �� ����(m_pBuffer)�� �׸��� �׸� Graphics ��ü�� ����
	Graphics bufferGraphics(m_pBuffer);

	// �ȼ� ��Ʈ�� Ȯ��� �� ������� �ʰ� ������ ���̵��� ����
	bufferGraphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	bufferGraphics.SetPixelOffsetMode(PixelOffsetModeHalf);

	// ���� Scene���� �� ���ۿ� ��� ���� �׸����� ����
	if (m_pCurrentScene) 
	{
		m_pCurrentScene->Render(bufferGraphics);
	}

	// �� ���ۿ� �׷��� �ϼ��� �̹����� ���� ȭ��(hdc)�� �� ���� ����
	Graphics screenGraphics(hdc);
	screenGraphics.DrawImage(m_pBuffer, 0, 0, m_clientWidth, m_clientHeight);
}

void Game::ChangeScene(Scene* newScene)
{
	// ������ ���� ���̴� Scene�� �ִٸ�, ���ҽ��� �����ϰ� �޸𸮿��� ����
	if (m_pCurrentScene) 
	{
		m_pCurrentScene->Release();
		m_pCurrentScene.reset(newScene);

	// ��� ������ ���ο� Scene�� ����Ű���� ��ü
	m_pCurrentScene = newScene;

	// ���ο� Scene�� �ʱ�ȭ(Init) �Լ��� ȣ��
	if (m_pCurrentScene) 
	{
		// ���� �ʱ�ȭ�� �����ϸ�, ������ �� ���� ������ �����ϰ� ���α׷��� ����
		if (!m_pCurrentScene->Init())
		{
			MessageBox(m_hWnd, L"���ο� ����� �ʱ�ȭ�ϴ� �� �����߽��ϴ�.", L"ġ���� ����", MB_OK);
			PostQuitMessage(1); // ���� �ڵ� 1�� �Բ� ���� �޽��� ����
		}
	}
}

LRESULT Game::MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	// ��� �޽����� �ϴ� ���� Scene���� ���� �����Ͽ� ó���� ��ȸ ����
	if (m_pCurrentScene)
	{
		LRESULT result = m_pCurrentScene->MsgProc(hWnd, iMessage, wParam, lParam);
		// Scene�� �޽����� ó���ߴٸ� (0�� �ƴ� ���� ��ȯ ��), �� ����� ��� ��ȯ
		if (result != 0)
		{
			return result;
		}
	}

	// Scene���� ó������ ���� �޽����� Game Ŭ������ ���� ó��
	switch (iMessage)
	{
	// ȭ���� �ٽ� �׷��� �� �� (InvalidateRect�� ȣ��� ��) �߻��ϴ� ���� �߿��� �޽���
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);	// �׸��� �����ϰ� Device Context�� ����
		Render(hdc);						// Render �Լ� ȣ��
		EndPaint(hWnd, &ps);				// �׸��� ����
		return 0;
	}
	// â ũ�Ⱑ ����Ǿ��� ��
	case WM_SIZE:
	{
		m_clientWidth = LOWORD(lParam);							// �� �ʺ�
		m_clientHeight = HIWORD(lParam);						// �� ����
		delete m_pBuffer;										// ���� ũ���� �� ���� ����
		m_pBuffer = new Bitmap(m_clientWidth, m_clientHeight);	// �� ũ��� �� ���� �����
		return 0;
	}
	// â�� ������ �� ��
	case WM_DESTROY:
	{
		PostQuitMessage(0);	// ���� ������ �����ϱ� ���� WM_QUIT �޽����� ����
		return 0;
	}
	}

	// ������ ó������ ���� ������ ��� �޽����� Windows�� �⺻ ó���⿡ �ñ�
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}