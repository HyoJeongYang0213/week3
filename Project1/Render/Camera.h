#pragma once

#include "Containers.h"
#include "FMatrix.h"
#include "FQuaternion.h"
#include "FVector.h"

class CameraBuffer;

enum {
	Perspective,
	Orthographic,
};

enum class EViewMode : uint8
{
	Unlit,
	Wireframe,
};

class Camera
{
public:
	Camera();

	static Camera& GetInstance() {
		static Camera instance;
		return instance;
	}
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	static constexpr FVector DefaultLocation{ -4.715f, 3.336f, 3.282f };
	static constexpr float DefaultYaw = -36.809f;
	static constexpr float DefaultPitch = -23.668f;

	FVector GetForward() const { return GetRotation().RotateVector(FVector::Forward); }
	FVector GetRight() const { return GetRotation().RotateVector(FVector::Right); }
	FVector GetUp() const { return GetRotation().RotateVector(FVector::Up); }
	FQuaternion GetRotation() const { return FQuaternion::FromEulerDegrees(0.0f, Pitch, Yaw); }

	void Rotate(float DeltaYaw, float DeltaPitch) {
		Yaw += DeltaYaw;
		Pitch += DeltaPitch;
		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;
	}
	void MoveForward(float delta);
	void MoveRight(float delta);
	void MoveWorldUp(float delta);

	float GetNear() const { return NearZ; }
	float GetFar() const { return FarZ; }
	float GetSpeed() const { return speed; }
	void SetSpeed(float inSpeed) { speed = inSpeed; }
	float& GetSpeedRef() { return speed; }

	float GetRotationSpeed() const { return rotationSpeed; }
	void SetRotationSpeed(float inRotSpeed) { rotationSpeed = inRotSpeed; }
	float& GetRotationSpeedRef() { return rotationSpeed; }

	float GetFOVX() const { return FovX; }
	void SetFOVX(float InFovX) { FovX = InFovX; }

	FMatrix GetViewProjectionMatrix(float Aspect) const;

	void SetProjectionMode(int mode) { ProjectionMode = mode; }
	int GetProjectionMode() { return ProjectionMode; }

	void SetOrthoWidth(float w) { OrthoWidth = w; }
	float GetOrthWidth() { return OrthoWidth; }

	void Update();

	FVector Location{ 0.0f, 0.0f, 0.0f };
	float Yaw = 0.0f;
	float Pitch = 0.0f;

	EViewMode ViewMode = EViewMode::Unlit;

	EEngineShowFlags ShowFlags =
		EEngineShowFlags::SF_Primitives |
		EEngineShowFlags::SF_BillboardText;

private:
	FMatrix GetViewMatrix() const;
	FMatrix GetProjectionMatrix(float Aspect) const;

	int ProjectionMode = Perspective;
	
	float OrthoWidth = 10.0f;

	float FovX = 60.0f;
	float NearZ = 0.1f;
	float FarZ = 1000.0f;
	float speed = 2.0f;
	float rotationSpeed = 0.08f;
};

