#include "DXWindow.h"
#include <Windowsx.h>

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

struct CUSTOMVERTEX
{
	float x, y, z, w, u, v;
};

DXWindow::DXWindow(void)
	: g_pD3D(nullptr)
	, g_pd3dDevice(nullptr)
	, g_pEffect(nullptr)
	, g_pHeight(nullptr)
	, g_pBack(nullptr)
	, g_pVertexBuffer(nullptr)
	, water(nullptr)
	, heightChanged(TRUE)
{

}

void DXWindow::OnDrop(LPARAM lParam)
{
	RECT rect;
	::GetClientRect(this->GetHandle(), &rect);
	this->water->Drop((float)GET_X_LPARAM(lParam) / (rect.right - rect.left), (float)GET_Y_LPARAM(lParam) / (rect.bottom - rect.top));
}

LRESULT WINAPI DXWindow::WndProc(UINT msg, WPARAM wParam, LPARAM lParam )
{
	HWND hWnd = this->GetHandle();
	switch( msg )
	{
		case WM_MOUSEACTIVATE:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			if(wParam & MK_LBUTTON) this->OnDrop(lParam);
			return 0;
		case WM_TIMER:
			if(wParam == 1)
			{
				this->water->Updata();
				this->heightChanged = TRUE;
			}
			return 0;
	}
	return Window::WndProc(msg, wParam, lParam);
}

void DXWindow::OnCreate()
{
	// Create the D3D object.
	if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return;

	water = new Water(200, 150);

	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	// Create the D3DDevice
	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->GetHandle(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice ) ) )
	{
		return;
	}

	RECT rect;
	::GetClientRect(this->GetHandle(), &rect);
	float width = rect.right - rect.left;
	float height = rect.bottom - rect.top;
	CUSTOMVERTEX vertex[] =
	{
		{ 0.0f,  0.0f,   0.0f, 1.0f, 0.0f, 0.0f, }, // x, y, z, w, u, v
		{ width, 0.0f,   0.0f, 1.0f, 1.0f, 0.0f, },
		{ 0.0f,  height, 0.0f, 1.0f, 0.0f, 1.0f, },
		{ width, height, 0.0f, 1.0f, 1.0f, 1.0f, },
	};
	
	g_pd3dDevice->CreateVertexBuffer( 4*sizeof(CUSTOMVERTEX),0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &(g_pVertexBuffer), NULL );

	void* pVertexBuffer;
	g_pVertexBuffer->Lock( 0, sizeof(vertex), (void**)&pVertexBuffer, 0 );
	memcpy( pVertexBuffer, vertex, sizeof(vertex) );
	g_pVertexBuffer->Unlock();

	g_pd3dDevice->SetStreamSource( 0, g_pVertexBuffer, 0, sizeof(CUSTOMVERTEX) );

	g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	D3DXCreateEffectFromResource(g_pd3dDevice, NULL, TEXT("IDR_SHADER"), NULL, NULL, D3DXFX_NOT_CLONEABLE, NULL, &g_pEffect, NULL);

	SetTimer(this->GetHandle(), 1, 10, NULL);

	g_pd3dDevice->CreateTexture(water->Width, water->Height, 0, NULL, D3DFMT_R32F, D3DPOOL_MANAGED, &g_pHeight, NULL);
}

void DXWindow::OnResize(int width, int height)
{
	if(g_pBack != NULL) g_pBack->Release();

	g_pd3dDevice->CreateTexture(width, height, 0, NULL, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &g_pBack, NULL);
	g_pd3dDevice->SetTexture(0, g_pBack);
}

void DXWindow::OnPaint()
{
	if(heightChanged)
	{
		D3DLOCKED_RECT rect;
		g_pHeight->LockRect(0, &rect, NULL, D3DLOCK_DISCARD);
		water->CopyTo(rect.pBits);
		g_pHeight->UnlockRect(0);
		heightChanged = FALSE;
	}

	// Clear the backbuffer to a blue color
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	// Begin the scene
	if( SUCCEEDED(g_pd3dDevice->BeginScene() ) )
	{
		g_pEffect->SetTechnique("simple");
		UINT iPass, cPasses;
		g_pEffect->Begin( &cPasses, 0);
		g_pEffect->BeginPass(0);
		{
			g_pEffect->SetFloat("dx", 1.0f / (water->Width - 1.0f));
			g_pEffect->SetFloat("dy", 1.0f / (water->Height - 1.0f));
			g_pEffect->SetTexture("Height", g_pHeight);//水波高度纹理

			// draw the single quad full screen (orthographic)
			g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
		}
		g_pEffect->EndPass();
		g_pEffect->End();
		// End the scene
		g_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

void DXWindow::OnDestroy()
{
	if( g_pd3dDevice != NULL ) 
	{
		g_pd3dDevice->Release();
		g_pd3dDevice = nullptr;
	}

	if( g_pD3D != NULL ) 
	{
		g_pD3D->Release();
		g_pD3D = nullptr;
	}

	if( g_pEffect != NULL )  
	{
		g_pEffect->Release();
		g_pEffect = nullptr;
	}

	if( g_pHeight != NULL )  
	{
		g_pHeight->Release();
		g_pHeight = nullptr;
	}

	if( g_pBack != NULL )  
	{
		g_pBack->Release();
		g_pBack = nullptr;
	}

	if( g_pVertexBuffer != NULL )
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = nullptr;
	}

	if( water != NULL )
	{
		delete water;
		water = nullptr;
	}
}

DXWindow::~DXWindow(void)
{
}
