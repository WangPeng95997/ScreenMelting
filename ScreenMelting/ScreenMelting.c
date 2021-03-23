#include <Windows.h>
#include <Math.h>
#define PI 3.1415926

// 屏幕融化的宽度
#define SIZE_X 30
// 屏幕融化的高度
#define SIZE_Y 8
// 屏幕融化的间隔(毫秒)
#define DELAY 50

// 随机数函数
HCRYPTPROV hProv;
int Random()
{
	if (hProv == 0)
		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
			return 0;

	int out = 0;
	CryptGenRandom(hProv, sizeof(out), (BYTE*)(&out));

	return out & 0x7FFFFFFF;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	HWND hwnd;
	HDC hdc, hdcCopy;
	HBITMAP hBitmap;
	int width = 0;
	int height = 0;

	for (;;)
	{
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);

		hwnd = GetDesktopWindow();
		hdc = GetDC(hwnd);
		hdcCopy = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, SIZE_X, height);
		SelectObject(hdcCopy, hBitmap);

		for (int i = 0; i < 10; i++)
		{
			int x = Random() % width - SIZE_X / 2;
			BitBlt(hdcCopy, 0, 0, SIZE_X, height, hdc, x, 0, SRCCOPY);

			for (int ix = 0, depth = 0; ix < SIZE_X; ix++)
			{
				depth = (int)(sin(ix / (double)SIZE_X * PI) * SIZE_Y);
				StretchBlt(hdcCopy, ix, 0, 1, depth, hdcCopy, ix, 0, 1, 1, SRCCOPY);
				BitBlt(hdcCopy, ix, 0, 1, height, hdcCopy, ix, -depth, SRCCOPY);
			}
			BitBlt(hdc, x, 0, SIZE_X, height, hdcCopy, 0, 0, SRCCOPY);
			Sleep(DELAY);
		}
		DeleteDC(hdcCopy);
		DeleteObject(hBitmap);
		ReleaseDC(hwnd, hdc);
	}

	return 0;
}