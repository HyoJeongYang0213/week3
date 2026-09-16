#pragma once

#include "FIntPoint.h"

#include <bitset>

enum MouseButton
{
	LEFT = 0x01,
	RIGHT = 0x02
};

class InputManager
{
public:
	static InputManager& GetInstance();

	void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void EndFrame();

	bool GetKeyDown(WPARAM wParam);
	bool GetKey(WPARAM wParam);
	bool GetKeyUp(WPARAM wParam);

	bool GetMouseButtonDown(MouseButton type);
	bool GetMouseButton(MouseButton type);
	bool GetMouseButtonUp(MouseButton type);

	FIntPoint GetMousePosition();
	FIntPoint GetMouseDelta();

	float GetMouseWheelDelta();

private:
	InputManager() = default;
	~InputManager() = default;
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

	std::bitset<256> m_current{};
	std::bitset<256> m_prev{};

	FIntPoint m_mousePosition{};
	FIntPoint m_prevMousePosition{};

	int32 m_mouseWheelDelta = 0;
};