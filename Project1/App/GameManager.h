#pragma once
#include "FVertexSimple.h"

class GameManager
{
private:
	HWND m_mainWindow = nullptr;
	
	void Initwindow(HINSTANCE hInstance);
	void InitImgui();

	void Update();
	void Render();

	void BuildImGuiLayout();
	
public:
	GameManager();
	~GameManager();

	static GameManager* Instance;
	static GameManager& GetInstance() { return *Instance; }

	void Init(HINSTANCE hInstance);
	void mainLoop();
	void ReleaseAll();

	HWND GetMainWindow() const { return m_mainWindow; }
};

// 기존 이름 호환 별칭
using App = GameManager;
