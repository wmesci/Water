#pragma once
#include "window.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "resource.h"
#include "Water.h"

class DXWindow : public Window
{
private:
	LPDIRECT3D9             g_pD3D;       // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       g_pd3dDevice; // Our rendering device
	ID3DXEffect*            g_pEffect;
	IDirect3DTexture9*      g_pHeight;
	IDirect3DTexture9*      g_pBack;
	IDirect3DVertexBuffer9* g_pVertexBuffer;

	Water*                  water;
	BOOL heightChanged;

private:
	void OnDrop(LPARAM lParam);

protected:
	void OnResize(int width, int height) override;
	void OnPaint() override;
	void OnCreate() override;
	void OnDestroy() override;
	LRESULT WINAPI WndProc(UINT msg, WPARAM wParam, LPARAM lParam ) override;

public:
	DXWindow();
	virtual ~DXWindow();
};

