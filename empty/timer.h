#pragma once
#include "header.h"

class timerC_DX
{
private:
	LARGE_INTEGER  m_liFrequency;  //초당 주파수
	LARGE_INTEGER  m_liCurTime;    //현재 프레임에서의 시간
	LARGE_INTEGER  m_liBefTime;    //이전 프레임에서의 시간
	LARGE_INTEGER  m_liFBefTime;   //이전 프레임에서의 시간
	LARGE_INTEGER  m_liBef1sTime;  //1초전 시간

	float m_dGameTime;
	float m_dSPF;
	int    m_iFPS;

	int            m_iFPSGether;   //초당 프레임 누적용

								   //개별 타이머는 타이머 클래스를 상속받아서 만드는걸로...
	LARGE_INTEGER m_BefTickTime;
	LARGE_INTEGER m_curTickTime;


public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();

	bool    tickAlram(double tick);

	float  getGameTime();
	float  getSPF();
	int     getFPS();
public:
	timerC_DX();
	virtual ~timerC_DX();
};