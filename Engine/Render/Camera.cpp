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
	float ZoomSpeed = currentSpeed * 0.2f;
	if (ProjectionMode == EProjectionMode::Perspective)
	{
		if (INPUT.GetKey('W')) MoveForward(currentSpeed);
		if (INPUT.GetKey('S')) MoveForward(-currentSpeed);
	}
	else if (ProjectionMode == EProjectionMode::Orthographic)
	{
		if (INPUT.GetKey('W')) UpdateOrthoWidth(ZoomSpeed);
		if (INPUT.GetKey('S')) UpdateOrthoWidth(-ZoomSpeed);
	}
	if (INPUT.GetKey('D')) MoveRight(currentSpeed);
	if (INPUT.GetKey('A')) MoveRight(-currentSpeed);
	if (INPUT.GetKey('Q')) MoveWorldUp(-currentSpeed);
	if (INPUT.GetKey('E')) MoveWorldUp(currentSpeed);

	//카메라 회전 처리
	if (INPUT.GetMouseButton(MouseButton::RIGHT)) {
		FIntPoint delta = INPUT.GetMouseDelta();
		Rotate(delta.X * rotationSpeed, -delta.Y * rotationSpeed);
	}

	// 카메라 줌인/줌아웃 처리
	float wheelDelta = INPUT.GetMouseWheelDelta();

	if (wheelDelta)
	{
		if (ProjectionMode == EProjectionMode::Perspective)
		{
			MoveForward(wheelDelta * wheelSpeed);
		}
		else
		{
			UpdateOrthoWidth(wheelDelta * wheelSpeed * 0.1f);
		}
	}
}

void Camera::UpdateOrthoWidth(float ZoomSpeed)
{
	if (MinOrthoWidth > OrthoWidth)
	{
		OrthoWidth = MinOrthoWidth;
		return;
	}
	else if (OrthoWidth > MaxOrthoWidth)
	{
		OrthoWidth = MaxOrthoWidth;
		return;
	}
	OrthoWidth *= (1 - ZoomSpeed);
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
	if (ProjectionMode == EProjectionMode::Orthographic) {
		return FMatrix::Orthographic(OrthoWidth, OrthoWidth / Aspect, NearZ, FarZ);
	}
	else {
		float fovRadians = FovX * (Global::PI / 180.0f);
		return FMatrix::PerspectiveFov(fovRadians, Aspect, NearZ, FarZ);
	}
}

