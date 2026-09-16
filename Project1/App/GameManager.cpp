#include "pch.h"
#include "GameManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Camera.h"
#include "ConsoleWindow.h"
#include "EditorSettings.h"

GameManager* GameManager::Instance = nullptr;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool imguiHandled = ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam);

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
	{
		if (wParam != SIZE_MINIMIZED)
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);

			RENDER.Resize(width, height);
		}
		break;
	}
	default:
		if (imguiHandled)
			return true;

		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

GameManager::GameManager()
{
	Instance = this;
}

GameManager::~GameManager()
{
	ReleaseAll();
}

void GameManager::Init(HINSTANCE hInstance)
{
	Initwindow(hInstance);
	RENDER.Initialize(m_mainWindow);

	InitImgui();
	IMGUI.Initialize();

	SCENE.AddScene("Default", new Scene());
	UE_LOG("New Scene!");
	SCENE.ChangeScene("Default");
	
	EditorSettings::Load();
}

void GameManager::Initwindow(HINSTANCE hInstance)
{
	WCHAR WindowClass[] = L"3DGameEngine";
	WCHAR Title[] = L"3DGameEngine";

	WNDCLASSW wndclass = { 0, WndProc, 0, 0, 0, 0, 0, 0, 0, WindowClass };
	RegisterClassW(&wndclass);

	m_mainWindow = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WIN_WIDTH, WIN_HEIGHT,
		nullptr, nullptr, wndclass.hInstance, nullptr);

	if (m_mainWindow)
	{
		ShowWindow(m_mainWindow, SW_SHOW);
		UpdateWindow(m_mainWindow);
	}
}

void GameManager::InitImgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	
	ImGui::GetStyle().ScaleAllSizes(1.3f);
	ImGui::GetStyle().FontScaleDpi = 1.3f;
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init((void*)m_mainWindow);
	ImGui_ImplDX11_Init(&DEVICE.GetNativeDevice(), &CONTEXT.GetNativeContext());
}

void GameManager::mainLoop()
{
	// 임구이 프레임 시작
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	BuildImGuiLayout();

	// 게임 로직 업데이트
	Update();

	// 렌더링 실행
	Render();
}

void GameManager::Update()
{
	CAMERA.Update();
	SCENE.Update(DELTA);
}

void GameManager::Render()
{
	// 프레임 버퍼 클리어 및 뷰포트 설정
	constexpr float ClearColor[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	CONTEXT.SetRenderTarget(RENDER.GetBackBufferRTV(), &RENDER.GetDepthStencilView());
	CONTEXT.SetViewport(RENDER.GetViewport());
	CONTEXT.ClearRenderTarget(RENDER.GetBackBufferRTV(), ClearColor);
	CONTEXT.ClearDepthStencil(RENDER.GetDepthStencilView());

	// 씬 오브젝트 렌더링
	SCENE.Render();

	// 임구이 렌더링
	IMGUI.RenderAll();
	ConsoleWindow::GetInstance().DrawConsole();
	ImGui::Render();

	ImDrawData* drawData = ImGui::GetDrawData();
	if (drawData)
	{
		ImGui_ImplDX11_RenderDrawData(drawData);
	}

	// 스왑체인 버퍼 교체
	RENDER.SwapBuffer();
}

void GameManager::BuildImGuiLayout()
{
	ImGuiViewport* Viewport = ImGui::GetMainViewport();
	ImGuiID DockSpaceID = ImGui::GetID("DockSpace");

	static bool bFirstTime = true;
	if (bFirstTime)
	{
		ImGui::DockBuilderAddNode(DockSpaceID, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(DockSpaceID, Viewport->Size);

		ImGuiID MainID = DockSpaceID;

		ImGuiID SceneCameraID = ImGui::DockBuilderSplitNode(MainID, ImGuiDir_Left, 0.2f, nullptr, &MainID);

		ImGuiID RightID = ImGui::DockBuilderSplitNode(MainID, ImGuiDir_Right, 0.2f, nullptr, &MainID);
		ImGuiID SceneManagerID = ImGui::DockBuilderSplitNode(RightID, ImGuiDir_Up, 0.22f, nullptr, &RightID);
		ImGuiID SpawnID = ImGui::DockBuilderSplitNode(RightID, ImGuiDir_Up, 0.25f, nullptr, &RightID);
		ImGuiID PickingID = RightID;

		ImGuiID DebugConsoleID = ImGui::DockBuilderSplitNode(MainID, ImGuiDir_Down, 0.15f, nullptr, &MainID);

		ImGui::DockBuilderDockWindow("Scene & Camera", SceneCameraID);
		ImGui::DockBuilderDockWindow("Scene Manager", SceneManagerID);
		ImGui::DockBuilderDockWindow("Spawn Primitives", SpawnID);
		ImGui::DockBuilderDockWindow("Inspector", PickingID);
		ImGui::DockBuilderDockWindow("Console", DebugConsoleID);
		ImGui::DockBuilderDockWindow("Engine Debug Info", DebugConsoleID);

		ImGui::DockBuilderFinish(DockSpaceID);

		bFirstTime = false;
	}
	ImGui::DockSpaceOverViewport(DockSpaceID, Viewport, ImGuiDockNodeFlags_PassthruCentralNode);
}

void GameManager::ReleaseAll()
{
	// 정상 종료 시 에디터 설정 일괄 저장
	EditorSettings::Save();

	// 씬 리소스 해제
	SCENE.Release();

	// 모든 오브젝트 해제
	OBJECT.DestroyAllObjects();
	OBJECT.DestroyAllMeshes();

	// 임구이 종료
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	
	RENDER.Shutdown();
}
