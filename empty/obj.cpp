#include "obj.h"


Object_DX::Object_DX()
{
}

//private function

//ȭ����ǥ�� -> ������ǥ��
void Object_DX::transStoP()
{
	m_fPRegion.left = (m_uSRegion.left / ((float)g_rtClient.right)) * 2 - 1.0f;
	m_fPRegion.top = -((m_uSRegion.top / ((float)g_rtClient.bottom)) * 2 - 1.0f);
	m_fPRegion.right = (m_uSRegion.right / ((float)g_rtClient.right)) * 2 - 1.0f;
	m_fPRegion.bottom = -((m_uSRegion.bottom / ((float)g_rtClient.bottom)) * 2 - 1.0f);
}

//������ǥ�� -> ȭ����ǥ��
void Object_DX::transPtoS()
{
	m_uSRegion.left = (UINT)((m_fPRegion.left + 1.0f) / 2 * ((float)g_rtClient.right));
	m_uSRegion.top = (UINT)((m_fPRegion.top - 1.0f) / -2 * ((float)g_rtClient.bottom));
	m_uSRegion.right = (UINT)((m_fPRegion.right + 1.0f) / 2 * ((float)g_rtClient.right));
	m_uSRegion.bottom = (UINT)((m_fPRegion.bottom - 1.0f) / -2 * ((float)g_rtClient.bottom));
}

//���� ����
void Object_DX::UpdateCP()
{
	m_ptCenter.x = (m_uSRegion.right + m_uSRegion.left) / 2;
	m_ptCenter.y = (m_uSRegion.bottom + m_uSRegion.top) / 2;

	m_v3Center.x = 0.0f;
	m_v3Center.y = 0.0f;
	m_v3Center.z = 0.0f;

	for (int iV = 0; iV < 4; iV++) {
		m_v3Center.x += m_pVertexList[iV].p.x;
		m_v3Center.y += m_pVertexList[iV].p.y;
	}

	m_v3Center.x /= 4;
	m_v3Center.y /= 4;
}

//���� ����Ʈ ����
void Object_DX::UpdateVertexList()
{
	m_pVertexList[0].p = D3DXVECTOR3(m_fPRegion.left, m_fPRegion.top, 0.0f);
	m_pVertexList[1].p = D3DXVECTOR3(m_fPRegion.left, m_fPRegion.bottom, 0.0f);
	m_pVertexList[2].p = D3DXVECTOR3(m_fPRegion.right, m_fPRegion.top, 0.0f);
	m_pVertexList[3].p = D3DXVECTOR3(m_fPRegion.right, m_fPRegion.bottom, 0.0f);


	for (int pl = 0; pl < 4; pl++) {
		m_pVertexList[pl].c = D3DXVECTOR4(1, 1, 1, 1);
	}

	//���� ����
	UpdateCP();
}




//public function -> ������Ʈ ����

//������Ʈ �����ϱ�
HRESULT Object_DX::Create(const TCHAR* pTexFile)
{
	//�̹� �����ÿ����� �̺κ� �� ��
	HRESULT hr;

	m_pVertexBuffer = CreateVertexBuffer(m_pVertexList, 4, sizeof(PNCT_VERTEX));

	
	//���� ���̴� ����
	ID3DBlob*  pVSBlob;
	m_pVS =	LoadSelectShaderFile(Vertex, L"Svertex.txt", &pVSBlob);
	
	//���̾ƿ� ����
	if (pVSBlob == NULL) {
		return E_FAIL;
	}

	//����� ���� ����ü�� PNCT�� �ٲ� ����
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	int iNumElement = sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	m_pInputLayout = CreateInputlayout(ied, iNumElement, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize());

	SAFE_RELEASE(pVSBlob);

	//���� ���̴� ����
	ID3DBlob* pGSBlob;
	m_pGS = LoadSelectShaderFile(Geometry, L"Svertex.txt");


	//�ȼ� ���̴� ����
	ID3DBlob*  pPSBlob;
	m_pPS = LoadSelectShaderFile(Pixel, L"Svertex.txt");


	//���� ������Ʈ ����
	m_pAlphaBlend = CreateBlendState();

	//samplerState ����

	m_pSamplerState = CreateSamplerState();
	
	//�ؽ��� �ε�
	hr = D3DX11CreateShaderResourceViewFromFile(g_pD3dDevice, pTexFile, NULL, NULL, m_pTextureSRV.GetAddressOf(), NULL);
	if (FAILED(hr)) {
		return hr;
	}

	return hr;
}

//�̹��� �ڸ���
void Object_DX::ImagePartialSelect(iXYWH imgXYWH, uWH imgSize)
{
	m_uImagePart = imgXYWH;
	m_uImageSize = imgSize;

	fLTRB fImageUV;
	//�̹��� ȭ�� ��ǥ�� uv��ǥ�� ��ȯ
	fImageUV.left = (float)m_uImagePart.left / m_uImageSize.width;
	fImageUV.top = (float)m_uImagePart.top / m_uImageSize.Height;
	fImageUV.right = (float)m_uImagePart.right / m_uImageSize.width;
	fImageUV.bottom = (float)m_uImagePart.bottom / m_uImageSize.Height;

	m_pVertexList[0].t = D3DXVECTOR2(fImageUV.left, fImageUV.top);
	m_pVertexList[1].t = D3DXVECTOR2(fImageUV.left, fImageUV.bottom);
	m_pVertexList[2].t = D3DXVECTOR2(fImageUV.right, fImageUV.top);
	m_pVertexList[3].t = D3DXVECTOR2(fImageUV.right, fImageUV.bottom);
}

//���� ��ġ �����ϱ�
void Object_DX::SetPosition(iXYWH _xywh)
{
	//ȭ�� ��ǥ�� ����
	m_uSRegion = _xywh;

	//���� ��ǥ�� ��ȯ
	transStoP();

	//���� ����Ʈ ����
	UpdateVertexList();
}

void Object_DX::ImageFileChange(const TCHAR* pTexFile)
{
	D3DX11CreateShaderResourceViewFromFile(g_pD3dDevice, pTexFile, NULL, NULL, m_pTextureSRV.GetAddressOf(), NULL);
}

//��ü �̹��� ������Ʈ ����
void Object_DX::CreateFullImgObj(iXYWH _xywh, const TCHAR* pTexFile) 
{
	SetPosition(_xywh);
	ImagePartialSelect({ 0,0,1,1 }, { 1,1 });
	Create(pTexFile);
}

//�κ� �̹��� ������Ʈ ����
void Object_DX::CreatePartImgObj(iXYWH _xywh, iXYWH imgXYWH, uWH imgSize, const TCHAR* pTexFile)
{
	SetPosition(_xywh);
	ImagePartialSelect(imgXYWH, imgSize);
	Create(pTexFile);
}

//�̹��� ���� ���� ����
void Object_DX::ImagePartialChange(iXYWH _uXYWH)
{
	ImagePartialSelect(_uXYWH, m_uImageSize);
}


//x�� �̵�
void Object_DX::MoveX(float fDis)
{
	for (int iV = 0; iV < 4; iV++) {
		m_pVertexList[iV].p.x += fDis * g_dSPF;
	}

	m_fPRegion.left += fDis * g_dSPF;
	m_fPRegion.right += fDis * g_dSPF;

	transPtoS();

	UpdateCP();
}

//y�� �̵�
void Object_DX::MoveY(float fDis)
{
	for (int iV = 0; iV < 4; iV++) {
		m_pVertexList[iV].p.y += fDis * g_dSPF;
	}

	m_fPRegion.top += fDis * g_dSPF;
	m_fPRegion.bottom += fDis * g_dSPF;

	transPtoS();

	UpdateCP();
}

//ȸ��
void Object_DX::spin(float fAngle)
{
	float S = sinf(fAngle);
	float C = cosf(fAngle);

	for (int iV = 0; iV < 4; iV++) {

		D3DXVECTOR3 vertex = m_pVertexList[iV].p;

		m_pVertexList[iV].p.x -= m_v3Center.x;
		m_pVertexList[iV].p.y -= m_v3Center.y;

		vertex.x = m_pVertexList[iV].p.x * C + m_pVertexList[iV].p.y * S / 2;
		vertex.y = m_pVertexList[iV].p.x * -S * 2 + m_pVertexList[iV].p.y * C;

		vertex.x += m_v3Center.x;
		vertex.y += m_v3Center.y;

		m_pVertexList[iV].p = vertex;

	}
}

//ũ�� ����
void Object_DX::scale(float size)
{//�� ���� �� ����

	for (int iV = 0; iV < 4; iV++) {
		m_pVertexList[iV].p.x -= m_v3Center.x;
		m_pVertexList[iV].p.y -= m_v3Center.y;

		m_pVertexList[iV].p.x *= size;
		m_pVertexList[iV].p.y *= size;

		m_pVertexList[iV].p.x += m_v3Center.x;
		m_pVertexList[iV].p.y += m_v3Center.y;
	}
}

void Object_DX::ColorChange(float r, float g, float b, float a)
{
	//for (int pl = 0; pl < 4; pl++) {
	//	m_pVertexList[pl].c = D3DXVECTOR4(r, g, b, a);
	//}
}

void Object_DX::Color() {
	//m_pVertexList[0].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	//m_pVertexList[1].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	//m_pVertexList[2].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	//m_pVertexList[3].c = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);
}


bool Object_DX::PreRender(UINT iVertxSize)
{
	g_pD3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	g_pD3dContext->IASetInputLayout(m_pInputLayout.Get());

	UINT iStrides, iOffsets;
		
	iStrides = (iVertxSize != 0) ? (iVertxSize) : sizeof(PNCT_VERTEX);
	iOffsets = 0;

	g_pD3dContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &iStrides, &iOffsets);

	g_pD3dContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	g_pD3dContext->VSSetShader(m_pVS.Get(), NULL, NULL);

	g_pD3dContext->GSSetShader(m_pGS.Get(), NULL, NULL);

	g_pD3dContext->PSSetShader(m_pPS.Get(), NULL, NULL);
	g_pD3dContext->PSSetShaderResources(0, 1, m_pTextureSRV.GetAddressOf());

	ID3D11SamplerState* SS[1];
	SS[0] = m_pSamplerState;
	g_pD3dContext->PSSetSamplers(0, 1, SS);

	g_pD3dContext->OMSetBlendState(m_pAlphaBlend.Get(), 0, -1);

	return true;
}

bool Object_DX::PostRender(UINT iCount)
{
	if (iCount != 0) {
		g_pD3dContext->DrawIndexed(iCount, 0, 0);
	}
	else {
		g_pD3dContext->Draw(4, 0);
	}


	return true;
}


bool Object_DX::Init()
{

	return true;
}

bool Object_DX::Frame()
{
	g_pD3dContext->UpdateSubresource(m_pVertexBuffer.Get(), 0, NULL, m_pVertexList, 0, 0);
	
	
	return true;
}

bool Object_DX::Render(UINT iVertxSize, UINT iCount)
{

	PreRender(iVertxSize);
	PostRender(iCount);	

	return true;
}

bool Object_DX::Release()
{

	return true;
}


Object_DX::~Object_DX()
{

}