// --------------------------------------------------------------------------------
// 필요한 헤더파일을 포함합니다. WinAPI를 사용하려면 이 헤더는 필수입니다.
// --------------------------------------------------------------------------------
#include <windows.h>
#include <gdiplus.h> // GDI+ 헤더 추가
#pragma comment(lib, "gdiplus.lib") // GDI+ 라이브러리 링크

// GDI+ 네임스페이스 사용
using namespace Gdiplus;

// HBITMAP 대신 Gdiplus::Bitmap 포인터를 사용합니다.
Bitmap* g_pBitmap = nullptr;
Bitmap* g_pBackground = nullptr;
Bitmap* g_pOpponentSprite = nullptr;
Bitmap* g_pPlayerSprite = nullptr;
Bitmap* g_pHudOpponent = nullptr;
Bitmap* g_pHudPlayer = nullptr;
Bitmap* g_pTextBox = nullptr;

// --------------------------------------------------------------------------------
// 윈도우 프로시저(Window Procedure) 함수의 전방 선언
// 이 함수는 윈도우에서 발생하는 이벤트(메시지)를 처리하는 역할을 합니다.
// --------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// --------------------------------------------------------------------------------
// 윈도우 애플리케이션의 시작점(Entry Point)입니다. (콘솔 앱의 main 함수와 같음)
// --------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


    // --- 1. 윈도우 클래스 등록 ---
    // 어떤 형태와 동작을 가진 창을 만들지 '설계도(클래스)'를 정의하고 OS에 등록하는 과정입니다.

    WNDCLASS wc; // 윈도우 클래스 정보를 담을 구조체

    // 중요한 정보들을 설정합니다.
    wc.lpfnWndProc = WndProc;                       // 윈도우의 이벤트를 처리할 함수를 지정 (가장 중요!)
    wc.hInstance = hInstance;                       // 프로그램 인스턴스 핸들
    wc.lpszClassName = L"MyPokemonGame";            // 이 윈도우 클래스의 이름 (나중에 창 생성 시 사용)
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 창의 배경색

    // 나머지 정보들은 기본값으로 설정합니다.
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszMenuName = NULL;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    // 정의한 윈도우 클래스를 운영체제에 등록합니다.
    RegisterClass(&wc);

    // --- 2. 윈도우 생성 ---
    // 등록한 설계도(클래스)를 바탕으로 실제 윈도우를 생성합니다.

    HWND hWnd; // 생성된 윈도우의 핸들(ID)을 저장할 변수

    hWnd = CreateWindow(
        L"MyPokemonGame",                           // 생성할 윈도우의 클래스 이름 (위에서 등록한 이름)
        L"내 포켓몬 게임",                           // 창의 제목 표시줄에 나타날 텍스트
        WS_OVERLAPPEDWINDOW,                        // 윈도우의 스타일 (기본적인 창 형태)
        CW_USEDEFAULT, CW_USEDEFAULT,               // 창이 나타날 초기 x, y 좌표 (OS에 맡김)
        800, 600,                                   // 창의 가로, 세로 크기
        NULL,                                       // 부모 윈도우 핸들 (없음)
        NULL,                                       // 메뉴 핸들 (없음)
        hInstance,                                  // 프로그램 인스턴스 핸들
        NULL                                        // 추가 데이터 (없음)
    );

    // 윈도우가 잘 만들어졌는지 확인합니다.
    if (hWnd == NULL) {
        return 0;
    }

    // 이제 PNG 파일을 직접 로드합니다. 파일명은 원본 PNG 파일명으로!
    g_pBitmap = new Bitmap(L"bulbasaur.png");
    g_pBackground = new Bitmap(L"background.png");
    g_pOpponentSprite = new Bitmap(L"opponent_sprite.png");
    g_pPlayerSprite = new Bitmap(L"player_sprite.png");
    g_pHudOpponent = new Bitmap(L"hud_opponent.png");
    g_pHudPlayer = new Bitmap(L"hud_player.png");
    g_pTextBox = new Bitmap(L"textbox.png");

    if (g_pBitmap->GetLastStatus() != Ok)
    {
        MessageBox(hWnd, L"PNG 파일을 불러올 수 없습니다!", L"에러", MB_OK);
    }

    // --- 3. 윈도우를 화면에 표시 ---
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // --- 4. 메시지 루프 ---
    // 프로그램의 심장부입니다. 이 루프가 돌면서 사용자 입력 등의 이벤트를 받아 처리합니다.
    // 이 루프가 없으면 프로그램은 바로 종료됩니다.
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg); // 키보드 입력을 처리 가능한 형태로 변환
        DispatchMessage(&msg);  // 메시지를 WndProc 함수로 전달하여 처리하도록 함
    }

    // 프로그램 종료 전 비트맵 객체 해제
    delete g_pBitmap;
    delete g_pBackground;
    delete g_pOpponentSprite;
    delete g_pPlayerSprite;
    delete g_pHudOpponent;
    delete g_pHudPlayer;
    delete g_pTextBox;

    GdiplusShutdown(gdiplusToken);

    return (int)msg.wParam;
}

// --------------------------------------------------------------------------------
// 윈도우 프로시저(Window Procedure) 함수 구현
// 메시지 루프에서 전달된 이벤트들을 실제로 처리하는 곳입니다.
// --------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (iMessage)
    {
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        Graphics graphics(hdc);

        // --- 1. 배경 그리기 ---
        if (g_pBackground) {
            graphics.DrawImage(g_pBackground, 0, 0);
        }

        // --- 2. 포켓몬 스프라이트 그리기 ---
        if (g_pOpponentSprite) {
            // 상대 포켓몬은 오른쪽 위에 그립니다. (좌표는 이미지 크기에 맞게 조절)
            graphics.DrawImage(g_pOpponentSprite, 400, 50);
        }
        if (g_pPlayerSprite) {
            // 플레이어 포켓몬은 왼쪽 아래에 그립니다.
            graphics.DrawImage(g_pPlayerSprite, 50, 200);
        }

        // --- 3. 상태창(HUD) 및 메시지 박스 그리기 ---
        if (g_pHudOpponent) {
            graphics.DrawImage(g_pHudOpponent, 50, 50);
        }
        if (g_pHudPlayer) {
            graphics.DrawImage(g_pHudPlayer, 350, 200);
        }
        if (g_pTextBox) {
            graphics.DrawImage(g_pTextBox, 0, 350);
        }

        // --- 4. HUD 위에 텍스트와 HP 바 그리기 ---
        // 폰트와 브러시(색상) 설정
        FontFamily fontFamily(L"둥근모꼴");
        Font font(&fontFamily, 16, FontStyleBold, UnitPixel);
        SolidBrush blackBrush(Color(255, 0, 0, 0));

        // HP 바 색상 설정
        SolidBrush greenBrush(Color(255, 34, 177, 76));   // 더 게임보이스러운 초록색
        SolidBrush yellowBrush(Color(255, 255, 201, 14)); // 노란색
        SolidBrush redBrush(Color(255, 237, 28, 36));    // 빨간색
        SolidBrush backgroundBrush(Color(255, 40, 40, 40)); // HP 바 배경 회색

        // TODO: BattleManager에서 실제 포켓몬 데이터를 가져와야 함
        // 지금은 임시 데이터로 그립니다.
        float opponentHpPercent = 0.5f; // 상대 HP 50%
        float playerHpPercent = 1.0f;   // 플레이어 HP 100%

        // 상대 HUD
        graphics.DrawString(L"이상해씨", -1, &font, PointF(70.0f, 60.0f), &blackBrush);
        graphics.DrawString(L"Lv5", -1, &font, PointF(230.0f, 60.0f), &blackBrush);
        // HP 바 (배경) - 모든 인자를 float으로 통일합니다. (숫자 뒤에 f를 붙임)
        graphics.FillRectangle(&backgroundBrush, 100.0f, 85.0f, 100.0f, 10.0f);
        // HP 바 (현재 체력) - HP 양에 따라 다른 색을 쓸 수 있습니다.
        SolidBrush* opponentHpBrush = &greenBrush;
        if (opponentHpPercent < 0.5f) opponentHpBrush = &yellowBrush;
        if (opponentHpPercent < 0.2f) opponentHpBrush = &redBrush;
        graphics.FillRectangle(opponentHpBrush, 100.0f, 85.0f, 100.0f * opponentHpPercent, 10.0f);


        // 플레이어 HUD
        graphics.DrawString(L"파이리", -1, &font, PointF(370.0f, 210.0f), &blackBrush);
        graphics.DrawString(L"Lv5", -1, &font, PointF(530.0f, 210.0f), &blackBrush);
        graphics.FillRectangle(&backgroundBrush, 400.0f, 235.0f, 100.0f, 10.0f);
        SolidBrush* playerHpBrush = &greenBrush;
        if (playerHpPercent < 0.5f) playerHpBrush = &yellowBrush;
        if (playerHpPercent < 0.2f) playerHpBrush = &redBrush;
        graphics.FillRectangle(playerHpBrush, 400.0f, 235.0f, 100.0f * playerHpPercent, 10.0f);
        graphics.DrawString(L"20 / 20", -1, &font, PointF(450.0f, 250.0f), &blackBrush);

        EndPaint(hWnd, &ps);
        return 0;
    }


    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, iMessage, wParam, lParam);
}