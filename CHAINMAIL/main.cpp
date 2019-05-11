#include <windows.h>
#include <d3dx9.h>
#include "Chainmail.h"
#include "Constant.h"

using namespace std;

/* global variable */
LPDIRECT3D9 pD3D = nullptr;       
LPDIRECT3DDEVICE9 pDevice = nullptr;
LPD3DXMESH pMesh = nullptr;
LPDIRECT3DVERTEXBUFFER9 pVertexBuf = nullptr;
LPDIRECT3DINDEXBUFFER9 pIdxBuf = nullptr;

Volume volume(
	Constant::Volume::WIDTH, 
	Constant::Volume::HEIGHT, 
	Constant::Volume::DEPTH);
Chainmail chainmail(volume);
D3DMATERIAL9 defaultMtrl;
D3DMATERIAL9 pivotMtrl;

bool bottonDown = false;
int curX = 0;
int curY = 0;
int prevX = 0;
int prevY = 0;

class VOLUME_VERTEX 
{
public:
	Point3f position;
	DWORD	color;
};

class LINE_INDEX 
{
public:
	DWORD start, end;
};

#define D3DFVF_VOLUME_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

static HRESULT initD3D(HWND hWnd)
{
	if (!(pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pDevice)))
		return E_FAIL;

	return S_OK;
}

static void initVolume()
{
	const float HALF_WIDTH = static_cast<float>(Constant::Volume::WIDTH / 2);
	const float HALF_HEIGHT = static_cast<float>(Constant::Volume::HEIGHT / 2);
	const float HALF_DEPTH = static_cast<float>(Constant::Volume::DEPTH / 2);
	
	const int WIDTH_IDX_MAX = (Constant::Volume::WIDTH - 1);
	const int HEIGHT_IDX_MAX = (Constant::Volume::HEIGHT - 1);
	const int DEPTH_IDX_MAX = (Constant::Volume::DEPTH - 1);

	Voxel*** const pVol = volume.getPtr3D();

	for (int d = 0; d < Constant::Volume::DEPTH; ++d)
		for (int h = 0; h < Constant::Volume::HEIGHT; ++h)
			for (int w = 0; w < Constant::Volume::WIDTH; ++w)
			{
				Voxel& curVoxel = pVol[d][h][w];

				curVoxel.position.x = (static_cast<float>(w) - HALF_WIDTH);
				curVoxel.position.y = (static_cast<float>(d) - HALF_DEPTH); 
				curVoxel.position.z = (static_cast<float>(h) - HALF_HEIGHT);

				if (w)
					curVoxel.left = &(pVol[d][h][w - 1]);
				if (w != WIDTH_IDX_MAX)
					curVoxel.right = &(pVol[d][h][w + 1]);

				if (d)
					curVoxel.bottom = &(pVol[d - 1][h][w]);
				if (d != DEPTH_IDX_MAX)
					curVoxel.top = &(pVol[d + 1][h][w]);

				if (h)
					curVoxel.back = &(pVol[d][h - 1][w]);
				if (h != HEIGHT_IDX_MAX)
					curVoxel.front = &(pVol[d][h + 1][w]);
			}
}

static void initChainMail()
{
	chainmail.setPivotIdx(Constant::Chainmail::PIVOT_IDX);

	chainmail.constraint.dX = Constant::Chainmail::DX;
	chainmail.constraint.dY = Constant::Chainmail::DY;
	chainmail.constraint.dZ = Constant::Chainmail::DZ;

	chainmail.constraint.shearX = Constant::Chainmail::SHEAR.x;
	chainmail.constraint.shearY = Constant::Chainmail::SHEAR.y;
	chainmail.constraint.shearZ = Constant::Chainmail::SHEAR.z;

	chainmail.setElasticity(Constant::Chainmail::ELASTICITY);
}

static void chainmailPropagate(const float x, const float y, const float z) 
{
	chainmail.propagate(x, y, z);

	VOLUME_VERTEX* pVertices = nullptr;

	if (FAILED(pVertexBuf->Lock(0, 0, (void**)&pVertices, 0)))
		return;

	Voxel*** const pVol = volume.getPtr3D();

	int i = 0;
	for (int d = 0; d < Constant::Volume::DEPTH; ++d)
		for (int h = 0; h < Constant::Volume::HEIGHT; ++h)
			for (int w = 0; w < Constant::Volume::WIDTH; ++w)
				pVertices[i++].position = pVol[d][h][w].position;

	pVertexBuf->Unlock();
}

static HRESULT initGeometry()
{
	if (FAILED(pDevice->CreateVertexBuffer(
		(Constant::Geometry::VERTEX_SIZE * sizeof(VOLUME_VERTEX)), 0,
		D3DFVF_VOLUME_VERTEX, D3DPOOL_DEFAULT, &pVertexBuf, NULL)))
		return E_FAIL;

	if (FAILED(pDevice->CreateIndexBuffer(
		Constant::Geometry::INDEX_SIZE * sizeof(LINE_INDEX), 0,
		D3DFMT_INDEX32, D3DPOOL_DEFAULT, &pIdxBuf, NULL)))
		return E_FAIL;

	D3DXCreateSphere(pDevice, 0.15f, 40, 40, &pMesh, NULL);

	//////////////////////////////////////////////////////////

	VOLUME_VERTEX* pVertices = nullptr;

	if (FAILED(pVertexBuf->Lock(0, 0, (void**)&pVertices, 0)))
		return E_FAIL;

	Voxel*** const pVol = volume.getPtr3D();

	int i = 0;
	for (int d = 0; d < Constant::Volume::DEPTH; ++d)
		for (int h = 0; h < Constant::Volume::HEIGHT; ++h)
			for (int w = 0; w < Constant::Volume::WIDTH; ++w)
			{
				pVertices[i].position = pVol[d][h][w].position;
				pVertices[i].color = 0xFF5E3309;

				++i;
			}

	pVertexBuf->Unlock();

	//////////////////////////////////////////////////////////

	LINE_INDEX* pIndices = nullptr;

	if (FAILED(pIdxBuf->Lock(0, 0, (void**)&pIndices, 0)))
		return E_FAIL;

	int offset = 0;

	const int X_AXIS_ITER = 
		(Constant::Volume::DEPTH * 
		Constant::Volume::HEIGHT * 
		(Constant::Volume::WIDTH - 1));

	for (i = 0; i < X_AXIS_ITER; ++i) 
	{
		pIndices[offset].start = 
			(i + (i / (Constant::Volume::WIDTH - 1)));

		pIndices[offset].end = (pIndices[i].start + 1);
		
		++offset;
	}

	const int Y_AXIS_ITER = 
		((Constant::Volume::DEPTH - 1) * 
		Constant::Volume::HEIGHT * 
		Constant::Volume::WIDTH);
	
	for (i = 0; i < Y_AXIS_ITER; ++i) 
	{
		pIndices[offset].start = i;

		pIndices[offset].end = 
			(i + (Constant::Volume::WIDTH * Constant::Volume::HEIGHT));

		++offset;
	}

	const int Z_AXIS_ITER = 
		(Constant::Volume::DEPTH * 
		(Constant::Volume::HEIGHT - 1) * 
		Constant::Volume::WIDTH);

	for (i = 0; i < Z_AXIS_ITER; ++i) 
	{
		pIndices[offset].start = 
			(i + (Constant::Volume::WIDTH * 
			(i / (Constant::Volume::WIDTH * 
			(Constant::Volume::HEIGHT - 1)))));

		pIndices[offset].end = 
			(Constant::Volume::WIDTH + pIndices[offset].start);
		
		++offset;
	}

	pIdxBuf->Unlock();

	return S_OK;
}

static void cleanup()
{
	if (pIdxBuf)
		pIdxBuf->Release();

	if (pVertexBuf)
		pVertexBuf->Release();

	if (pMesh)
		pMesh->Release();

	if (pDevice)    
		pDevice->Release();

	if (pD3D)         
		pD3D->Release();
}

static void setupViewProjection()
{
	D3DXVECTOR3 vEyePt = Constant::Camera::EYE;
	D3DXVECTOR3 vLookatPt = Constant::Camera::AT;
	D3DXVECTOR3 vUpVec = Constant::Camera::UPVECTOR;
	
	D3DXMATRIXA16 matView;                           
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	
	pDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProj;   
	D3DXMatrixPerspectiveFovLH(&matProj, (D3DX_PI / 90.f), 1.f, 1.f, 1000.f);
	
	pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

static void setupLight()
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse = Constant::Light::DIFFUSE;
	light.Specular = Constant::Light::SPECULAR;
	light.Direction = Constant::Light::DIRECTION;

	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
}

static void setupMaterial() 
{
	ZeroMemory(&defaultMtrl, sizeof(D3DMATERIAL9));
	defaultMtrl.Diffuse.r = defaultMtrl.Ambient.r = 
		Constant::Material::Default::DIFFUSE.r;
	defaultMtrl.Diffuse.g = defaultMtrl.Ambient.g = 
		Constant::Material::Default::DIFFUSE.g;
	defaultMtrl.Diffuse.b = defaultMtrl.Ambient.b = 
		Constant::Material::Default::DIFFUSE.b;
	defaultMtrl.Diffuse.a = defaultMtrl.Ambient.a =
		Constant::Material::Default::DIFFUSE.a;
	defaultMtrl.Specular =
		Constant::Material::Default::SPECULAR;

	ZeroMemory(&pivotMtrl, sizeof(D3DMATERIAL9));
	pivotMtrl.Diffuse.r = pivotMtrl.Ambient.r = 
		Constant::Material::Pivot::DIFFUSE.r;
	pivotMtrl.Diffuse.g = pivotMtrl.Ambient.g = 
		Constant::Material::Pivot::DIFFUSE.g;
	pivotMtrl.Diffuse.b = pivotMtrl.Ambient.b = 
		Constant::Material::Pivot::DIFFUSE.b;
	pivotMtrl.Diffuse.a = pivotMtrl.Ambient.a = 
		Constant::Material::Pivot::DIFFUSE.a;
	pivotMtrl.Specular = 
		Constant::Material::Pivot::SPECULAR;
}

void render()
{
	if (!pDevice)  
		return;

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.f, 0);

	if (SUCCEEDED(pDevice->BeginScene()))
	{
		D3DXMATRIXA16 matWorld;

		const float HALF_DEPTH = 
			static_cast<float>(Constant::Volume::DEPTH / 2);
		const float HALF_HEIGHT = 
			static_cast<float>(Constant::Volume::HEIGHT / 2);
		const float HALF_WIDTH =
			static_cast<float>(Constant::Volume::WIDTH / 2);

		const Index PIVOT_IDX = chainmail.getPivotIdx();
		Voxel*** const pVol = volume.getPtr3D();

		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		for (int d = 0; d < Constant::Volume::DEPTH; ++d)
			for (int h = 0; h < Constant::Volume::HEIGHT; ++h)
				for (int w = 0; w < Constant::Volume::WIDTH; ++w)
				{
					if (PIVOT_IDX == Index(w, h, d))
						pDevice->SetMaterial(&pivotMtrl);
					else
						pDevice->SetMaterial(&defaultMtrl);

					Voxel& curVoxel = pVol[d][h][w];
					D3DXMatrixTranslation(
						&matWorld, 
						(curVoxel.position.x), 
						(curVoxel.position.y), 
						(curVoxel.position.z));
					
					pDevice->SetTransform(D3DTS_WORLD, &matWorld);
					pMesh->DrawSubset(0);
				}

		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		D3DXMatrixIdentity(&matWorld);

		// 복셀을 연결하는 라인을 그린다.
		pDevice->SetTransform(D3DTS_WORLD, &matWorld);
		pDevice->SetStreamSource(0, pVertexBuf, 0, sizeof(VOLUME_VERTEX));
		pDevice->SetIndices(pIdxBuf);
		pDevice->SetFVF(D3DFVF_VOLUME_VERTEX);
		pDevice->DrawIndexedPrimitive(
			D3DPT_LINELIST, 0, 0, 
			Constant::Geometry::VERTEX_SIZE, 
			0, 
			Constant::Geometry::INDEX_SIZE);
		
		pDevice->EndScene();
	}

	pDevice->Present(NULL, NULL, NULL, NULL);
}

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		cleanup();   
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		render();    
		ValidateRect(hWnd, NULL);
		return 0;

	case WM_LBUTTONDOWN:
		bottonDown = true;
		prevX = LOWORD(lParam);
		prevY = HIWORD(lParam);
		return 0;

	case WM_LBUTTONUP:
		bottonDown = false;
		return 0;

	case WM_MOUSEMOVE:
		if (bottonDown)
		{
			curX = LOWORD(lParam);
			curY = HIWORD(lParam);
			float dX = static_cast<float>(curX - prevX);
			float dY = static_cast<float>(curY - prevY);
			
			chainmailPropagate(dX, -dY, 0.f);
			render();

			prevX = curX;
			prevY = curY;
		}
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL, 
		Constant::Screen::TITLE, NULL };
	
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(
		Constant::Screen::TITLE, Constant::Screen::TITLE,
		WS_OVERLAPPEDWINDOW, 50, 50, 
		Constant::Screen::WIDTH, Constant::Screen::HEIGHT,
		GetDesktopWindow(), NULL, wc.hInstance, NULL);

	initVolume();
	initChainMail();

	if (SUCCEEDED(initD3D(hWnd)) && 
		SUCCEEDED(initGeometry()))
	{
		setupMaterial();
		setupLight();
		setupViewProjection();

		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				InvalidateRect(hWnd, NULL, TRUE);
		}
	}

	UnregisterClass(Constant::Screen::TITLE, wc.hInstance);
	return 0;
}