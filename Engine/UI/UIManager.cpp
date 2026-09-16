#include "pch.h"
#include "UIPanel.h"
#include "UIManager.h"

#include "ConsoleWindow.h"

// "IMGUI" 매크로 등록 완료
UIManager& UIManager::GetInstance()
{
	static UIManager instance;
	return instance;
}

// panel 생성
void UIManager::Initialize()
{
	RegisterPanel(new UIPanel_Debug);
	RegisterPanel(new UIPanel_SceneCamera);
	RegisterPanel(new UIPanel_Spawn);
	RegisterPanel(new UIPanel_Picking);
	RegisterPanel(new UIPanel_SceneManager);
}

// panel 등록
void UIManager::RegisterPanel(IUIPanel* panel)
{
	Panels.push_back(std::move(panel));
}

// 모든 panel render
void UIManager::RenderAll()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Window"))
		{
			for (IUIPanel* panel : Panels)
			{
				if (ImGui::MenuItem(panel->GetName().c_str(), nullptr, panel->IsOpen()))
				{
					panel->SetOpen(!panel->IsOpen());
				}
			}
			ImGui::MenuItem("Console", nullptr, &ConsoleWindow::GetInstance().bIsOpen);

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	for (IUIPanel* panel : Panels)
	{
		if (panel->IsOpen()) panel->Render();
	}
}

// ~UIManager()에서 사용하는 panel clear 함수
void UIManager::Shutdown()
{
	for (IUIPanel* panel : Panels)
	{
		delete panel;
	}
	Panels.clear();
}

UIManager::~UIManager()
{
	Shutdown();
}