#pragma once
#include "header.h"

//------------------------------------------------------------------------------------//
//-DX11 Device를 생성한다. -----------------------------------------------------------// 
//-wndC_DX에서 DX11의 멤버 변수를 직접 접근하기 위해서 deviceC_DX가 더 위에 있다.-----//
//------------------------------------------------------------------------------------//

class deviceC_DX
{
protected:
	D3D_DRIVER_TYPE            m_d3dDriverType;        // 디바이스 타입 (디폴트 : 하드웨어 가속)
	D3D_FEATURE_LEVEL          m_d3dFeatureLevel;	   // DX의 기능 수준

	IDXGIFactory*              m_pGIFactory;		   // DXGI 객체
	ID3D11RenderTargetView*    m_pRenderTagetView;	   // 메인 렌더타켓 뷰
	D3D11_VIEWPORT             m_d3dViewPort;		   // 뷰포트

public:
	HRESULT CreateDevice();                 // 디바이스 및 디바이스 컨텍스트를 생성한다.
	HRESULT	CreateGIFactory();	 			// IDXGIFactory 객체 생성
	HRESULT	CreateSwapChain();	            // IDXGIFactory를 이용하여 스왑체인 생성
	HRESULT	SetRenderTargetView();			// 타겟 뷰 설정
	HRESULT	SetViewPort();     				// 뷰 포트 설정

public:
	HRESULT InitDevice();      // 장치를 순서대로 생성한다.
	bool CreanupDevice();      // 생성한 것을 역순으로 삭제한다.

public:
	deviceC_DX();
	virtual ~deviceC_DX(
	);
};