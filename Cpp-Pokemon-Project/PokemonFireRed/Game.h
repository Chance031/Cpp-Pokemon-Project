#pragma once
#include <windows.h>
#include <gdiplus.h>
#include "Scene.h"

// ================================================================================
// 게임의 전체 생명 주기와 핵심 로직(게임 루프, 장면 관리)을 담당하는 메인 엔진 클래스
// ================================================================================
class Game
{
public:
	// --- 생성자 및 소멸자 ---
	Game();
	~Game();

	// --- 공개 인터페이스 ---
	bool Init(HWND hWnd);	// 게임에 필요한 모든 리소스와 GDI+를 초기화
	void Run();				// 메인 게임 루프를 시작하고 실행
	void Release();			// 게임이 종료될 때 사용된 모든 리소스를 해제
	LRESULT MsgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);	// 윈도우 메시지를 처리하는 프로시저

private:
	// --- 내부 로직 함수 ---
	void Update(float deltaTime);			// 게임의 상태를 업데이트 (매 프레임 호출)
	void Render(HDC hdc);					// 게임 화면을 그림 (매 프레임 호출)
	void ChangeScene(Scene* newScene);		// 현재 장면을 다른 장면으로 교체

	// --- 멤버 변수 ---
	HWND m_hWnd = NULL;						// 메인 윈도우 핸들
	int m_clientWidth = 800;				// 클라이언트 영역의 너비
	int m_clientHeight = 600;				// 클라이언트 영역의 높이

	ULONG_PTR m_gdiplusToken = 0;			// GDI+ 라이브러리 사용을 위한 토큰
	Gdiplus::Bitmap* m_pBuffer = nullptr;	// 더블 버퍼링을 위한 백 버퍼 비트맵
	
	Scene* m_pCurrentScene = nullptr;		// 현재 활성화된 장면(Scene)을 가리키는 포인터
};