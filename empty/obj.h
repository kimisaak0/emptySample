#pragma once
#include "objHeader.h"
#include "input.h"

class Object_DX
{
public:
	ComPtr<ID3D11Buffer>               m_pVertexBuffer;   // ���� ����
	ComPtr<ID3D11Buffer>               m_pIndexBuffer;    // �ε��� ����
	ComPtr<ID3D11Buffer>               m_pConstantBuffer; // ��� ����
	ComPtr<ID3D11VertexShader>         m_pVS;             // ���� ���̴�
	ComPtr<ID3D11PixelShader>          m_pPS;             // �ȼ� ���̴�
	ComPtr<ID3D11GeometryShader>       m_pGS;             // ���� ���̴�
	ComPtr<ID3D11InputLayout>          m_pInputLayout;    // ��ǲ ���̾ƿ�
	ComPtr<ID3D11ShaderResourceView>   m_pTextureSRV;     // �ؽ��� SRV
	ComPtr<ID3D11BlendState>           m_pAlphaBlend;

	//sampler ����
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
	//���� �Լ�
	void transStoP(); //ȭ�� -> ����
	void transPtoS(); //���� -> ȭ��
	void UpdateCP(); //���� ����
	void UpdateVertexList(); //���� ����Ʈ ����

public:
	//������Ʈ ���� ���� �Լ�
	HRESULT Create(const TCHAR* pTexFile);
	void ImagePartialSelect(iXYWH imgXYWH, uWH imgSize);
	void SetPosition(iXYWH _xywh);

	//���� ����
	void CreateFullImgObj(iXYWH _xywh, const TCHAR* pTexFile);
	void CreatePartImgObj(iXYWH _xywh, iXYWH imgXYWH, uWH imgSize, const TCHAR* pTexFile);

	//�ؽ��� ���� ���� �Լ�
	void ImagePartialChange(iXYWH);
	void ImageFileChange(const TCHAR* pTexFile);

	void ColorChange(float r, float g, float b, float a);
	void Color();

	//������Ʈ ���� ���� �Լ�
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