#pragma once

#include "UObject.h"
#include "TemplateLibrary.h"
#include "Camera.h"
#include "Renderer.h"
#include "PickingManager.h"
#include "AGizmo.h"
#include "FMeshRenderer.h"
#include "FOutlineRenderer.h"
#include "ShaderConstants.h"

class Scene
{   

public:
	Scene() : FrameBuffer(DEVICEN.CreateConstantBuffer(sizeof(FrameConstants))) {}
    virtual ~Scene() {}
    virtual void Initialize() {}
    virtual void Enter() {}
    virtual void Exit() {}
    
    virtual void Update(float deltaTime)
    {
        auto& objects = OBJECT.GUObjectArray;
        for (size_t i = 0; i < objects.size(); ++i)
        {
            if (objects[i] && objects[i]->GetIsActive())
            {
                objects[i]->Update(deltaTime);
            }
        }

        PICK.Update();//pickmanager

        // 삭제 예약 된 객체 정리
        OBJECT.ProcessPendingDestroy();
    }

    virtual void Render()
    {
		FMatrix ViewProjection = CAMERA.GetViewMatrix() * CAMERA.GetProjectionMatrix(RENDER.GetViewport().Width / RENDER.GetViewport().Height);
		FVector CameraLocation = CAMERA.GetLocation();

        CONTEXT.UpdateConstantBuffer(FrameBuffer, FrameConstants{
            .VP = ViewProjection.Transpose(),
            .CameraPos = CameraLocation,
        });

        auto& objects = OBJECT.GUObjectArray;
        TArray<FMeshRenderData> MeshRenderData;
        for (size_t i = 0; i < objects.size(); ++i)
        {
            if (objects[i] && objects[i]->GetIsActive())
            {
                // 기즈모는 모든 오브젝트와 아웃라인 렌더링 후 맨 마지막에 렌더링
                if (objects[i] == AGizmo::MainGizmo)
                    continue;

                ACollider* Collider = Cast<ACollider>(objects[i]);
                if (Collider && Collider->GetIsActive() && !Collider->IsSelected())
                {
                    MeshRenderData.Add(FMeshRenderData{
	                    .Mesh = RESOURCES.GetMesh(Collider->GetMeshName()),
	                    .Material = nullptr,
	                    .World = Collider->GetTransform().GetWorldMatrix(),
	                    .Color = Collider->GetColor()});
                }
                else
                {
                    objects[i]->Render();
                }
            }
        }
		MeshRenderer.Render(
			FrameBuffer,
			MeshRenderData,
			CAMERA.ViewMode == EViewMode::Wireframe);


        if (AGizmo::MainGizmo) {
            AActor* selected = AGizmo::MainGizmo->GetTargetActor();
            if (selected)
            {
                OutlineRenderer.Render(
                    FrameBuffer,
                    FOutlineRenderData{
                        .Mesh = RESOURCES.GetMesh(selected->GetRenderMeshName()),
                        .World = selected->GetTransform().GetWorldMatrix(),
                        .PixelWidth = 4.0f,
                        .ViewportWidth = RENDER.GetViewport().Width,
                        .ViewportHeight = RENDER.GetViewport().Height
                    }
                );
            }
        }
    }

private:
    FMeshRenderer MeshRenderer;
	FOutlineRenderer OutlineRenderer;

protected:
    ConstantBuffer FrameBuffer;
};

