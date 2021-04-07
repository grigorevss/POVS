#include <Windows.h>
#include <Windowsx.h>
#include <tchar.h>
#include <cstdlib>
#include <ctime>

BOOL isCursorInClienPart = FALSE;
int CurrentClientPart = 0;
TRACKMOUSEEVENT trackMouseEvent;

LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
);

BOOL SetCurrentPart(HWND hwnd, int x, int y);

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	std::srand(time(NULL));

	WNDCLASS ws = { 0 };
	ws.lpszClassName = _T("MainWClass");
	ws.hInstance = hInstance;
	ws.lpfnWndProc = WindowProc;
	ws.style = CS_HREDRAW | CS_VREDRAW;
	ws.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	if (NULL == RegisterClass(&ws))
		return 0;

	HWND hwnd = CreateWindow(
		ws.lpszClassName,
		_T("Master"),
		WS_OVERLAPPEDWINDOW,
		810,
		390,
		310,
		340,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (hwnd == NULL)
		return 0;

	ShowWindow(hwnd, nCmdShow);

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
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CREATE:
		trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
		trackMouseEvent.hwndTrack = hwnd;
		trackMouseEvent.dwFlags = TME_LEAVE;
		trackMouseEvent.dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(&trackMouseEvent);
		return 0;

	case WM_PAINT: {
		PAINTSTRUCT paintStruct;

		RECT currentRect;
		GetClientRect(hwnd, &currentRect);
		int xCenter = (currentRect.right - currentRect.left) / 2;
		int yCenter = (currentRect.bottom - currentRect.top) / 2;

		HDC hdc = BeginPaint(hwnd, &paintStruct);

		switch (CurrentClientPart) {
		case 1: {
			currentRect.left = xCenter;
			currentRect.top = 0;
			currentRect.right = paintStruct.rcPaint.right;
			currentRect.bottom = yCenter;
			HBRUSH tmpBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
			FillRect(hdc, &currentRect, tmpBrush);
			DeleteBrush(tmpBrush);
			break;
		}
		case 2: {
			currentRect.left = 0;
			currentRect.top = 0;
			currentRect.right = xCenter;
			currentRect.bottom = yCenter;
			HBRUSH tmpBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
			FillRect(hdc, &currentRect, tmpBrush);
			DeleteBrush(tmpBrush);
			break;
		}
		case 3: {
			currentRect.left = 0;
			currentRect.top = yCenter;
			currentRect.right = xCenter;
			currentRect.bottom = paintStruct.rcPaint.bottom;
			HBRUSH tmpBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
			FillRect(hdc, &currentRect, tmpBrush);
			DeleteBrush(tmpBrush);
			break;
		}
		case 4: {
			currentRect.left = xCenter;
			currentRect.top = yCenter;
			currentRect.right = paintStruct.rcPaint.right;
			currentRect.bottom = paintStruct.rcPaint.bottom;
			HBRUSH tmpBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
			FillRect(hdc, &currentRect, tmpBrush);
			DeleteBrush(tmpBrush);
			break;
		}
		}

		EndPaint(hwnd, &paintStruct);
		return 0;
	}
	case WM_MOUSELEAVE:
		if (isCursorInClienPart) {
			CurrentClientPart = 0;
			isCursorInClienPart = FALSE;
			InvalidateRect(hwnd, NULL, TRUE);
		}

		return 0;

	case WM_MOUSEMOVE:
		if (SetCurrentPart(hwnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
			InvalidateRect(hwnd, NULL, TRUE);

		if (!isCursorInClienPart) {
			isCursorInClienPart = TRUE;
			TrackMouseEvent(&trackMouseEvent);
		}

		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL SetCurrentPart(HWND hwnd, int x, int y)
{
	RECT rectWindow;
	GetClientRect(hwnd, &rectWindow);

	int xCenter = (rectWindow.right - rectWindow.left) / 2;
	int yCenter = (rectWindow.bottom - rectWindow.top) / 2;

	if (x >= xCenter)
		if (y >= yCenter)
			return 4 != CurrentClientPart ? CurrentClientPart = 4 : 0;
		else
			return 1 != CurrentClientPart ? CurrentClientPart = 1 : 0;
	else
		if (y >= yCenter)
			return 3 != CurrentClientPart ? CurrentClientPart = 3 : 0;
		else
			return 2 != CurrentClientPart ? CurrentClientPart = 2 : 0;
}
