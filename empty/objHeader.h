#include "Header.h"

enum ShaderName
{
	Vertex = 1,
	Pixel = 2,
	Geometry = 3,
	Hull = 4,
	Domain = 5,
	Computer = 6,
};

//전방 선언(기본값을 주기 위함)
void     ClearD3D11DeviceContext();

HRESULT  CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

ID3D11DeviceChild* LoadSelectShaderFile(ShaderName Select, const void* pShaderFileData, ID3DBlob** ppBlobOut = nullptr, char* pFuntionName = 0, bool bBinary = false);

ID3D11InputLayout* CreateInputlayout(D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements, LPCVOID lpData, DWORD dwSize);

ID3D11Buffer* CreateVertexBuffer    (void* vertices, UINT iNumVertex,   UINT iVertexSize, bool bDynamic = false);
ID3D11Buffer* CreateIndexBuffer     (void* indices,  UINT iNumIndex,    UINT iIndexSize,  bool bDynamic = false);
ID3D11Buffer* CreateConstantBuffer  (void* data,     UINT iBufferCount, UINT iBufferSize, bool bDynamic = false);

ID3D11ShaderResourceView* CreateShaderResourceView (const TCHAR* strFilePath);
ID3D11DepthStencilView*   CreateDepthStencilView   (DWORD dwWidth, DWORD dwHeight);

ID3D11BlendState* CreateBlendState();
ID3D11SamplerState* CreateSamplerState();

//------------- 오브젝트 관련 전역 함수 세팅 시작 ------//

void ClearD3D11DeviceContext()
{
	// Unbind all objects from the immediate context
	if (g_pD3dContext == NULL) return;

	ID3D11ShaderResourceView* pSRVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11RenderTargetView* pRTVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11DepthStencilView* pDSV = NULL;
	ID3D11Buffer* pBuffers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	ID3D11SamplerState* pSamplers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	UINT StrideOffset[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	// Shaders
	g_pD3dContext->VSSetShader(NULL, NULL, 0);
	g_pD3dContext->HSSetShader(NULL, NULL, 0);
	g_pD3dContext->DSSetShader(NULL, NULL, 0);
	g_pD3dContext->GSSetShader(NULL, NULL, 0);
	g_pD3dContext->PSSetShader(NULL, NULL, 0);

	// IA clear
	g_pD3dContext->IASetVertexBuffers(0, 16, pBuffers, StrideOffset, StrideOffset);
	g_pD3dContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
	g_pD3dContext->IASetInputLayout(NULL);

	// Constant buffers
	g_pD3dContext->VSSetConstantBuffers(0, 14, pBuffers);
	g_pD3dContext->HSSetConstantBuffers(0, 14, pBuffers);
	g_pD3dContext->DSSetConstantBuffers(0, 14, pBuffers);
	g_pD3dContext->GSSetConstantBuffers(0, 14, pBuffers);
	g_pD3dContext->PSSetConstantBuffers(0, 14, pBuffers);

	// Resources
	g_pD3dContext->VSSetShaderResources(0, 16, pSRVs);
	g_pD3dContext->HSSetShaderResources(0, 16, pSRVs);
	g_pD3dContext->DSSetShaderResources(0, 16, pSRVs);
	g_pD3dContext->GSSetShaderResources(0, 16, pSRVs);
	g_pD3dContext->PSSetShaderResources(0, 16, pSRVs);

	// Samplers
	g_pD3dContext->VSSetSamplers(0, 16, pSamplers);
	g_pD3dContext->HSSetSamplers(0, 16, pSamplers);
	g_pD3dContext->DSSetSamplers(0, 16, pSamplers);
	g_pD3dContext->GSSetSamplers(0, 16, pSamplers);
	g_pD3dContext->PSSetSamplers(0, 16, pSamplers);

	// Render targets
	g_pD3dContext->OMSetRenderTargets(8, pRTVs, pDSV);

	// States
	FLOAT blendFactor[4] = { 0,0,0,0 };
	g_pD3dContext->OMSetBlendState(NULL, blendFactor, 0xFFFFFFFF);
	g_pD3dContext->OMSetDepthStencilState(NULL, 0);
	g_pD3dContext->RSSetState(NULL);
}


HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )	
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel, dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr)) {
		if (pErrorBlob != NULL) {
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		}

		if (pErrorBlob) {
			pErrorBlob->Release();
		}

		return hr;
	}

	if (pErrorBlob) {
		pErrorBlob->Release();
	}

	return S_OK;
}

ID3D11DeviceChild* LoadSelectShaderFile(ShaderName Select, const void* pShaderFileData, ID3DBlob** ppBlobOut, char* pFuntionName, bool bBinary)
{
	ID3D11DeviceChild* pShader = nullptr;

	HRESULT hr = S_OK;
	ID3DBlob* pBlob = NULL;
	LPCVOID lpData = NULL;
	DWORD dwSize = 0;

	const char* pFName = pFuntionName;
	const char* pSLevel = 0;

	switch (Select) {
		case Vertex:
		{
			if (pFuntionName == 0) {
				pFName = "VS";
			}
			pSLevel = "vs_5_0";
		} break;

		case Pixel:
		{
			if (pFuntionName == 0) {
				pFName = "PS";
			}
			pSLevel = "ps_5_0";
		} break;

		case Geometry:
		{
			if (pFuntionName == 0) {
				pFName = "GS";
			}
			pSLevel = "gs_5_0";
		} break;

		case Hull:
		{
			if (pFuntionName == 0) {
				pFName = "HS";
			}
			pSLevel = "hs_5_0";
		} break;

		case Domain:
		{
			if (pFuntionName == 0) {
				pFName = "DS";
			}
			pSLevel = "ds_5_0";
		} break;

		case Computer:
		{
			if (pFuntionName == 0) {
				pFName = "CS";
			}
			pSLevel = "cs_5_0";
		} break;

	}

	if (bBinary == false) {
		hr = CompileShaderFromFile((TCHAR*)pShaderFileData, pFName, pSLevel, &pBlob);
		if (FAILED(hr)) {
			return nullptr;
		}
	}
	else {
		pBlob = *ppBlobOut;

		if (pBlob == nullptr) {
			return nullptr;
		}
	}
	dwSize = (DWORD)pBlob->GetBufferSize();
	lpData = pBlob->GetBufferPointer();

	switch (Select) {
		case Vertex:
		{
			ID3D11VertexShader* pVertexShader = nullptr;
			hr = g_pD3dDevice->CreateVertexShader(lpData, dwSize, NULL, &pVertexShader);
			if (FAILED(hr)) {
				pBlob->Release();
				return nullptr;
			}
			pShader = pVertexShader;
		} break;

		case Pixel:
		{
			ID3D11PixelShader* pPixelShader = nullptr;
			hr = g_pD3dDevice->CreatePixelShader(lpData, dwSize, NULL, &pPixelShader);
			if (FAILED(hr)) {
				pBlob->Release();
				return nullptr;
			}
			pShader = pPixelShader;
		} break;

		case Geometry:
		{
			ID3D11GeometryShader* pGeometryShader = nullptr;
			hr = g_pD3dDevice->CreateGeometryShader(lpData, dwSize, NULL, &pGeometryShader);
			if (FAILED(hr)) {
				pBlob->Release();
				return nullptr;
			}
			pShader = pGeometryShader;
		} break;

		case Hull:
		{
			ID3D11HullShader* pHullShader = nullptr;
			hr = g_pD3dDevice->CreateHullShader(lpData, dwSize, NULL, &pHullShader);
			if (FAILED(hr)) {
				pBlob->Release();
				return nullptr;
			}
			pShader = pHullShader;
		} break;

		case Domain:
		{
			ID3D11DomainShader* pDomainShader = nullptr;
			hr = g_pD3dDevice->CreateDomainShader(lpData, dwSize, NULL, &pDomainShader);
			if (FAILED(hr))
			{
				pBlob->Release();
				return nullptr;
			}
			pShader = pDomainShader;
		} break;

		case Computer:
		{
			ID3D11ComputeShader* pComputeShader = nullptr;
			hr = g_pD3dDevice->CreateComputeShader(lpData, dwSize, NULL, &pComputeShader);
			if (FAILED(hr))
			{
				pBlob->Release();
				return nullptr;
			}
			pShader = pComputeShader;
		} break;
	}

	if (ppBlobOut == nullptr) {
		pBlob->Release();
	}
	else {
		*ppBlobOut = pBlob;
	}

	return pShader;
}


ID3D11Buffer* CreateVertexBuffer(void* vertices, UINT iNumVertex, UINT iVertexSize, bool bDynamic)
{
	ID3D11Buffer* pBuffer = nullptr;

	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	if (bDynamic == false) {
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
	}
	else {
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = iVertexSize * iNumVertex;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));

	InitData.pSysMem = vertices;
	if (vertices != NULL) {
		hr = g_pD3dDevice->CreateBuffer(&bd, &InitData, &pBuffer);
		if (FAILED(hr)) {
			H(hr);
			return nullptr;
		}
	}
	else {
		g_pD3dDevice->CreateBuffer(&bd, NULL, &pBuffer);
		if (FAILED(hr)) {
			H(hr);
			return nullptr;
		}
	}
	return pBuffer;
}


ID3D11InputLayout* CreateInputlayout(D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements, LPCVOID lpData, DWORD dwSize)
{
	HRESULT hr = S_OK;

	ID3D11InputLayout* pInutLayout = nullptr;
	hr = g_pD3dDevice->CreateInputLayout(layout, numElements, lpData, dwSize, &pInutLayout);
	if (FAILED(hr)) {
		H(hr);
		return nullptr;
	}

	return pInutLayout;
}


ID3D11Buffer* CreateIndexBuffer(void *indices, UINT iNumIndex, UINT iSize, bool bDynamic)
{
	ID3D11Buffer* pBuffer = nullptr;

	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	if (bDynamic) {

		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else {

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
	}
	bd.ByteWidth = iSize * iNumIndex;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;
	if (indices != NULL)
	{
		hr = g_pD3dDevice->CreateBuffer(&bd, &InitData, &pBuffer);
		if (FAILED(hr)) {
			H(hr);
			return nullptr;
		}
	}
	else {
		hr = g_pD3dDevice->CreateBuffer(&bd, NULL, &pBuffer);
		if (FAILED(hr)) {
			H(hr);
			return nullptr;
		}
	}

	return pBuffer;
}

ID3D11Buffer* CreateConstantBuffer(void *data, UINT iBufferCount, UINT iSize, bool bDynamic)
{
	ID3D11Buffer* pBuffer = nullptr;

	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	if (bDynamic) {
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else {
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
	}
	bd.ByteWidth = iSize * iBufferCount;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = data;
	if (data != NULL) {
		hr = g_pD3dDevice->CreateBuffer(&bd, &InitData, &pBuffer);
		if (FAILED(hr)) {
			H(hr);
			return nullptr;
		}
	}
	else {
		hr = g_pD3dDevice->CreateBuffer(&bd, NULL, &pBuffer);
		if (FAILED(hr)) {
			H(hr);
			return nullptr;
		}
	}
	return pBuffer;
}

ID3D11ShaderResourceView*	CreateShaderResourceView(const TCHAR* strFilePath)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	HRESULT hr = S_OK;

	if (strFilePath == NULL) {
		return nullptr;
	}

	hr = D3DX11CreateShaderResourceViewFromFile(g_pD3dDevice, strFilePath, NULL, NULL, &pSRV, NULL);
	if (FAILED(hr)) {
		H(hr);
		return nullptr;
	}

	return pSRV;
}

ID3D11DepthStencilView* CreateDepthStencilView(DWORD dwWidth, DWORD dwHeight)
{
	ID3D11DepthStencilView* pDSV = nullptr;

	HRESULT hr;

	D3D11_TEXTURE2D_DESC DescDepth;
	DescDepth.Width = dwWidth;
	DescDepth.Height = dwHeight;
	DescDepth.MipLevels = 1;
	DescDepth.ArraySize = 1;
	DescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DescDepth.SampleDesc.Count = 1;
	DescDepth.SampleDesc.Quality = 0;
	DescDepth.Usage = D3D11_USAGE_DEFAULT;
	DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DescDepth.CPUAccessFlags = 0;
	DescDepth.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
	hr = g_pD3dDevice->CreateTexture2D(&DescDepth, NULL, &pDSTexture);
	if (FAILED(hr)) {
		return nullptr;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DescDepth.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;

	hr = g_pD3dDevice->CreateDepthStencilView(pDSTexture.Get(), &dsvd, &pDSV);
	if (FAILED(hr)) {
		return nullptr;
	}

	return pDSV;
}

ID3D11BlendState* CreateBlendState()
{
	ID3D11BlendState* returnBlendState;

	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));

	BlendState.AlphaToCoverageEnable = FALSE;
	BlendState.IndependentBlendEnable = FALSE;

	BlendState.RenderTarget[0].BlendEnable = TRUE;

	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	g_pD3dDevice->CreateBlendState(&BlendState, &returnBlendState);

	return returnBlendState;	
}

ID3D11SamplerState* CreateSamplerState()
{
	ID3D11SamplerState* returnSamplerState;

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

	g_pD3dDevice->CreateSamplerState(&samplerDesc, &returnSamplerState);

	return returnSamplerState;
}