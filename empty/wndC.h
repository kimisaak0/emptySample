#pragma once
#include "device.h"

class wndC_DX : public deviceC_DX
{
protected:
	WNDCLASSEX   m_wndC;        //윈도우 클래스
	LPCWSTR      m_lWndName;    //윈도우 이름

public:
	void CenterWindow();       //윈도우를 중앙으로 옮기는 함수

							   //윈도우 프로시저 - 아예 전역 함수화 시켜둠.
	static LRESULT CALLBACK MsgProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

	bool registWnd(LPCTSTR LWndName);   //윈도우 등록.

	virtual bool runWindow();           //윈도우 돌리기

public:
	virtual bool gameInit() = 0;            //게임 전체의 초기화를 담당
	virtual bool gameRun() = 0;             //게임의 전 과정을 실행.
	virtual bool gameFrame() = 0;           //게임의 매 프레임 계산을 담당.
	virtual bool gamePreRender() = 0;       //게임의 매 프레임 렌더링 전 필요한 절차를 담당.
	virtual bool gameRender() = 0;          //게임의 매 프레임 렌더링을 담당.
	virtual bool gamePostRender() = 0;      //게임의 매 프레임 렌더링 후 필요한 절차를 담당.
	virtual bool gameRelease() = 0;         //게임 전체의 메모리 소멸 및 객체 해제를 담당.

											//	virtual bool ResetRT() = 0;

public:
	wndC_DX(LPCTSTR LWndName);
	virtual ~wndC_DX();
};