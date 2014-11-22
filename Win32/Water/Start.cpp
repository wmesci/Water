#include "DXWindow.h"

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	//CComModule _Module;
	CoInitialize(NULL);  
	//AtlAxWinInit();  

	auto window = new DXWindow();
	window->Show();

	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );

	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			window->Invalidate();
			::Sleep(1);
		}
	}

	delete window;

	CoUninitialize();

	return 0;
}