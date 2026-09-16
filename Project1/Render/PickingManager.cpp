#include "pch.h"
#include "PickingManager.h"
#include "Camera.h"
#include "TemplateLibrary.h"
#include "AGizmo.h"
#include "AWorldAxes.h"

FRay PickingManager::ScreenToWorldRay() const
{
	FIntPoint mousePos = INPUT.GetMousePosition();

	float screenW = RENDER.GetViewport().Width;
	float screenH = RENDER.GetViewport().Height;

	// NDC -> View 
	float ndcX = 2.0f * mousePos.X / screenW - 1.0f;
	float ndcY = -2.0f * mousePos.Y / screenH + 1.0f;


	FMatrix proj = CAMERA.GetProjectionMatrix(screenW / screenH);

	float ViewX = ndcX / proj.M[0][0];
	float ViewY = ndcY / proj.M[1][1];

	FMatrix ViewMatrix = CAMERA.GetViewMatrix();
	FMatrix InvViewMatrix = ViewMatrix.InverseAffine();

	FVector RayOrigin = CAMERA.GetLocation();
	FVector RayDirection;

	if (CAMERA.GetProjectionMode() == EProjectionMode::Orthographic)
	{
		RayOrigin += CAMERA.GetRight() * ViewX + CAMERA.GetUp() * ViewY;
		RayDirection = CAMERA.GetForward();
	}
	else if (CAMERA.GetProjectionMode() == EProjectionMode::Perspective)
	{
		// View -> World
		FVector ViewDirection(ViewX, ViewY, 1.0f);
		RayDirection = TransformDirection(ViewDirection, InvViewMatrix);
	}
	RayDirection.Normalize();

	return FRay{ RayOrigin, RayDirection };
}

AActor* PickingManager::Pick()
{
	FRay ray = PICK.ScreenToWorldRay();

	//기즈모 축 피킹 우선 검사
	if (AGizmo::MainGizmo && AGizmo::MainGizmo->GetTargetActor())
	{
		AGizmoAxis* closestAxis = nullptr;
		float closestDist = FLT_MAX;

		for (AGizmoAxis* axis : AGizmo::MainGizmo->GetAxes())
		{
			float axisDist = 0.0f;
			if (axis->bIsPicked(ray, axisDist))
			{
				if (axisDist > 0.0f && axisDist < closestDist)
				{
					closestDist = axisDist;
					closestAxis = axis;
				}
			}
		}

		if (closestAxis)
		{
			closestAxis->Picked();
			pickedObjcect = closestAxis;
			return closestAxis;
		}
	}

			//일반 액터 피킹 검사
	AActor* closest = nullptr;
	float closestDist = FLT_MAX;

	for (auto Object : OBJECT.GUObjectArray) 
	{
		AActor* Actor = Cast<AActor>(Object);
		FVector ActorOrigin = Actor->GetLocation();
		FVector RayToActor = ActorOrigin - ray.Origin;
		float DistanceRay = RayToActor.Cross(ray.Direction).Length();
		if (RayToActor.Dot(ray.Direction) < 0)
		{
			continue;
		}
		if (DistanceRay <= BoundingSphereThreshold * Actor->GetScale().Length())
		{
			if (Actor == nullptr || Cast<AGizmo>(Actor) || Cast<AWorldAxes>(Actor)) continue;

			float dist = 0.0f;
			if (Actor->bIsPicked(ray, dist) && dist < closestDist)
			{
				closestDist = dist;
				closest = Actor;
			}
		}
	}

	if (AGizmo::MainGizmo)
	{
		AGizmo::MainGizmo->SetTargetActor(closest);
	}

	pickedObjcect = closest;
	return closest;
}

void PickingManager::Pressed()
{
	
}

void PickingManager::Update()
{
	if (INPUT.GetMouseButtonDown(MouseButton::LEFT))
	{
		Pick();
	}
	else if (INPUT.GetMouseButton(MouseButton::LEFT))
	{
		if (pickedObjcect)
		{
			pickedObjcect->Pressed();
		}
	}
	else if (INPUT.GetMouseButtonUp(MouseButton::LEFT))
	{
		if (pickedObjcect)
		{
			pickedObjcect->Released();
		}
	}
}
