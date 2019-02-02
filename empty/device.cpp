#include "device.h"

ID3D11Device*              g_pD3dDevice;           // ����̽� ��ü
ID3D11DeviceContext*       g_pD3dContext;	       // ����̽� ���ؽ�Ʈ
IDXGISwapChain*            g_pSwapChain;		   // ����ü�� ��ü


deviceC_DX::deviceC_DX()
{
	//�����(�׷���ī��)�� ����Ͽ� ����̽� ��ü�� �����ؾ� �ϹǷ� NULL�� ����.
	m_d3dDriverType = D3D_DRIVER_TYPE_NULL;

	//���̴�5.0�� ����ؾ� �ϹǷ� 11�� ����.
	m_d3dFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	//���� �� ���� �� �ʱ�ȭ. 
	g_pD3dDevice = nullptr;
	g_pD3dContext = nullptr;
	m_pGIFactory = nullptr;
	g_pSwapChain = nullptr;

	m_pRenderTagetView = nullptr;
}

//ID3D11Device �������̽��� �����Ѵ�.
HRESULT deviceC_DX::CreateDevice()
{
	HRESULT hr;
	//d2d�� ������ �� �÷��׸� �����.
	UINT uCreateDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;  //����̽� ���� �÷���

#if defined(DEBUG) || defined(_DEBUG)
	uCreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


	//����̽� ���� Ÿ��
	D3D_DRIVER_TYPE dxDriverTypes[] =
	{
		D3D_DRIVER_TYPE_UNKNOWN,  //����̽� ��ü�� ������ �� �ִ� �ֻ��� ����̺� Ÿ���� �ڵ������� ���õȴٴµ�...?
		D3D_DRIVER_TYPE_HARDWARE,   //�ּ�
		D3D_DRIVER_TYPE_WARP,       //����
		D3D_DRIVER_TYPE_REFERENCE   //�־� (�׷��� ī�尡 �� �����ٶ� ���߿����� ���)
	};
	UINT uNumDriverTypes = sizeof(dxDriverTypes) / sizeof(dxDriverTypes[0]);

	//���õ� �� �ִ� ��ó���� �迭
	D3D_FEATURE_LEVEL dxFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
	};
	UINT uNumFeatureLevels = sizeof(dxFeatureLevels) / sizeof(dxFeatureLevels[0]);

	//���ķ����� 11�� ������ ������ ������ ���� �ݺ���
	for (UINT uDriverTypeIndex = 0; uDriverTypeIndex < uNumDriverTypes; uDriverTypeIndex++) {

		m_d3dDriverType = dxDriverTypes[uDriverTypeIndex];

		hr = D3D11CreateDevice(
			NULL,                  //���÷��� �ý���(�����) NULL�̸� �ֵ��÷��̷� �ڵ� ���õ�.
			m_d3dDriverType,       //����̹� Ÿ�� ����.
			NULL,                  //Ư�� ����Ʈ������ ����� ����ϴ� ����̽��� ���鶧 ���.
			uCreateDeviceFlags,    //����̽� ���� �÷���
			dxFeatureLevels,       //���õ� �� �ִ� ��ó ������ ����ִ� �迭
			uNumFeatureLevels,     //���� �迭�� ���Ҽ�
			D3D11_SDK_VERSION,     //SDK ����
			&g_pD3dDevice,         //������ ����̽��� ��ȯ���� �ּ�
			&m_d3dFeatureLevel,    //���õ� ��ó������ ��ȯ���� �ּ�.
			&g_pD3dContext);       //������ DC�� ��ȯ���� �ּ�.

								   //����̽��� �����ϴµ� �����Ͽ����� ������ 11�� �ƴ� ��� �ٽ� �����ϰ� �Ѵ�.
		if (SUCCEEDED(hr)) {
			if (FAILED(hr) || m_d3dFeatureLevel < D3D_FEATURE_LEVEL_11_0) {
				if (g_pD3dContext) { g_pD3dContext->Release(); }
				if (g_pD3dDevice) { g_pD3dDevice->Release(); }
				continue;
			} break;
		}
	}

	if (FAILED(hr)) {
		return false;
	}

	return S_OK;
}

//DXGIFactory �������̽��� �����Ѵ�. 
HRESULT	deviceC_DX::CreateGIFactory()
{
	if (g_pD3dDevice == nullptr) { return E_FAIL; }

	HRESULT hr;
	IDXGIDevice* pDXGIDevice;
	IDXGIAdapter* pDXGIAdapter;

	//�����Ǿ� �ִ� ID3D11Device�κ��� ����ؼ� IDXGIFactory�� �Ѱܹ޴´�.
	hr = g_pD3dDevice->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)(&pDXGIDevice));
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (LPVOID*)(&pDXGIAdapter));
	hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (LPVOID*)(&m_pGIFactory));

	pDXGIDevice->Release();
	pDXGIAdapter->Release();
	return hr;
}

//DXGIFactory �������̽��κ��� IDXGISwapChain �������̽��� �����Ѵ�.
HRESULT	deviceC_DX::CreateSwapChain()
{
	HRESULT hr = S_OK;
	if (m_pGIFactory == nullptr) { return S_FALSE; }

	//����ü�� ����ü�� ������ �� �ʿ��� ������ ä���� �̸� �̿��� ����ü�� ��ü�� �����Ѵ�.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	sd.BufferCount = 1;                                  //���� ����         
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;   //������ ���� ���� (�ʼ�)
	sd.BufferDesc.Width = g_rtWindow.right;
	sd.BufferDesc.Height = g_rtWindow.bottom;
	sd.BufferDesc.RefreshRate.Numerator = 60;            //ȭ�� �ֻ��� (����)
	sd.BufferDesc.RefreshRate.Denominator = 1;           //ȭ�� �ֻ��� (�и�)
	sd.SampleDesc.Count = 1;                             //��Ƽ���ø��� 
	sd.SampleDesc.Quality = 0;                           //�̹��� ǰ�� ����
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    //���� �뵵
	sd.OutputWindow = g_hWnd;                            //��� �����쿡 �Ѹ��� (�ʼ�)
	sd.Windowed = true;                                  //false ��üȭ��, true â���
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//����ü�� ����
	hr = m_pGIFactory->CreateSwapChain(g_pD3dDevice, &sd, &g_pSwapChain);
	if (FAILED(hr)) {
		return hr;
	}

	return hr;
}

//����Ÿ�ٺ� ���� �� ����
HRESULT	deviceC_DX::SetRenderTargetView()
{
	HRESULT hr;
	ID3D11Texture2D* pBackBuffer;

	//����۸� �޾ƿ�
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) {
		return hr;
	}

	//���� Ÿ�� �� ����
	hr = g_pD3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTagetView);
	if (FAILED(hr)) {
		return hr;
	}

	pBackBuffer->Release();

	//����Ÿ�ٺ� ����
	g_pD3dContext->OMSetRenderTargets(1, &m_pRenderTagetView, nullptr);
	return S_OK;
}



//����Ʈ ����
HRESULT	deviceC_DX::SetViewPort()
{
	//����ü�� ������ ������.
	DXGI_SWAP_CHAIN_DESC Desc;
	g_pSwapChain->GetDesc(&Desc);

	//����Ʈ�� ���̿� ���̸� ����ü�� ������ ũ��� ����
	m_d3dViewPort.Width = (FLOAT)Desc.BufferDesc.Width;
	m_d3dViewPort.Height = (FLOAT)Desc.BufferDesc.Height;
	m_d3dViewPort.MinDepth = 0.0f;  //���̰��� 0���� 1���̷� ���ѵ�
	m_d3dViewPort.MaxDepth = 1.0f;

	//����Ÿ�Ϻ� ����
	g_pD3dContext->RSSetViewports(1, &m_d3dViewPort);

	return S_OK;
}

//��ü �����ϱ�
HRESULT deviceC_DX::InitDevice()
{
	HRESULT hr;

	if (FAILED(hr = CreateDevice())) { return hr; }
	if (FAILED(hr = CreateGIFactory())) { return hr; }
	if (FAILED(hr = CreateSwapChain())) { return hr; }
	if (FAILED(hr = SetRenderTargetView())) { return hr; }
	if (FAILED(hr = SetViewPort())) { return hr; }

	//ALT+ENTER�� �޽���ť ����͸��� ���´�.
	m_pGIFactory->MakeWindowAssociation(g_hWnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);

	return hr;
}

//�����ϱ�
bool deviceC_DX::CreanupDevice()
{
	//������ ������ ����.
	//���ð��� ���������ְ� �����Ѵ�.
	if (g_pD3dContext) { g_pD3dContext->ClearState(); }
	if (g_pD3dContext) { g_pD3dContext->Release(); }
	g_pD3dContext = nullptr;

	if (m_pRenderTagetView) { m_pRenderTagetView->Release(); }
	m_pRenderTagetView = nullptr;

	if (g_pSwapChain) { g_pSwapChain->Release(); }
	g_pSwapChain = nullptr;

	if (g_pD3dDevice) { g_pD3dDevice->Release(); }
	g_pD3dDevice = nullptr;

	if (m_pGIFactory) { m_pGIFactory->Release(); }
	m_pGIFactory = nullptr;

	return true;
}


deviceC_DX::~deviceC_DX()
{

}