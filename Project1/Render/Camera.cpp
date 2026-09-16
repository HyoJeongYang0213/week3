#include "pch.h"
#include "Camera.h"
#include "Global.h"
#include "Transform.h"


Camera::Camera()
{
	//기본 카메라 위치 및 회전 설정
	transform.SetLocation(FVector(3.336f, 3.282f, -4.715f));
	transform.SetRotation(FQuaternion::FromEuler(0.391f, -0.468f, 0.0f));
}

void Camera::Rotate(float deltaYaw, float deltaPitch)
{
	/////////////////
	//// 쿼터니언 ////
	////////////////

	float yawRad   = deltaYaw   * (Global::PI / 180.0f);
	float pitchRad = deltaPitch * (Global::PI / 180.0f);

	// yaw : 월드 Y축 기준
	// pitch : 카메라 로컬 X축 기준
	FQuaternion qYaw   = FQuaternion::FromAxisAngle(FVector(0.0f, 1.0f, 0.0f), yawRad);
	FQuaternion qPitch = FQuaternion::FromAxisAngle(FVector(1.0f, 0.0f, 0.0f), pitchRad);

	FQuaternion newRot = qYaw * transform.GetRotation() * qPitch;
	transform.SetRotation(newRot.Normalized());
}
	///////////////
	//// 오일러 ////
	///////////////

	// float deltaYawRad = deltaYaw * (Global::PI / 180.0f);
	// float deltaPitchRad = deltaPitch * (Global::PI / 180.0f);

	// FQuaternion rot = transform.GetRotation();
	// rot.y += deltaYawRad;
	// rot.x += deltaPitchRad;

	// // 짐벌락 방지 (-89도 ~ +89도)
	// float maxPitch = 89.0f * (Global::PI / 180.0f);
	// if (rot.x > maxPitch)
	// 	rot.x = maxPitch;
	// if (rot.x < -maxPitch)
	// 	rot.x = -maxPitch;

	// transform.SetRotation(rot);


FMatrix Camera::GetViewMatrix() const
{
	const FVector& eye = transform.GetLocation();
	FVector target = eye + transform.Forward();
	FVector up = transform.Up();

	return FMatrix::LookAt(eye, target, up);
}

FMatrix Camera::GetProjectionMatrix(float aspectRatio) const
{
	if (ProjectionMode == Orthographic) {
		return FMatrix::Orthographic(OrthoWidth, OrthoWidth/aspectRatio, NearZ, FarZ);
	}
	else {
		float fovRadians = fov * (Global::PI / 180.0f);
		return FMatrix::PerspectiveFov(fovRadians, aspectRatio, NearZ, FarZ);
	}
}

void Camera::Update()
{
	//카메라 이동 처리
	float currentSpeed = speed * DELTA;

	if (INPUT.GetKey('W')) MoveForward(-currentSpeed);
	if (INPUT.GetKey('S')) MoveForward(-currentSpeed);
	if (INPUT.GetKey('D')) MoveRight(currentSpeed);
	if (INPUT.GetKey('A')) MoveRight(-currentSpeed);
	if (INPUT.GetKey('Q')) MoveWorldUp(-currentSpeed);
	if (INPUT.GetKey('E')) MoveWorldUp(currentSpeed);

	//카메라 회전 처리
	if (INPUT.GetMouseButton(MouseButton::RIGHT)) {
		FIntPoint delta = INPUT.GetMouseDelta();
		Rotate(delta.X * rotationSpeed, delta.Y * rotationSpeed);
	}
}
