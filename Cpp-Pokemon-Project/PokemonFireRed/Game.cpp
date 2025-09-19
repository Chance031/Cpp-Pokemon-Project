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
	m_hWnd = hWnd;	// 윈도우 핸들 저장

	// GDI+ 초기화
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	
	// 화면 크기와 동일한 백 버퍼 비트맵 생성 (더블 버퍼링용)
	m_pBuffer = new Bitmap(m_clientWidth, m_clientHeight);

	// 게임 시작 시 필요한 모든 CSV 데이터를 미리 로드
	DataManager::GetInstance().LoadAllData();

	// 게임의 첫 장면으로 IntroScene을 생성
	m_pCurrentScene = std::make_unique<IntroScene>(m_hWnd, m_clientWidth, m_clientHeight);

	// 첫 장면의 초기화 함수를 호출, 실패시 false를 반환
	if (!m_pCurrentScene->Init()) 
	{
		return false;
	}
	return true;
}

void Game::Release()
{
	// 백 버퍼와 GDI+를 해제
	delete m_pBuffer;
	GdiplusShutdown(m_gdiplusToken);
}

void Game::Run()
{
	MSG msg = {};
	ULONGLONG lastTime = GetTickCount64();	// 마지막 업데이트 시간을 기록

	// 루프가 게임이 실행되는 동안 계속 반복
	while (true) 
	{
		// 처리할 윈도우 메시지가 있는지 확인
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			// WM_QUIT 메시지를 받으면 루프를 종료
			if (msg.message == WM_QUIT)
			{
				break;
			}
			// 그 외 메시지는 WndProc으로 전달하여 처리
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// 처리할 메시지가 없을 때 게임 로직을 실행
		else 
		{
			ULONGLONG currentTime = GetTickCount64();
			// 아주 짧은 시간(10ms) 내의 변화는 무시하여 CPU 사용량을 감소
			if (currentTime - lastTime > 10)
			{
				// 이전 프레임과 현재 프레임 사이의 시간 차이(deltaTime)를 계산
				float deltaTime = (currentTime - lastTime) / 1000.0f;
				lastTime = currentTime;

				Update(deltaTime);						// 게임 상태 업데이트
				InvalidateRect(m_hWnd, NULL, FALSE);	// 화면을 다시 그리도록 요청 (WM_PAINT 발생)
			}
		}
	}
}

void Game::Update(float deltaTime)
{
	// 현재 Scene이 존재할 경우, 해당 Scene의 Update 함수를 호출
	if (m_pCurrentScene) 
	{
		m_pCurrentScene->Update(deltaTime);

		// --- 장면 전환 로직 ---
		IntroScene* intro = dynamic_cast<IntroScene*>(m_pCurrentScene.get());
		// IntroScene이 맞고, IsFinished()가 ture를 반환했다면
		if (intro && intro->IsFinished()) 
		{
			// 기존 IntroScene을 메모리에서 해제, 새로운 BattleScene으로 교체
			ChangeScene(new BattleScene(m_hWnd, m_clientWidth, m_clientHeight));
		}
	}
}

void Game::Render(HDC hdc)
{
	// 백 버퍼(m_pBuffer)에 그림을 그릴 Graphics 객체를 생성
	Graphics bufferGraphics(m_pBuffer);

	// 픽셀 아트가 확대될 때 흐려지지 않고 각지게 보이도록 설정
	bufferGraphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	bufferGraphics.SetPixelOffsetMode(PixelOffsetModeHalf);

	// 현재 Scene에게 백 버퍼에 모든 것을 그리도록 위임
	if (m_pCurrentScene) 
	{
		m_pCurrentScene->Render(bufferGraphics);
	}

	// 백 버퍼에 그려진 완성된 이미지를 실제 화면(hdc)에 한 번에 복사
	Graphics screenGraphics(hdc);
	screenGraphics.DrawImage(m_pBuffer, 0, 0, m_clientWidth, m_clientHeight);
}

void Game::ChangeScene(Scene* newScene)
{
	// 기존에 실행 중이던 Scene이 있다면, 리소스를 해제하고 메모리에서 삭제
	if (m_pCurrentScene) 
	{
		m_pCurrentScene->Release();
		m_pCurrentScene.reset(newScene);

	// 멤버 변수가 새로운 Scene을 가리키도록 교체
	m_pCurrentScene = newScene;

	// 새로운 Scene의 초기화(Init) 함수를 호출
	if (m_pCurrentScene) 
	{
		// 만약 초기화에 실패하면, 복구할 수 없는 오류로 간주하고 프로그램을 종료
		if (!m_pCurrentScene->Init())
		{
			MessageBox(m_hWnd, L"새로운 장면을 초기화하는 데 실패했습니다.", L"치명적 오류", MB_OK);
			PostQuitMessage(1); // 에러 코드 1과 함께 종료 메시지 전송
		}
	}
}

LRESULT Game::MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	// 모든 메시지는 일단 현재 Scene에게 먼저 전달하여 처리할 기회 제공
	if (m_pCurrentScene)
	{
		LRESULT result = m_pCurrentScene->MsgProc(hWnd, iMessage, wParam, lParam);
		// Scene이 메시지를 처리했다면 (0이 아닌 값을 반환 시), 그 결과를 즉시 반환
		if (result != 0)
		{
			return result;
		}
	}

	// Scene에서 처리하지 않은 메시지만 Game 클래스가 직접 처리
	switch (iMessage)
	{
	// 화면을 다시 그려야 할 때 (InvalidateRect가 호출될 때) 발생하는 가장 중요한 메시지
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);	// 그리기 시작하고 Device Context를 얻음
		Render(hdc);						// Render 함수 호출
		EndPaint(hWnd, &ps);				// 그리기 종료
		return 0;
	}
	// 창 크기가 변경되었을 때
	case WM_SIZE:
	{
		m_clientWidth = LOWORD(lParam);							// 새 너비
		m_clientHeight = HIWORD(lParam);						// 새 높이
		delete m_pBuffer;										// 이전 크기의 백 버퍼 삭제
		m_pBuffer = new Bitmap(m_clientWidth, m_clientHeight);	// 새 크기로 백 버퍼 재생성
		return 0;
	}
	// 창이 닫히려 할 때
	case WM_DESTROY:
	{
		PostQuitMessage(0);	// 게임 루프를 종료하기 위해 WM_QUIT 메시지를 보냄
		return 0;
	}
	}

	// 위에서 처리하지 않은 나머지 모든 메시지는 Windows의 기본 처리기에 맡김
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}