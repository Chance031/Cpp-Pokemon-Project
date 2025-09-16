// --------------------------------------------------------------------------------
// �ʿ��� ��������� �����մϴ�. WinAPI�� ����Ϸ��� �� ����� �ʼ��Դϴ�.
// --------------------------------------------------------------------------------
#include <windows.h>
#include <gdiplus.h> // GDI+ ��� �߰�
#pragma comment(lib, "gdiplus.lib") // GDI+ ���̺귯�� ��ũ

// GDI+ ���ӽ����̽� ���
using namespace Gdiplus;

// HBITMAP ��� Gdiplus::Bitmap �����͸� ����մϴ�.
Bitmap* g_pBitmap = nullptr;
Bitmap* g_pBackground = nullptr;
Bitmap* g_pOpponentSprite = nullptr;
Bitmap* g_pPlayerSprite = nullptr;
Bitmap* g_pHudOpponent = nullptr;
Bitmap* g_pHudPlayer = nullptr;
Bitmap* g_pTextBox = nullptr;

// --------------------------------------------------------------------------------
// ������ ���ν���(Window Procedure) �Լ��� ���� ����
// �� �Լ��� �����쿡�� �߻��ϴ� �̺�Ʈ(�޽���)�� ó���ϴ� ������ �մϴ�.
// --------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// --------------------------------------------------------------------------------
// ������ ���ø����̼��� ������(Entry Point)�Դϴ�. (�ܼ� ���� main �Լ��� ����)
// --------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow)
{
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


    // --- 1. ������ Ŭ���� ��� ---
    // � ���¿� ������ ���� â�� ������ '���赵(Ŭ����)'�� �����ϰ� OS�� ����ϴ� �����Դϴ�.

    WNDCLASS wc; // ������ Ŭ���� ������ ���� ����ü

    // �߿��� �������� �����մϴ�.
    wc.lpfnWndProc = WndProc;                       // �������� �̺�Ʈ�� ó���� �Լ��� ���� (���� �߿�!)
    wc.hInstance = hInstance;                       // ���α׷� �ν��Ͻ� �ڵ�
    wc.lpszClassName = L"MyPokemonGame";            // �� ������ Ŭ������ �̸� (���߿� â ���� �� ���)
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // â�� ����

    // ������ �������� �⺻������ �����մϴ�.
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszMenuName = NULL;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    // ������ ������ Ŭ������ �ü���� ����մϴ�.
    RegisterClass(&wc);

    // --- 2. ������ ���� ---
    // ����� ���赵(Ŭ����)�� �������� ���� �����츦 �����մϴ�.

    HWND hWnd; // ������ �������� �ڵ�(ID)�� ������ ����

    hWnd = CreateWindow(
        L"MyPokemonGame",                           // ������ �������� Ŭ���� �̸� (������ ����� �̸�)
        L"�� ���ϸ� ����",                           // â�� ���� ǥ���ٿ� ��Ÿ�� �ؽ�Ʈ
        WS_OVERLAPPEDWINDOW,                        // �������� ��Ÿ�� (�⺻���� â ����)
        CW_USEDEFAULT, CW_USEDEFAULT,               // â�� ��Ÿ�� �ʱ� x, y ��ǥ (OS�� �ñ�)
        800, 600,                                   // â�� ����, ���� ũ��
        NULL,                                       // �θ� ������ �ڵ� (����)
        NULL,                                       // �޴� �ڵ� (����)
        hInstance,                                  // ���α׷� �ν��Ͻ� �ڵ�
        NULL                                        // �߰� ������ (����)
    );

    // �����찡 �� ����������� Ȯ���մϴ�.
    if (hWnd == NULL) {
        return 0;
    }

    // ���� PNG ������ ���� �ε��մϴ�. ���ϸ��� ���� PNG ���ϸ�����!
    g_pBitmap = new Bitmap(L"bulbasaur.png");
    g_pBackground = new Bitmap(L"background.png");
    g_pOpponentSprite = new Bitmap(L"opponent_sprite.png");
    g_pPlayerSprite = new Bitmap(L"player_sprite.png");
    g_pHudOpponent = new Bitmap(L"hud_opponent.png");
    g_pHudPlayer = new Bitmap(L"hud_player.png");
    g_pTextBox = new Bitmap(L"textbox.png");

    if (g_pBitmap->GetLastStatus() != Ok)
    {
        MessageBox(hWnd, L"PNG ������ �ҷ��� �� �����ϴ�!", L"����", MB_OK);
    }

    // --- 3. �����츦 ȭ�鿡 ǥ�� ---
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // --- 4. �޽��� ���� ---
    // ���α׷��� ������Դϴ�. �� ������ ���鼭 ����� �Է� ���� �̺�Ʈ�� �޾� ó���մϴ�.
    // �� ������ ������ ���α׷��� �ٷ� ����˴ϴ�.
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg); // Ű���� �Է��� ó�� ������ ���·� ��ȯ
        DispatchMessage(&msg);  // �޽����� WndProc �Լ��� �����Ͽ� ó���ϵ��� ��
    }

    // ���α׷� ���� �� ��Ʈ�� ��ü ����
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
// ������ ���ν���(Window Procedure) �Լ� ����
// �޽��� �������� ���޵� �̺�Ʈ���� ������ ó���ϴ� ���Դϴ�.
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

        // --- 1. ��� �׸��� ---
        if (g_pBackground) {
            graphics.DrawImage(g_pBackground, 0, 0);
        }

        // --- 2. ���ϸ� ��������Ʈ �׸��� ---
        if (g_pOpponentSprite) {
            // ��� ���ϸ��� ������ ���� �׸��ϴ�. (��ǥ�� �̹��� ũ�⿡ �°� ����)
            graphics.DrawImage(g_pOpponentSprite, 400, 50);
        }
        if (g_pPlayerSprite) {
            // �÷��̾� ���ϸ��� ���� �Ʒ��� �׸��ϴ�.
            graphics.DrawImage(g_pPlayerSprite, 50, 200);
        }

        // --- 3. ����â(HUD) �� �޽��� �ڽ� �׸��� ---
        if (g_pHudOpponent) {
            graphics.DrawImage(g_pHudOpponent, 50, 50);
        }
        if (g_pHudPlayer) {
            graphics.DrawImage(g_pHudPlayer, 350, 200);
        }
        if (g_pTextBox) {
            graphics.DrawImage(g_pTextBox, 0, 350);
        }

        // --- 4. HUD ���� �ؽ�Ʈ�� HP �� �׸��� ---
        // ��Ʈ�� �귯��(����) ����
        FontFamily fontFamily(L"�ձٸ��");
        Font font(&fontFamily, 16, FontStyleBold, UnitPixel);
        SolidBrush blackBrush(Color(255, 0, 0, 0));

        // HP �� ���� ����
        SolidBrush greenBrush(Color(255, 34, 177, 76));   // �� ���Ӻ��̽����� �ʷϻ�
        SolidBrush yellowBrush(Color(255, 255, 201, 14)); // �����
        SolidBrush redBrush(Color(255, 237, 28, 36));    // ������
        SolidBrush backgroundBrush(Color(255, 40, 40, 40)); // HP �� ��� ȸ��

        // TODO: BattleManager���� ���� ���ϸ� �����͸� �����;� ��
        // ������ �ӽ� �����ͷ� �׸��ϴ�.
        float opponentHpPercent = 0.5f; // ��� HP 50%
        float playerHpPercent = 1.0f;   // �÷��̾� HP 100%

        // ��� HUD
        graphics.DrawString(L"�̻��ؾ�", -1, &font, PointF(70.0f, 60.0f), &blackBrush);
        graphics.DrawString(L"Lv5", -1, &font, PointF(230.0f, 60.0f), &blackBrush);
        // HP �� (���) - ��� ���ڸ� float���� �����մϴ�. (���� �ڿ� f�� ����)
        graphics.FillRectangle(&backgroundBrush, 100.0f, 85.0f, 100.0f, 10.0f);
        // HP �� (���� ü��) - HP �翡 ���� �ٸ� ���� �� �� �ֽ��ϴ�.
        SolidBrush* opponentHpBrush = &greenBrush;
        if (opponentHpPercent < 0.5f) opponentHpBrush = &yellowBrush;
        if (opponentHpPercent < 0.2f) opponentHpBrush = &redBrush;
        graphics.FillRectangle(opponentHpBrush, 100.0f, 85.0f, 100.0f * opponentHpPercent, 10.0f);


        // �÷��̾� HUD
        graphics.DrawString(L"���̸�", -1, &font, PointF(370.0f, 210.0f), &blackBrush);
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