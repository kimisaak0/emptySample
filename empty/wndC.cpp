#include "wndC.h"

//�������� ����
HINSTANCE    g_hInst;              //�������� �ν��Ͻ� �ڵ鰪
HWND         g_hWnd;			   //������ �������� �ڵ鰪
RECT         g_rtWindow;		   //������ ����
RECT         g_rtClient;		   //Ŭ���̾�Ʈ ���� (�۾�����)
wndC_DX*     g_pWindow;            //���� ������ �����쿡 ���� ������

RECT         g_rtScreen;

//������ ���ν��� Static�Լ�
LRESULT CALLBACK wndC_DX::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	assert(g_pWindow);

	switch (iMsg) {
		//�����츦 ������ �� �߻��ϴ� �޽���
		case WM_CREATE: {
		}break;

			//�����츦 ������ �� �߻��ϴ� �޽��� (���α׷� ����ʹ� ����)
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

//�����츦 �߾����� �ű�� �Լ�.
void wndC_DX::CenterWindow()
{
	// ȭ�� ��ũ���� �ػ�(���̿� ����)�� ��´�.
	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	// ������ Ŭ���̾�Ʈ �߾Ӱ� ȭ�� ��ũ�� �߾��� �����.
	int iDestX = (iScreenWidth - g_rtWindow.right) / 2;
	int iDestY = (iScreenHeight - g_rtWindow.bottom) / 2;

	// �����츦 ȭ���߾����� �̵���Ų��.
	MoveWindow(g_hWnd, iDestX, iDestY, g_rtWindow.right, g_rtWindow.bottom, true);
}

//������ ��� �� ����
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

	g_hWnd = CreateWindowEx(0,//WS_EX_TOPMOST,         // ������ â Ȯ�� ��Ÿ��
		m_wndC.lpszClassName, m_wndC.lpszClassName,	   // ������ Ŭ���� �̸�(�߿�), Ÿ��Ʋ �ٿ� ��Ÿ�� ���ڿ�.
		WS_SYSMENU | WS_BORDER | WS_VISIBLE,           // ������ ������ â�� ��Ÿ�� ���� 
		CW_USEDEFAULT, CW_USEDEFAULT,				   // ������ X,Y ��ǥ
		1516, 789,		                               // ������ ����, ���� ũ�� (�ȼ� ����) (x+16, y+39)
		NULL, NULL, g_hInst, 			               // �θ� �������� �ڵ� ����, �޴� �ڵ� ����, �����츦 �����ϴ� �ν��Ͻ� �ڵ�
		NULL);                                         // WM_CREATE�޽����� lParam���� ���޵� CREATESTRUCT����ü ������
													   //â ��Ÿ�ϰ� â Ȯ�� ��Ÿ�Ͽ� ���� �κ��� ���� ����

													   //�����찡 �������� �ʾ����� false�� �����Ѵ�.
	if (g_hWnd == NULL) {
		return false;
	}

	GetWindowRect(g_hWnd, &g_rtWindow);
	GetClientRect(g_hWnd, &g_rtClient);

	return true;
}

//������ ������
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