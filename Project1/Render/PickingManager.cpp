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

	float viewX = ndcX / proj.M[0][0];
	float viewY = ndcY / proj.M[1][1];

	FMatrix view = CAMERA.GetViewMatrix();
	FMatrix invView = view.InverseAffine();

	// View -> World
	FVector viewDirection(viewX, viewY, 1.0f);
	FVector worldDirection = TransformDirection(viewDirection, invView);
	worldDirection.Normalize();

	FVector worldOrigin = CAMERA.GetLocation();
	return FRay{ worldOrigin, worldDirection };
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

	for (auto object : OBJECT.GUObjectArray) {
		AActor* actor = Cast<AActor>(object);
		if (actor == nullptr || Cast<AGizmo>(actor) || Cast<AWorldAxes>(actor)) continue;

		float dist = 0.0f;
		if (actor->bIsPicked(ray, dist) && dist < closestDist)
		{
			closestDist = dist;
			closest = actor;
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
