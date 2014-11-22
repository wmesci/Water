#include "Window.h"

//默认窗口类名
#define WindowClass L"Window"
static bool isinited = false;

Window::Window()
	: hWnd(nullptr)
{
	if( !isinited )
	{
		WNDCLASSEX wc;
		::ZeroMemory(&wc, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.style = 0;
		wc.lpfnWndProc = Window::WndProc;
		wc.hInstance = ::GetModuleHandle(NULL);
		wc.hCursor=::LoadCursor(NULL,IDC_ARROW); 
		wc.lpszClassName = WindowClass;

		if( ::RegisterClassEx( &wc ) )
			isinited = true;
	}
}

LRESULT WINAPI Window::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	auto pThis = (Window*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(pThis != nullptr)
	{
		return pThis->WndProc(msg, wParam, lParam);
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::WndProc(UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch(msg)
	{
		case WM_PAINT:
			this->OnPaint();
			break;
		case WM_SIZE:
			this->OnResize(LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_DESTROY:
			this->OnDestroy();
			break;
		case WM_NCDESTROY:
			hWnd = nullptr;
			::PostThreadMessage( ::GetCurrentThreadId(), WM_QUIT, 0, 0);
			break;
	}
	return ::DefWindowProc(this->GetHandle(), msg, wParam, lParam);
}

HWND Window::GetHandle()
{
	if(hWnd == NULL)
	{
		hWnd = ::CreateWindow( WindowClass, NULL, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 1286, 973, NULL, NULL, ::GetModuleHandle(NULL), NULL );
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)this);
		OnCreate();
	}
	return hWnd;
}

void Window::Show()
{
	::ShowWindow(this->GetHandle(), SW_SHOWDEFAULT);
	Update();
}

void Window::Hide()
{
	::ShowWindow(this->GetHandle(), SW_HIDE);
	Update();
}

BOOL Window::IsVisible()
{
	return ::IsWindowVisible(this->GetHandle());
}

void Window::Update()
{
	::UpdateWindow(this->GetHandle());
}

void Window::Close()
{
	::DestroyWindow(this->GetHandle());
}

void Window::Invalidate()
{
	::InvalidateRect(this->GetHandle(), nullptr, TRUE);
}

Window::~Window(void)
{
	this->Close();
}
