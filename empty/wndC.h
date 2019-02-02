#pragma once
#include "device.h"

class wndC_DX : public deviceC_DX
{
protected:
	WNDCLASSEX   m_wndC;        //������ Ŭ����
	LPCWSTR      m_lWndName;    //������ �̸�

public:
	void CenterWindow();       //�����츦 �߾����� �ű�� �Լ�

							   //������ ���ν��� - �ƿ� ���� �Լ�ȭ ���ѵ�.
	static LRESULT CALLBACK MsgProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

	bool registWnd(LPCTSTR LWndName);   //������ ���.

	virtual bool runWindow();           //������ ������

public:
	virtual bool gameInit() = 0;            //���� ��ü�� �ʱ�ȭ�� ���
	virtual bool gameRun() = 0;             //������ �� ������ ����.
	virtual bool gameFrame() = 0;           //������ �� ������ ����� ���.
	virtual bool gamePreRender() = 0;       //������ �� ������ ������ �� �ʿ��� ������ ���.
	virtual bool gameRender() = 0;          //������ �� ������ �������� ���.
	virtual bool gamePostRender() = 0;      //������ �� ������ ������ �� �ʿ��� ������ ���.
	virtual bool gameRelease() = 0;         //���� ��ü�� �޸� �Ҹ� �� ��ü ������ ���.

											//	virtual bool ResetRT() = 0;

public:
	wndC_DX(LPCTSTR LWndName);
	virtual ~wndC_DX();
};