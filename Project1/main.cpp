
#include "pch.h"
#include "GameManager.h"
#include "FConstants.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// COM 라이브러리 초기화
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	srand(static_cast<unsigned>(time(nullptr)));

	GameManager gm;
	gm.Init(hInstance);
	bool bIsExit = false;
	MSG msg;

	FName test1 = "";
	FName test2 = "Player";
	FName test3 = "Actor_0";
	FName test4 = "Actor_5";
	FName test5 = "My_Actor_5";
	FName test6 = "Foo_";
	FName test7 = "Foo_bar";
	FName test8 = "Foo_007";
	FName test9 = "Foo_1_2";
	FName test10 = "_5";
	FName tetsnull;

	FNamePool::Instance().Debug();

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