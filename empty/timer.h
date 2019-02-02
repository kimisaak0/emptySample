#pragma once
#include "header.h"

class timerC_DX
{
private:
	LARGE_INTEGER  m_liFrequency;  //�ʴ� ���ļ�
	LARGE_INTEGER  m_liCurTime;    //���� �����ӿ����� �ð�
	LARGE_INTEGER  m_liBefTime;    //���� �����ӿ����� �ð�
	LARGE_INTEGER  m_liFBefTime;   //���� �����ӿ����� �ð�
	LARGE_INTEGER  m_liBef1sTime;  //1���� �ð�

	float m_dGameTime;
	float m_dSPF;
	int    m_iFPS;

	int            m_iFPSGether;   //�ʴ� ������ ������

								   //���� Ÿ�̸Ӵ� Ÿ�̸� Ŭ������ ��ӹ޾Ƽ� ����°ɷ�...
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