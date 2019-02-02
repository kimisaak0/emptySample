#include "timer.h"

timerC_DX::timerC_DX()
{
	m_liFrequency.QuadPart = 0;
	m_liCurTime.QuadPart = 0;
	m_liBefTime.QuadPart = 0;
	m_liBef1sTime.QuadPart = 0;

	m_iFPSGether = 0;

	m_BefTickTime.QuadPart = 0;
	m_curTickTime.QuadPart = 0;

	//�ý����� ���ļ� �������� ���� //
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_liFrequency);
}

bool	timerC_DX::Init()
{
	//�ϴ� ī���� ��������
	QueryPerformanceCounter(&m_liBefTime);
	QueryPerformanceCounter(&m_liBef1sTime);
	QueryPerformanceCounter(&m_BefTickTime);


	//Ÿ�̸� ���� ���� Ȯ��
	if (m_liBefTime.QuadPart == 0) {
		return false;
	}

	m_iFPSGether = 0;

	return true;
}

bool	timerC_DX::Frame()
{
	//���� �ð� Ȯ��
	QueryPerformanceCounter(&m_liCurTime);
	m_dSPF = static_cast<float>(m_liCurTime.QuadPart - m_liBefTime.QuadPart) / static_cast<float>(m_liFrequency.QuadPart);
	m_liBefTime = m_liCurTime;

	m_dGameTime += m_dSPF;

	static double fpsPivot;

	fpsPivot += m_dSPF;
	m_iFPSGether++;
	if (fpsPivot >= 1.0)
	{
		m_iFPS = m_iFPSGether;
		m_iFPSGether = 0;
		fpsPivot -= 1.0;
	}

	return true;
}

bool	timerC_DX::Render()
{
	return true;
}
bool	timerC_DX::Release()
{
	return true;
}

bool    timerC_DX::tickAlram(double tick)
{
	QueryPerformanceCounter(&m_curTickTime);
	if (((m_curTickTime.LowPart - m_BefTickTime.LowPart) / (m_liFrequency.LowPart)) >= tick) {
		m_BefTickTime = m_curTickTime;
		return true;
	}
	return false;
}


float  timerC_DX::getGameTime()
{
	return m_dGameTime;
}
float  timerC_DX::getSPF()
{
	return m_dSPF;
}
int     timerC_DX::getFPS()
{
	return m_iFPS;
}

timerC_DX::~timerC_DX()
{

}