#include <Windows.h>

#include "Game.h"

// --- ���� ���� ---
Game* g_game = nullptr;	// ������ ���ν������� Game ��ü�� �����ϱ� ���� ������

// --- ������ ���ν��� ---
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (g_game)
	{
		return g_game->MsgProc(hWnd, iMessage, wParam, lParam);
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

// --- ���α׷� ������ ---
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // 1. ������ Ŭ���� ���
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


    // 2. ������ ����
    HWND hWnd = CreateWindow(L"PokemonFireRed", L"���ϸ��� ���̾��", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Critical Error", MB_ICONERROR);
        return 0;
    }
    //MessageBox(NULL, L"Checkpoint 2: Window Created", L"Debug", MB_OK);


    // 3. Game ��ü ���� �� �ʱ�ȭ
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

    // 4. ���� ���� ����
    //MessageBox(NULL, L"Checkpoint 6: Calling Run...", L"Debug", MB_OK);
    g_game->Run();

    // 5. ���� ���� �� ���ҽ� ����
    g_game->Release();
    delete g_game;
    g_game = nullptr;

    return 0;
}