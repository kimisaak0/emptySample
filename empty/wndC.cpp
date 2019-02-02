#include "wndC.h"

//전역변수 선언
HINSTANCE    g_hInst;              //윈도우의 인스턴스 핸들값
HWND         g_hWnd;			   //생성된 윈도우의 핸들값
RECT         g_rtWindow;		   //윈도우 영역
RECT         g_rtClient;		   //클라이언트 영역 (작업영역)
wndC_DX*     g_pWindow;            //현재 생성된 윈도우에 대한 포인터

RECT         g_rtScreen;

//윈도우 프로시져 Static함수
LRESULT CALLBACK wndC_DX::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	assert(g_pWindow);

	switch (iMsg) {
		//윈도우를 생성할 때 발생하는 메시지
		case WM_CREATE: {
		}break;

			//윈도우를 종료할 때 발생하는 메시지 (프로그램 종료와는 별도)
		case WM_DESTROY: {
			PostQuitMessage(0);
		}break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

wndC_DX::wndC_DX(LPCTSTR LWndName)
{
	g_pWindow = this;

	registWnd(LWndName);
}

//윈도우를 중앙으로 옮기는 함수.
void wndC_DX::CenterWindow()
{
	// 화면 스크린의 해상도(넓이와 높이)을 얻는다.
	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	// 윈도우 클라이언트 중앙과 화면 스크린 중앙을 맞춘다.
	int iDestX = (iScreenWidth - g_rtWindow.right) / 2;
	int iDestY = (iScreenHeight - g_rtWindow.bottom) / 2;

	// 윈도우를 화면중앙으로 이동시킨다.
	MoveWindow(g_hWnd, iDestX, iDestY, g_rtWindow.right, g_rtWindow.bottom, true);
}

//윈도우 등록 및 생성
bool wndC_DX::registWnd(LPCTSTR LWndName)
{
	//Regist wndClass
	ZeroMemory(&m_wndC, sizeof(WNDCLASSEX));
	m_wndC.cbSize = sizeof(WNDCLASSEX);
	m_wndC.hInstance = g_hInst;
	m_wndC.lpfnWndProc = MsgProc;
	m_wndC.lpszClassName = LWndName;

	if (!RegisterClassEx(&m_wndC)) {
		return false;
	}

	g_hWnd = CreateWindowEx(0,//WS_EX_TOPMOST,         // 윈도우 창 확장 스타일
		m_wndC.lpszClassName, m_wndC.lpszClassName,	   // 윈도우 클래스 이름(중요), 타이틀 바에 나타날 문자열.
		WS_SYSMENU | WS_BORDER | WS_VISIBLE,           // 생성될 윈도우 창의 스타일 지정 
		CW_USEDEFAULT, CW_USEDEFAULT,				   // 윈도우 X,Y 좌표
		1516, 789,		                               // 윈도우 수평, 수직 크기 (픽셀 단위) (x+16, y+39)
		NULL, NULL, g_hInst, 			               // 부모 윈도우의 핸들 지정, 메뉴 핸들 지정, 윈도우를 생성하는 인스턴스 핸들
		NULL);                                         // WM_CREATE메시지의 lParam으로 전달될 CREATESTRUCT구조체 포인터
													   //창 스타일과 창 확장 스타일에 대한 부분은 문서 참조

													   //윈도우가 생성되지 않았으면 false를 리턴한다.
	if (g_hWnd == NULL) {
		return false;
	}

	GetWindowRect(g_hWnd, &g_rtWindow);
	GetClientRect(g_hWnd, &g_rtClient);

	return true;
}

//윈도우 돌리기
bool wndC_DX::runWindow()
{
	if (!gameInit()) { return false; }

	//Main message loop
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				break;
			}
		}
		else {
			gameRun();
		}
	}

	if (!gameRelease()) { return false; }

	return true;
}

wndC_DX::~wndC_DX()
{

}