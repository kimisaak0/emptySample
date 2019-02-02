#pragma once
#include "objHeader.h"
#include "input.h"

class Object_DX
{
public:
	ComPtr<ID3D11Buffer>               m_pVertexBuffer;   // 정점 버퍼
	ComPtr<ID3D11Buffer>               m_pIndexBuffer;    // 인덱스 버퍼
	ComPtr<ID3D11Buffer>               m_pConstantBuffer; // 상수 버퍼
	ComPtr<ID3D11VertexShader>         m_pVS;             // 정점 쉐이더
	ComPtr<ID3D11PixelShader>          m_pPS;             // 픽셀 쉐이더
	ComPtr<ID3D11GeometryShader>       m_pGS;             // 기하 쉐이더
	ComPtr<ID3D11InputLayout>          m_pInputLayout;    // 인풋 레이아웃
	ComPtr<ID3D11ShaderResourceView>   m_pTextureSRV;     // 텍스쳐 SRV
	ComPtr<ID3D11BlendState>           m_pAlphaBlend;

	//sampler 변수
	ComPtr<ID3D11SamplerState>         m_pSamplerState;
	UINT                               m_idxFilter;
	D3D11_FILTER                       m_dxFilter;
	UINT                               m_iTexAddressMode;
	FLOAT                              m_MinLod;
							          
	ComPtr<ID3DBlob>                   m_pVSBlob;
	ComPtr<ID3DBlob>                   m_pPSBlob;
	ComPtr<ID3DBlob>                   m_pGSBlob;

public:
	PNCT_VERTEX m_pVertexList[4];

	uWH   m_uImageSize;
	iLTRB m_uImagePart;

	fLTRB m_fPRegion;

	POINT m_ptCenter;
	D3DXVECTOR3 m_v3Center;

public:
	iLTRB m_uSRegion;

	bool m_bExist;

private:
	//내부 함수
	void transStoP(); //화면 -> 투영
	void transPtoS(); //투영 -> 화면
	void UpdateCP(); //중점 갱신
	void UpdateVertexList(); //정점 리스트 갱신

public:
	//오브젝트 생성 관련 함수
	HRESULT Create(const TCHAR* pTexFile);
	void ImagePartialSelect(iXYWH imgXYWH, uWH imgSize);
	void SetPosition(iXYWH _xywh);

	//종합 생성
	void CreateFullImgObj(iXYWH _xywh, const TCHAR* pTexFile);
	void CreatePartImgObj(iXYWH _xywh, iXYWH imgXYWH, uWH imgSize, const TCHAR* pTexFile);

	//텍스쳐 변경 관련 함수
	void ImagePartialChange(iXYWH);
	void ImageFileChange(const TCHAR* pTexFile);

	void ColorChange(float r, float g, float b, float a);
	void Color();

	//오브젝트 조작 관련 함수
	void MoveX(float fDis);
	void MoveY(float fDis);

	void spin(float fAngle);
	void scale(float size);

	
public:
	bool PreRender(UINT iVertxSize = 0);
	bool PostRender(UINT iCount = 0);

	virtual bool Init();
	virtual bool Frame();
	virtual bool Render(UINT iVertxSize = 0, UINT iCount = 0);
	virtual bool Release();

public:
	Object_DX();
	virtual ~Object_DX();

};