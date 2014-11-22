#pragma once
#include <Windows.h>

class Window
{
private:
	HWND hWnd;

private:
	static LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

protected:
	virtual void OnCreate() { }
	virtual void OnPaint() { }
	virtual void OnResize(int width, int height){ }
	virtual void OnClose() { }
	virtual void OnDestroy() { }
	virtual LRESULT WINAPI WndProc(UINT msg, WPARAM wParam, LPARAM lParam );

public:
	Window(void);
	HWND GetHandle();
	void Show();
	void Hide();
	BOOL IsVisible();
	void Update();
	void Close();
	void Invalidate();

	virtual ~Window(void);
};

