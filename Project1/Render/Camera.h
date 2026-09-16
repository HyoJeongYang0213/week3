#pragma once

#include "Transform.h"
#include "Containers.h"

class CameraBuffer;

enum class EProjectionMode{
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

	Transform& GetTransform() { return transform; }
	const Transform& GetTransform() const { return transform; }
	void SetTransform(const Transform& inTransform) { transform = inTransform; }

	const FVector& GetLocation() const { return transform.GetLocation(); }
	void SetLocation(const FVector& loc) { transform.SetLocation(loc); }

	const FQuaternion& GetRotation() const { return transform.GetRotation(); }
	void SetRotation(const FQuaternion& rot) { transform.SetRotation(rot); }

	FVector GetForward() const { return transform.Forward(); }
	FVector GetRight() const { return transform.Right(); }
	FVector GetUp() const { return transform.Up(); }

	void Rotate(float deltaYaw, float deltaPitch);

	void MoveForward(float delta) { transform.SetLocation(transform.GetLocation() + GetForward() * delta); }
	void MoveRight(float delta) { transform.SetLocation(transform.GetLocation() + GetRight() * delta); }
	void MoveWorldUp(float delta) { transform.SetLocation(transform.GetLocation() + FVector(0.0f, 1.0f, 0.0f) * delta); }

	float GetNear() const { return NearZ; }
	float GetFar() const { return FarZ; }
	float GetSpeed() const { return speed; }
	void SetSpeed(float inSpeed) { speed = inSpeed; }
	float& GetSpeedRef() { return speed; }

	float GetRotationSpeed() const { return rotationSpeed; }
	void SetRotationSpeed(float inRotSpeed) { rotationSpeed = inRotSpeed; }
	float& GetRotationSpeedRef() { return rotationSpeed; }

	float GetWheelSpeed() const { return wheelSpeed; }
	void SetWheelSpeed(float inWheelSpeed) { wheelSpeed = inWheelSpeed; }
	float& GetWheelSpeedRef() { return wheelSpeed; }

	float GetFOV() const { return fov; }
	void SetFOV(float inFov) { fov = inFov; }

	FMatrix GetViewMatrix() const;
	FMatrix GetProjectionMatrix(float aspectRatio) const;

	void SetProjectionMode(EProjectionMode mode) { ProjectionMode = mode; }
	EProjectionMode GetProjectionMode() { return ProjectionMode; }

	void SetOrthoWidth(float w) { OrthoWidth = w; }
	float GetOrthoWidth() { return OrthoWidth; }
	void UpdateOrthoWidth(float CurrentSpeed);

	void SetNear(float inNear) { NearZ = inNear; }
	void SetFar(float inFar) { FarZ = inFar; }

	void Update();

	EViewMode ViewMode = EViewMode::Unlit;

	EEngineShowFlags ShowFlags =
		EEngineShowFlags::SF_Primitives |
		EEngineShowFlags::SF_BillboardText;
private:
	Transform transform;

	EProjectionMode ProjectionMode = EProjectionMode::Perspective;
	
	float OrthoWidth = 25.0f;
	float MinOrthoWidth = 5.0f;
	float MaxOrthoWidth = 100.f;

	float fov = 60.0f;
	float NearZ = 0.1f;
	float FarZ = 1000.0f;

	float speed = 2.0f;
	float rotationSpeed = 0.08f;
	float wheelSpeed = 1.0f;
};

