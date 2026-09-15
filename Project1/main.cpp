
#include "pch.h"
#include "GameManager.h"
#include "FConstants.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// COM 라이브러리 초기화
	// COM 객체를 STA(COINIT_APARTMENTTHREADED)방식으로 접근하도록 변경. 사유: Microsoft에서 UI 스레드는 STA 사용을 권장

	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

	srand(static_cast<unsigned>(time(nullptr)));

	GameManager gm;
	gm.Init(hInstance);
	bool bIsExit = false;
	MSG msg;

	while (!bIsExit)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				bIsExit = true;
				break;
			}
		}

		gm.mainLoop();
	}

	// COM 라이브러리 해제
	CoUninitialize();

	return 0;
}