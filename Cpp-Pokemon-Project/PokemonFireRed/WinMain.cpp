#include <Windows.h>

#include "Game.h"

// --- 전역 변수 ---
Game* g_game = nullptr;	// 윈도우 프로시저에서 Game 객체에 접근하기 위한 포인터

// --- 윈도우 프로시저 ---
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (g_game)
	{
		return g_game->MsgProc(hWnd, iMessage, wParam, lParam);
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

// --- 프로그램 시작점 ---
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // 1. 윈도우 클래스 등록
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"PokemonFireRed";
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Critical Error", MB_ICONERROR);
        return 0;
    }
    //MessageBox(NULL, L"Checkpoint 1: Class Registered", L"Debug", MB_OK);


    // 2. 윈도우 생성
    HWND hWnd = CreateWindow(L"PokemonFireRed", L"포켓몬스터 파이어레드", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Critical Error", MB_ICONERROR);
        return 0;
    }
    //MessageBox(NULL, L"Checkpoint 2: Window Created", L"Debug", MB_OK);


    // 3. Game 객체 생성 및 초기화
    g_game = new Game();
    //MessageBox(NULL, L"Checkpoint 3: Game Object Created", L"Debug", MB_OK);

    //MessageBox(NULL, L"Checkpoint 4: Calling Init...", L"Debug", MB_OK);
    if (!g_game->Init(hWnd))
    {
        MessageBox(NULL, L"g_game->Init() returned false!", L"Error", MB_OK);
        delete g_game;
        return 0;
    }
    //MessageBox(NULL, L"Checkpoint 5: Init Succeeded", L"Debug", MB_OK);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 4. 게임 루프 실행
    //MessageBox(NULL, L"Checkpoint 6: Calling Run...", L"Debug", MB_OK);
    g_game->Run();

    // 5. 게임 종료 및 리소스 해제
    g_game->Release();
    delete g_game;
    g_game = nullptr;

    return 0;
}