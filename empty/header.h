#pragma once
#pragma warning(disable : 4005)

#define DIRECTINPUT_VERSION  0x0800


#pragma region //헤더 및 lib 포함

//DX11을 쓰려면 필요함
#pragma comment (lib, "D3D11.lib")
#include "D3D11.h"

//DXGIFactory를 만드는데 사용됨.
#pragma comment (lib, "DXGI.lib")
#include "DXGI.h"

//다이렉트 2D사용
#pragma comment (lib, "d2d1.lib")
#include "D2D1.h"
#include "D2D1Helper.h"

//다이렉트 라이트 사용
#pragma comment (lib, "dwrite.lib")
#include "DWrite.h"

//다이렉트 인풋 사용.
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#include "dinput.h"

//쉐이더를 사용가
#pragma comment(lib, "d3dx11.lib")
#include "D3DX11.h"

#pragma comment(lib, "d3dcompiler.lib")
#include "D3Dcompiler.h"

//DX벡터 사용
#include "D3DX10math.h"

//에러처리를 위한 헤더
#include <assert.h>

//유니코드 처리를 위한 부분
#include <string>
using std::string;

#include <tchar.h>
using C_STR = std::basic_string<char>;
using W_STR = std::basic_string<wchar_t>;
using T_STR = std::basic_string<TCHAR>;

using T_ITOR = std::basic_string<TCHAR>::iterator;

//STL 헤더
#include <vector>
#include <map>
#include <list>
#include <set>

#include <bitset>
#include <algorithm>
#include <functional>

#include <math.h>


#include <memory> //스마트 포인터 사용
#include <wrl.h> //Windows Runtime C++ Template Library //ComPtr사용

using namespace std;
using namespace D2D1;
using namespace Microsoft::WRL;

#pragma endregion

#pragma region //타입 정의
using T_STR_VECTOR = vector<basic_string<TCHAR>>;
using C_STR_VECTOR = vector<string>;

using DWORD_VECTOR = vector<DWORD>;
using DWORD_VECTOR_ITOR = vector<DWORD>::iterator;

using T_STR_LIST = list<basic_string<TCHAR>>;
using CHAR_STRING = string;

using DWORD_LIST = list<DWORD>;
using DWORD_LIST_ITOR = list<DWORD>::iterator;

using HANDLE_LIST = list<HANDLE>;
using HANDLE_LIST_ITOR = list<HANDLE>::iterator;

#pragma endregion

#pragma region //매크로 정의

#define str(x) L#x
#define xstr(x) str(x)
#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;

#define  stpX(x)	((x) / ((float)g_rtClient.right))
#define  stpY(y)    -((y) / ((float)g_rtClient.bottom))

#ifndef V
#define V(x)       { hr = (x); }
#endif

#ifndef V_FRETURN
#define V_FRETURN(x){ hr = (x); if( FAILED(hr) ) { return hr; } }
#endif


#if defined(DEBUG) | defined(_DEBUG) 
#ifndef H_RETURN
#define H_RETURN(x){ if (FAILED(x)){\
		LPWSTR output;\
		WCHAR buffer[256]={0,};\
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
		NULL,x,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
		wsprintf(buffer,L"File=%s\nLine=%s", xstr(__FILE__),xstr(__LINE__));\
		MessageBox(NULL, buffer,output,MB_OK); return hr;}\
	}
#endif
#ifndef H
#define H(x){ if (FAILED(x)){\
		LPWSTR output;\
		WCHAR buffer[256]={0,};\
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
		NULL,x,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
		wsprintf(buffer,L"File=%s\nLine=%s", xstr(__FILE__),xstr(__LINE__));\
		MessageBox(NULL, buffer,output,MB_OK);}\
	}
#endif
#else
#ifndef H_RETURN
#define H_RETURN(x) (x)
#endif
#ifndef H
#define H(x) (x)
#endif
#endif

//////////////////////////////////////////////
// 객체 및 배열 할당과 삭제 및 소멸 매크로
//////////////////////////////////////////////
#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				   { A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)			   { if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)				   { if (A) delete A; (A)=NULL; }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)	   { if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)	       { if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)		       { if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )      { if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define NEW_ARRAY_CLEAR( A, B, C )  { if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif

#pragma endregion

#pragma region //외부 변수 정의

class wndC_DX;

//device_DX
extern ID3D11Device*              g_pD3dDevice;
extern ID3D11DeviceContext*       g_pD3dContext;
extern IDXGISwapChain*            g_pSwapChain;

//WndC_DX
extern HINSTANCE g_hInst;
extern HWND      g_hWnd;
extern RECT      g_rtWindow;
extern RECT      g_rtClient;
extern wndC_DX*  g_pWindow;

//core
extern float g_dSPF;

//게임 씬에서
extern float g_iWaveX;
extern float g_iWaveY;

//Hero
extern float g_PCSpeedX;
extern float g_PCSpeedY;

#pragma endregion

#pragma region //열거형 정의
enum direction
{
	d_NO = 0,
	d_LEFT = 1,
	d_TOP = 2,
	d_RIGHT = 3,
	d_BOTTOM = 4,
};

enum push
{
	p_FREE = 0,
	p_DOWN = 1,
	p_HOLD = 2,
	p_UP = 3,
};

#pragma endregion

#pragma region //구조체 정의
struct P_VERTEX
{
	D3DXVECTOR3 p;
	bool operator == (const P_VERTEX & Vertex)
	{
		if (p == Vertex.p)
		{
			return true;
		}
		return  false;
	}
	P_VERTEX() {}
	P_VERTEX(D3DXVECTOR3 vp)
	{
		p = vp;
	}
};

struct PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR4 c;
	bool operator == (const PC_VERTEX & Vertex)
	{
		if (p == Vertex.p  && c == Vertex.c)
		{
			return true;
		}
		return  false;
	}
	PC_VERTEX() {}
	PC_VERTEX(D3DXVECTOR3		vp,
		D3DXVECTOR4		vc)
	{
		p = vp, c = vc;
	}
};

struct PCT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR4 c;
	D3DXVECTOR2 t;

	PCT_VERTEX() {}

	PCT_VERTEX(D3DXVECTOR3 vp, D3DXVECTOR4 vc, D3DXVECTOR2 vt)
	{
		p = vp;
		c = vc;
		t = vt;
	}

	bool operator==  (const PCT_VERTEX& Vertex)
	{
		if (p == Vertex.p && c == Vertex.c && t == Vertex.t) {
			return true;
		}
		else {
			return false;
		}
	}
};

struct PNCT_VERTEX
{
	D3DXVECTOR3		p;
	D3DXVECTOR3		n;
	D3DXVECTOR4		c;
	D3DXVECTOR2     t;
	bool operator == (const PNCT_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT_VERTEX() {}
	PNCT_VERTEX(D3DXVECTOR3		vp,
		D3DXVECTOR3		vn,
		D3DXVECTOR4		vc,
		D3DXVECTOR2     vt)
	{
		p = vp, n = vn, c = vc, t = vt;
	}
};

struct uWH
{
	UINT width;
	UINT Height;
};

struct uPOINT
{
	UINT x;
	UINT y;

	uPOINT& operator= (const POINT& pt) {
		x = pt.x;
		y = pt.y;

		return *this;
	}
};

struct fLTRB
{
	float left;
	float top;
	float right;
	float bottom;
};

struct iXYWH
{
	LONG ltx;
	LONG lty;
	UINT width;
	UINT height;
};

struct iLTRB
{
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;

	iLTRB& operator= (const iXYWH& _xywh) {
		left = _xywh.ltx;
		top = _xywh.lty;
		right = _xywh.ltx + _xywh.width;
		bottom = _xywh.lty + _xywh.height;

		return *this;
	}
};

struct circle
{
	POINT pCenter;
	double dRadius;
};

struct ClsInfo
{
	bool bDoCls;
	direction drClsSrc;
	direction drClsDest;
	POINT ptInLength;
};

struct MouseInfo
{
	POINT xy;

	push left;
	push right;
	push middle;
};

#pragma endregion

