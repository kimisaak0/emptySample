#pragma once
#include "wndC.h"

#include "timer.h"
#include "write.h"
#include "input.h"

#include "sample.h"

class coreC_DX : public wndC_DX
{
	timerC_DX m_GameTimer;
	writeC_DX m_Font;

	sample scene;

	ID3D11RasterizerState* m_pRSSolid;

private:
	bool m_swTimerRender;
	bool m_swKeyRender;

	HRESULT SetRasterizerState();

public:
	//���� ��ü���� ó�� ������ ���� �Լ���. ���ӿ� ���� �κа� ������ ������ ���� �κ��� �и�
	bool gameInit() override;
	bool gameRun() override;
	bool gameFrame() override;
	bool gamePreRender() override;
	bool gameRender() override;
	bool gamePostRender() override;
	bool gameRelease() override;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	coreC_DX(LPCTSTR LWndName);
	virtual ~coreC_DX();
};
