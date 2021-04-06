#include <Windows.h>
#include <tchar.h>

HINSTANCE g_hInsnt;
HWND hwndChild;

LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
);

LRESULT CALLBACK WindowProcChild(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
);

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	g_hInsnt = hInstance;
	WNDCLASS ws = { 0 };
	ws.lpszClassName = _T("MainWClass");
	ws.hInstance = g_hInsnt;
	ws.lpfnWndProc = WindowProc;
	ws.style = CS_HREDRAW | CS_VREDRAW;
	ws.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);

	if (NULL == RegisterClass(&ws))
		return 0;

	HWND hwnd = CreateWindow(
		ws.lpszClassName,
		_T("Master"),
		WS_OVERLAPPEDWINDOW,
		500,		
		390,	
		310,		
		340,	
		NULL,	
		NULL,		
		g_hInsnt,  
		NULL);		

	if (hwnd == NULL)
		return 0;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
		DispatchMessage(&msg);

	return 0;
}

LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	HWND hwnd2;

	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CREATE:
		WNDCLASS w, w2;

		w = { 0 };
		w.lpszClassName = _T("ChildWClass");
		w.hInstance = g_hInsnt;
		w.lpfnWndProc = WindowProcChild;
		w.style = CS_HREDRAW | CS_VREDRAW;
		w.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		if (NULL == RegisterClass(&w))
			return 0;

		hwndChild = CreateWindow(
			w.lpszClassName,	 
			_T("some child title"),			
			WS_CHILD | WS_VISIBLE, 
			100,		
			100,		
			100,		
			100,		
			hwnd,		
			NULL,		
			g_hInsnt,  
			NULL);		

		if (hwndChild == NULL)
			return 0;

		w2 = { 0 };
		w2.lpszClassName = _T("SecondWClass");
		w2.hInstance = g_hInsnt;
		w2.lpfnWndProc = WindowProc;
		w2.style = CS_HREDRAW | CS_VREDRAW;
		w2.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);

		if (NULL == RegisterClass(&w2))
			return 0;

		hwnd2 = CreateWindow(
			w2.lpszClassName,	 
			_T("Slave"),			
			WS_POPUP | WS_CAPTION | WS_VISIBLE, 
			810,		
			390,		
			310,		
			340,		
			NULL,		
			NULL,		
			g_hInsnt,  
			NULL);		

		if (hwnd2 == NULL)
			return 0;

		break;

	case WM_LBUTTONDOWN:
		HWND tmp = GetParent(hwndChild);
		if (tmp != hwnd)
			SetParent(hwndChild, hwnd);

		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK WindowProcChild(HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}