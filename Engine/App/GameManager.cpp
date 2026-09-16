#include "pch.h"
#include "GameManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Camera.h"
#include "ConsoleWindow.h"
#include "EditorSettings.h"
#include "../resource.h"

GameManager* GameManager::Instance = nullptr;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	INPUT.ProcessMessage(message, wParam, lParam);
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
	WCHAR WindowClass[] = L"AebollaeEngine";
	WCHAR Title[] = L"Aebollae Engine";

	WNDCLASSEXW wndclass = {};
	wndclass.cbSize = sizeof(wndclass);
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = WindowClass;
	wndclass.hIcon = static_cast<HICON>(LoadImageW(hInstance, MAKEINTRESOURCEW(IDI_ICON1),
		IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), LR_SHARED));
	wndclass.hIconSm = static_cast<HICON>(LoadImageW(hInstance, MAKEINTRESOURCEW(IDI_ICON1),
		IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_SHARED));
	RegisterClassExW(&wndclass);

	m_mainWindow = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		(GetSystemMetrics(SM_CXSCREEN) - WIN_WIDTH) / 2, (GetSystemMetrics(SM_CYSCREEN) - WIN_HEIGHT) / 2, WIN_WIDTH, WIN_HEIGHT,
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

	SetImGuiStyle();

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

	INPUT.EndFrame();
}

void GameManager::Update()
{
	CAMERA.Update();
	SCENE.Update(DELTA);
}

void GameManager::Render()
{
	// 프레임 버퍼 클리어 및 뷰포트 설정
	constexpr float ClearColor[] = { 0.8f, 0.8f, 0.9f, 1.0f };
	CONTEXT.SetRenderTarget(RENDER.GetBackBufferRTV(), &RENDER.GetDepthStencilView());
	CONTEXT.SetViewport(RENDER.GetViewport());
	CONTEXT.ClearRenderTarget(RENDER.GetBackBufferRTV(), ClearColor);
	CONTEXT.ClearDepthStencil(RENDER.GetDepthStencilView());

	// 씬 오브젝트 렌더링
	SCENE.Render();

	// 임구이 렌더링
	IMGUI.RenderAll();
	ConsoleWindow::GetInstance().DrawConsole();

	CONTEXT.SetRenderTarget(RENDER.GetBackBufferUNormRTV(), &RENDER.GetDepthStencilView());
	ImGui::Render();

	ImDrawData* drawData = ImGui::GetDrawData();
	if (drawData)
	{
		ImGui_ImplDX11_RenderDrawData(drawData);
	}

	// 스왑체인 버퍼 교체
	RENDER.SwapBuffer();
}

void GameManager::SetImGuiStyle()
{
	ImGuiStyle& Style = ImGui::GetStyle();
	Style.ScaleAllSizes(1.3f);
	Style.FontScaleDpi = 1.3f;

	ImVec4* Colors = Style.Colors;
	// Catppuccin Mocha Palette
	// --------------------------------------------------------
	const ImVec4 base = ImVec4(0.117f, 0.117f, 0.172f, 0.4f); // #1e1e2e
	const ImVec4 mantle = ImVec4(0.109f, 0.109f, 0.156f, 0.4f); // #181825
	const ImVec4 surface0 = ImVec4(0.200f, 0.207f, 0.286f, 0.4f); // #313244
	const ImVec4 surface1 = ImVec4(0.247f, 0.254f, 0.337f, 0.4f); // #3f4056
	const ImVec4 surface2 = ImVec4(0.290f, 0.301f, 0.388f, 0.4f); // #4a4d63
	const ImVec4 surface2Opaque = ImVec4(0.290f, 0.301f, 0.388f, 0.9f); // #4a4d63
	const ImVec4 overlay0 = ImVec4(0.396f, 0.403f, 0.486f, 1.0f); // #65677c
	const ImVec4 overlay1 = ImVec4(0.498f, 0.518f, 0.612f, 1.0f); // #7f849c
	const ImVec4 overlay2 = ImVec4(0.576f, 0.584f, 0.654f, 1.0f); // #9399b2
	const ImVec4 text = ImVec4(0.803f, 0.815f, 0.878f, 1.0f); // #cdd6f4
	const ImVec4 subtext0 = ImVec4(0.639f, 0.658f, 0.764f, 1.0f); // #a3a8c3
	const ImVec4 mauve = ImVec4(0.796f, 0.698f, 0.972f, 1.0f); // #cba6f7
	const ImVec4 peach = ImVec4(0.980f, 0.709f, 0.572f, 1.0f); // #fab387
	const ImVec4 yellow = ImVec4(0.980f, 0.913f, 0.596f, 1.0f); // #f9e2af
	const ImVec4 green = ImVec4(0.650f, 0.890f, 0.631f, 1.0f); // #a6e3a1
	const ImVec4 teal = ImVec4(0.580f, 0.886f, 0.819f, 1.0f); // #94e2d5
	const ImVec4 sapphire = ImVec4(0.458f, 0.784f, 0.878f, 1.0f); // #74c7ec
	const ImVec4 blue = ImVec4(0.533f, 0.698f, 0.976f, 1.0f); // #89b4fa
	const ImVec4 lavender = ImVec4(0.709f, 0.764f, 0.980f, 1.0f); // #b4befe

	// Main window and backgrounds
	Colors[ImGuiCol_WindowBg] = surface0;
	Colors[ImGuiCol_ChildBg] = surface0;
	Colors[ImGuiCol_PopupBg] = surface2Opaque;
	Colors[ImGuiCol_Border] = surface1;
	Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	Colors[ImGuiCol_FrameBg] = base;
	Colors[ImGuiCol_FrameBgHovered] = surface1;
	Colors[ImGuiCol_FrameBgActive] = surface2;
	Colors[ImGuiCol_TitleBg] = mantle;
	Colors[ImGuiCol_TitleBgActive] = surface0;
	Colors[ImGuiCol_TitleBgCollapsed] = mantle;
	Colors[ImGuiCol_MenuBarBg] = surface0;
	Colors[ImGuiCol_ScrollbarBg] = surface0;
	Colors[ImGuiCol_ScrollbarGrab] = surface2;
	Colors[ImGuiCol_ScrollbarGrabHovered] = overlay0;
	Colors[ImGuiCol_ScrollbarGrabActive] = overlay2;
	Colors[ImGuiCol_CheckMark] = mauve;
	Colors[ImGuiCol_CheckboxSelectedBg] = base;
	Colors[ImGuiCol_SliderGrab] = overlay2;
	Colors[ImGuiCol_SliderGrabActive] = mauve;
	Colors[ImGuiCol_Button] = base;
	Colors[ImGuiCol_ButtonHovered] = surface1;
	Colors[ImGuiCol_ButtonActive] = mauve;
	Colors[ImGuiCol_Header] = surface0;
	Colors[ImGuiCol_HeaderHovered] = surface1;
	Colors[ImGuiCol_HeaderActive] = surface2;
	Colors[ImGuiCol_Separator] = surface1;
	Colors[ImGuiCol_SeparatorHovered] = mauve;
	Colors[ImGuiCol_SeparatorActive] = mauve;
	Colors[ImGuiCol_ResizeGrip] = surface2;
	Colors[ImGuiCol_ResizeGripHovered] = mauve;
	Colors[ImGuiCol_ResizeGripActive] = mauve;
	Colors[ImGuiCol_Tab] = surface0;
	Colors[ImGuiCol_TabHovered] = surface1;
	Colors[ImGuiCol_TabActive] = surface2;
	Colors[ImGuiCol_TabUnfocused] = surface0;
	Colors[ImGuiCol_TabUnfocusedActive] = surface1;
	Colors[ImGuiCol_TabDimmedSelectedOverline] = mauve;
	Colors[ImGuiCol_TabSelectedOverline] = mauve;
	Colors[ImGuiCol_DockingPreview] = mauve;
	Colors[ImGuiCol_DockingEmptyBg] = base;
	Colors[ImGuiCol_PlotLines] = blue;
	Colors[ImGuiCol_PlotLinesHovered] = peach;
	Colors[ImGuiCol_PlotHistogram] = teal;
	Colors[ImGuiCol_PlotHistogramHovered] = green;
	Colors[ImGuiCol_TableHeaderBg] = surface0;
	Colors[ImGuiCol_TableBorderStrong] = surface1;
	Colors[ImGuiCol_TableBorderLight] = surface0;
	Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	Colors[ImGuiCol_TextSelectedBg] = surface2;
	Colors[ImGuiCol_DragDropTarget] = yellow;
	Colors[ImGuiCol_NavHighlight] = mauve;
	Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.35f);
	Colors[ImGuiCol_Text] = text;
	Colors[ImGuiCol_TextDisabled] = subtext0;

	// Rounded corners
	Style.WindowRounding = 6.0f;
	Style.ChildRounding = 6.0f;
	Style.FrameRounding = 4.0f;
	Style.PopupRounding = 4.0f;
	Style.ScrollbarRounding = 9.0f;
	Style.GrabRounding = 4.0f;
	Style.TabRounding = 4.0f;

	// Padding and spacing
	Style.WindowPadding = ImVec2(8.0f, 8.0f);
	Style.FramePadding = ImVec2(5.0f, 3.0f);
	Style.ItemSpacing = ImVec2(8.0f, 4.0f);
	Style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	Style.IndentSpacing = 21.0f;
	Style.ScrollbarSize = 14.0f;
	Style.GrabMinSize = 10.0f;

	// Borders
	Style.WindowBorderSize = 1.0f;
	Style.ChildBorderSize = 1.0f;
	Style.PopupBorderSize = 1.0f;
	Style.FrameBorderSize = 0.0f;
	Style.TabBorderSize = 0.0f;
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
