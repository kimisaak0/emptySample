#include "sample.h"
#include <time.h>

sample::sample()
{
	m_bDynamic = false;
	m_isBinary = true;
}

bool sample::Init()
{
#pragma region VertexBuffer	

	P_VERTEX vertices[4];

	vertices[0].p = { -0.5f,  0.5f, 0.5f };
	vertices[1].p = {  0.5f,  0.5f, 0.5f };
	vertices[2].p = { -0.5f, -0.5f, 0.5f };
	vertices[3].p = {  0.5f, -0.5f, 0.5f };

	int numVertices = sizeof(vertices) / sizeof(vertices[0]);
	m_obj.m_pVertexBuffer.Attach(CreateVertexBuffer(vertices, numVertices, sizeof(P_VERTEX)));

#pragma endregion

#pragma region IndexBuffer

	DWORD indices[] = { 0,1,2, 0,2,3, };
	int iNumIndex = sizeof(indices) / sizeof(indices[0]);
	m_obj.m_pIndexBuffer.Attach(CreateIndexBuffer(indices, iNumIndex, sizeof(DWORD)));

#pragma endregion

#pragma region ConstantBuffer
	
	m_obj.m_pConstantBuffer.Attach(CreateConstantBuffer(&m_cbData, 1, sizeof(VSCB)));

#pragma endregion

#pragma region Shader

	if (m_isBinary) {
		DWORD dwSize = sizeof(g_VSHex);
		D3DCreateBlob(dwSize, m_obj.m_pVSBlob.GetAddressOf());
		memcpy(m_obj.m_pVSBlob->GetBufferPointer(), g_VSHex, dwSize);

		m_obj.m_pVS.Attach((ID3D11VertexShader*)LoadSelectShaderFile(Vertex, nullptr, m_obj.m_pVSBlob.GetAddressOf(), 0, true));
	}
	else {
		m_obj.m_pVS.Attach((ID3D11VertexShader*)LoadSelectShaderFile(Vertex, L"Svertex.txt", m_obj.m_pVSBlob.GetAddressOf()));
	}

	m_obj.m_pPS.Attach((ID3D11PixelShader*)LoadSelectShaderFile(Pixel, L"Svertex.txt"));
//	m_obj.m_pGS.Attach((ID3D11GeometryShader*)LoadSelectShaderFile(Geometry, L"Svertex.txt"));

#pragma endregion

#pragma region InputLayout

	//D3D11_INPUT_ELEMENT_DESC layout[] =
	//{
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "NOMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//};

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	if (m_isBinary) {
		m_obj.m_pInputLayout.Attach(CreateInputlayout(sizeof(g_VS), g_VS, layout, numElements));
	}
	else {
		m_obj.m_pInputLayout.Attach(CreateInputlayout(m_obj.m_pVSBlob.Get()->GetBufferSize(), m_obj.m_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
	}

#pragma endregion

	//samplerState 생성

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_EQUAL;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 0;

	g_pD3dDevice->CreateSamplerState(&samplerDesc, &m_obj.m_pSamplerState);
	
	m_Timer.Init();

	return true;
}

bool sample::Frame()
{
	m_Timer.Frame();

	float fTime = m_Timer.getGameTime();
	float fBoundedTime = cosf(fTime) * 0.5f + 0.5f;

	//상수 버퍼를 갱신한다.
	m_cbData.vColor = D3DXVECTOR4(cosf(fTime), sinf(fTime), 1.0f - cosf(fTime), 1.0f);
	m_cbData.fC5.x = fBoundedTime;

	if (m_bDynamic) {
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		g_pD3dContext->Map(m_obj.m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		VSCB* pConstData = (VSCB*)MappedResource.pData;
		pConstData->vColor = m_cbData.vColor;
		pConstData->fC5.x = m_cbData.fC5.x;
		g_pD3dContext->Unmap(m_obj.m_pConstantBuffer.Get(), 0);
	}
	else {
		g_pD3dContext->UpdateSubresource(m_obj.m_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
	}

	return true;
}

bool sample::Render()
{
	m_Timer.Render();

	m_obj.Render();

	return true;
}

bool sample::Release()
{
	m_Timer.Release();

	m_obj.Release();

	return true;
}

sample::~sample()
{

}