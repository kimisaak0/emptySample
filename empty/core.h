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
	//게임 전체적인 처리 순서에 대한 함수들. 게임에 관한 부분과 윈도우 생성에 관한 부분을 분리
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
