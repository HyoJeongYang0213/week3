#include "pch.h"
#include "Camera.h"
#include "Global.h"

Camera::Camera()
{
	//기본 카메라 위치 및 회전 설정
	Location = DefaultLocation;
	Pitch = DefaultPitch;
	Yaw = DefaultYaw;
}

void Camera::MoveForward(float delta)
{
	Location += GetForward() * delta;
}

void Camera::MoveRight(float delta)
{
	Location += GetRight() * delta;
}

void Camera::MoveWorldUp(float delta)
{
	Location += FVector::Up * delta;
}

FMatrix Camera::GetViewProjectionMatrix(float Aspect) const
{
	static FMatrix UEViewToD3D = { {
		{ 0, 0, 1, 0 },
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 0, 1 }
	} };

	return GetViewMatrix() * UEViewToD3D * GetProjectionMatrix(Aspect);
}

void Camera::Update()
{
	//카메라 이동 처리
	float currentSpeed = speed * DELTA;
	if (KEY_PRESS(ImGuiKey_W)) MoveForward(currentSpeed);
	if (KEY_PRESS(ImGuiKey_S)) MoveForward(-currentSpeed);
	if (KEY_PRESS(ImGuiKey_D)) MoveRight(currentSpeed);
	if (KEY_PRESS(ImGuiKey_A)) MoveRight(-currentSpeed);
	if (KEY_PRESS(ImGuiKey_Q)) MoveWorldUp(-currentSpeed);
	if (KEY_PRESS(ImGuiKey_E)) MoveWorldUp(currentSpeed);

	//카메라 회전 처리
	if (MOUSE_PRESS(1)) {
		ImVec2 delta = ImGui::GetIO().MouseDelta;
		Rotate(delta.x * rotationSpeed, -delta.y * rotationSpeed);
	}
}

FMatrix Camera::GetViewMatrix() const
{
	const FVector& Eye = Location;
	FVector Target = Eye + GetForward();
	FVector Up = GetUp();

	return FMatrix::LookAt(Eye, Target, Up);
}

FMatrix Camera::GetProjectionMatrix(float Aspect) const
{
	if (ProjectionMode == Orthographic) {
		return FMatrix::Orthographic(OrthoWidth, OrthoWidth / Aspect, NearZ, FarZ);
	}
	else 
	{
		float FovRadians = FovX * (Global::PI / 180.0f);
		return FMatrix::PerspectiveFov(FovRadians, Aspect, NearZ, FarZ);
	}
}
