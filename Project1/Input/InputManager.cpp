#include "pch.h"

#include "InputManager.h"

InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return instance;
}

void InputManager::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_KEYDOWN:
			m_current[wParam] = true;
			break;

		case WM_LBUTTONDOWN:
			m_current[MouseButton::LEFT] = true;
			break;

		case WM_RBUTTONDOWN:
			m_current[MouseButton::RIGHT] = true;
			break;

		case WM_KEYUP:
			m_current[wParam] = false;
			break;

		case WM_LBUTTONUP:
			m_current[MouseButton::LEFT] = false;
			break;

		case WM_RBUTTONUP:
			m_current[MouseButton::RIGHT] = false;
			break;

		case WM_MOUSEMOVE:
		{
			int32 xPos = GET_X_LPARAM(lParam);
			int32 yPos = GET_Y_LPARAM(lParam);

			m_mousePosition.X = xPos;
			m_mousePosition.Y = yPos;
			break;
		}

		case WM_MOUSEWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(wParam); // 여기 breakpoint
			m_mouseWheelDelta += delta;
			break;
		}
	}
}

void InputManager::EndFrame()
{
	m_prev = m_current;
	m_prevMousePosition = m_mousePosition;
	m_mouseWheelDelta = 0;
}

// 키보드 이번 프레임 눌림
bool InputManager::GetKeyDown(WPARAM wParam)
{
	if (ImGui::GetIO().WantCaptureKeyboard) return false;
	return !m_prev[wParam] && m_current[wParam];
}

// 키보드 현재 눌려 있음
bool InputManager::GetKey(WPARAM wParam)
{
	bool bCaptureKeyboard = ImGui::GetIO().WantCaptureKeyboard;
	bool bCurrent = m_current[wParam];

	if (bCaptureKeyboard) return false;
	return m_current[wParam];
}

// 키보드 이번 프레임 뗌
bool InputManager::GetKeyUp(WPARAM wParam)
{
	if (ImGui::GetIO().WantCaptureKeyboard) return false;
	return m_prev[wParam] == true && m_current[wParam] == false;
}

// 마우스 이번 프레임 눌림
bool InputManager::GetMouseButtonDown(MouseButton type)
{
	if (ImGui::GetIO().WantCaptureMouse) return false;
	return m_prev[type] == false && m_current[type] == true;
}

// 마우스 현재 눌려 있음
bool InputManager::GetMouseButton(MouseButton type)
{
	if (ImGui::GetIO().WantCaptureMouse) return false;
	return m_current[type];
}

// 마우스 이번 프레임 뗌
bool InputManager::GetMouseButtonUp(MouseButton type)
{
	if (ImGui::GetIO().WantCaptureMouse) return false;
	return m_prev[type] == true && m_current[type] == false;
}

// 현재 마우스 위치
FIntPoint InputManager::GetMousePosition()
{
	return m_mousePosition;
}

// 이전 프레임 ~ 현재 프레임 마우스 위치 델타
FIntPoint InputManager::GetMouseDelta()
{
	return m_mousePosition - m_prevMousePosition;
}

// 이전 프레임 ~ 현재 프레임 마우스 휠 델타
float InputManager::GetMouseWheelDelta()
{
	if (ImGui::GetIO().WantCaptureMouse) return 0.0f;
	return static_cast<float>(m_mouseWheelDelta) / WHEEL_DELTA;
}