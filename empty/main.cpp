#include "core.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	g_hInst = hInst;

	coreC_DX GameWnd(_T("GameWindow"));

	GameWnd.runWindow();

	return 0;
}