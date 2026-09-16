#include "pch.h"
#include "PickingManager.h"
#include "Camera.h"
#include "TemplateLibrary.h"
#include "AGizmo.h"
#include "AWorldAxes.h"

FRay PickingManager::ScreenToWorldRay(float mouseX, float mouseY, float screenW, float screenH) const
{
	float ndcX = 2.0f * mouseX / screenW - 1.0f;
	float ndcY = -2.0f * mouseY / screenH + 1.0f;

	FMatrix InvVP = CAMERA.GetViewProjectionMatrix(screenW / screenH).Inverse();

	// 개념 코드: 4차원 행벡터 연산
	FVector4 NearH = FVector4(ndcX, ndcY, 0.0f, 1.0f) * InvVP;
	FVector4 FarH = FVector4(ndcX, ndcY, 1.0f, 1.0f) * InvVP;

	FVector NearWorld = FVector{ NearH.X, NearH.Y, NearH.Z } / NearH.W;
	FVector FarWorld = FVector{ FarH.X, FarH.Y, FarH.Z } / FarH.W;

	return FRay{ NearWorld, (FarWorld - NearWorld).Normalized() };
}

FRay PickingManager::ScreenToWorldRay() const
{
	return ScreenToWorldRay(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y,
		RENDER.GetViewport().Width, RENDER.GetViewport().Height);
}

AActor* PickingManager::Pick()
{
	FRay ray = PICK.ScreenToWorldRay(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y,
		RENDER.GetViewport().Width, RENDER.GetViewport().Height);

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
	if (MOUSE_CLICK(0))
	{
		Pick();
	}
	else if (MOUSE_PRESS(0))
	{
		if (pickedObjcect)
		{
			pickedObjcect->Pressed();
		}
	}
	else if (MOUSE_UP(0))
	{
		if (pickedObjcect)
		{
			pickedObjcect->Released();
		}
	}
	
}
