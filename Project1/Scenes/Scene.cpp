#include "pch.h"
#include "Scene.h"

#include "AGizmo.h"
#include "ALight.h"
#include "ASkySphere.h"
#include "ATextActor.h"
#include "AWorldAxes.h"
#include "FGrid.h"
#include "FLineBatchRenderer.h"
#include "FMeshRenderer.h"
#include "FOutlineRenderer.h"
#include "RenderSystem.h"
#include "ShaderConstants.h"
#include "TemplateLibrary.h"
#include "UBillboard.h"
#include "UParticleSubUVComp.h"

Scene::Scene(): FrameBuffer(DEVICE.CreateConstantBuffer(sizeof(FrameConstants)))
{
	WorldAxes = FObjectFactory::SpawnActor<AWorldAxes>();
	Gizmo = FObjectFactory::SpawnActor<AGizmo>();
	SkySphere = FObjectFactory::SpawnActor<ASkySphere>();
}

void Scene::Update(float DeltaTime)
{
	PICK.Update();

	auto& Objects = OBJECT.GUObjectArray;
	for (size_t i = 0; i < Objects.size(); ++i)
	{
		if (Objects[i] && Objects[i]->GetIsActive())
		{
			Objects[i]->Update(DeltaTime);
		}
	}
	PICK.Update();
	Grid.Update(CAMERA.Location);

	// 삭제 예약 된 객체 정리
	OBJECT.ProcessPendingDestroy();
}

void Scene::Render()
{
	FMatrix ViewProjection = CAMERA.GetViewProjectionMatrix(RENDER.GetViewport().Width / RENDER.GetViewport().Height);
	FVector CameraLocation = CAMERA.Location;

	CONTEXT.UpdateConstantBuffer(
		FrameBuffer, 
		FrameConstants{
			.VP = ViewProjection.Transpose(),
			.CameraPos = CameraLocation });

	Grid.AddLines();

	TArray<FMeshRenderData> ObjectData;
	TArray<FMeshRenderData> TextData;
	CollectRenderData(ObjectData, TextData);

	if ((CAMERA.ShowFlags & EEngineShowFlags::SF_Primitives) != EEngineShowFlags::SF_None)
	{
		MeshRenderer.Render(FrameBuffer, ObjectData);
	}

	LINEBATCH.Render(FrameBuffer);


	if (CAMERA.ViewMode != EViewMode::Wireframe)
	{
		RenderOutline();
	}

	if (AGizmo::MainGizmo)
	{
		if (AActor* Selected = AGizmo::MainGizmo->GetTargetActor())
		{
			if (ALight* Light = Cast<ALight>(Selected))
			{
				Light->AddLines();
			}

			Selected->DrawingBox();
		}
	}
	LINEBATCH.Render(FrameBuffer, false);

	if ((CAMERA.ShowFlags & EEngineShowFlags::SF_BillboardText) != EEngineShowFlags::SF_None)
	{
		MeshRenderer.Render(FrameBuffer, TextData, false);
	}

	CONTEXT.ClearDepthStencil(RENDER.GetDepthStencilView());
	RenderGizmo();
}

// TODO: 캐스팅이 아니라 컴포넌트 기반으로 바꾸는 게 좋음
void Scene::CollectRenderData(TArray<FMeshRenderData>& ObjectData, TArray<FMeshRenderData>& TextData) const
{
	static const Material SkyMaterial{
		.VertexShader = VertexShaderType::Sky,
		.PixelShader = PixelShaderType::Sky,
		.Layout = VertexLayout::PositionColorUVNormal,
		.Texture = "SkyTexture",
		.Sampler = Sampler::LinearWrap,
		.Blend = BlendMode::None
	};

	static const Material FontMaterial
	{
		.VertexShader = VertexShaderType::Font,
		.PixelShader = PixelShaderType::Font,
		.Layout = VertexLayout::PositionColorUVNormal,
		.Texture = "FontAtlas",
		.Sampler = Sampler::LinearWrap,
		.Blend = BlendMode::AlphaBlend
	};

	auto& objects = OBJECT.GUObjectArray;
	for (size_t i = 0; i < objects.size(); ++i)
	{
		if (objects[i] && objects[i]->GetIsActive())
		{
			if (ACollider* Collider = Cast<ACollider>(objects[i]))
			{
				ObjectData.Add(FMeshRenderData{
					.Mesh = *RESOURCES.GetMesh(Collider->GetMeshName()),
					.World = Collider->GetTransform().GetWorldMatrix(),
					.Color = Collider->GetColor(),
					.bSelected = Collider->IsSelected(),
					.bWireFrame = CAMERA.ViewMode == EViewMode::Wireframe });
			}
			else if (ASkySphere* SkySphere = Cast<ASkySphere>(objects[i]))
			{
				if (CAMERA.GetProjectionMode() == EProjectionMode::Perspective)
				{
					Transform SkyTransform = SkySphere->GetTransform();
					SkyTransform.SetLocation(CAMERA.Location);

					ObjectData.Add(FMeshRenderData{
						.Mesh = *RESOURCES.GetMesh("SkySphere"),
						.Material = SkyMaterial,
						.World = SkyTransform.GetWorldMatrix(),
						.Color = SkySphere->GetColor(),
						.bSelected = false,
						.bWireFrame = CAMERA.ViewMode == EViewMode::Wireframe });
				}

			}
			else if (AWorldAxes* WorldAxes = Cast<AWorldAxes>(objects[i]))
			{
				WorldAxes->AddLines();
			}
			else if (ATextActor* TextActor = Cast<ATextActor>(objects[i]))
			{
				if (TextActor->bIsUUIDLabel && (CAMERA.ShowFlags & EEngineShowFlags::SF_BillboardText) == EEngineShowFlags::SF_None)
				{
					continue;
				}

				FString Key = TextActor->GetRenderMeshName();
				const RenderMesh* Mesh = RESOURCES.GetMesh(Key);
				if (!Mesh)
				{
					auto& Geometry = TextActor->GetGeometry();
					Mesh = &RESOURCES.CreateMesh(Key, Geometry.Vertices, Geometry.Indices);
				}

				TextData.Add(FMeshRenderData{
					.Mesh = *Mesh,
					.Material = FontMaterial,
					.World = TextActor->GetTransform().GetWorldMatrix(),
					.Color = TextActor->GetColor(),
					.bSelected = false,
					.bWireFrame = false });
			}
			else if (ALight* Light = Cast<ALight>(objects[i]))
			{
				ObjectData.Add(FMeshRenderData{
					.Mesh = *RESOURCES.GetMesh(Light->GetRenderMeshName()),
					.World = Light->GetTransform().GetWorldMatrix(),
					.Color = Light->GetColor(),
					.bSelected = Light->IsSelected() });
			}
			else if (UParticleSubUVComp* ParticleSubUV = Cast<UParticleSubUVComp>(objects[i]))
			{
				const Material ParticleSubUVMaterial
				{
					.VertexShader = VertexShaderType::SubUV,
					.PixelShader = PixelShaderType::SubUV,
					.Layout = VertexLayout::PositionColorUVNormal,
					.Texture = ParticleSubUV->GetTextureName(),
					.Sampler = Sampler::PointClamp,
					.Blend = BlendMode::AlphaBlend
				};
				ObjectData.Add(FMeshRenderData{
					.Mesh = *RESOURCES.GetMesh(ParticleSubUV->GetRenderMeshName()),
					.Material = ParticleSubUVMaterial,
					.World = ParticleSubUV->GetTransform().GetWorldMatrix(),
					.Color = ParticleSubUV->GetColor(),
					.bSelected = ParticleSubUV->IsSelected(),
					.bWireFrame = false,
					.UVScale = ParticleSubUV->GetSubUVScale(),
					.UVOffset = ParticleSubUV->GetSubUVOffset()
				});
			}
			else if (UBillboard* Billboard = Cast<UBillboard>(objects[i]))
			{
				const Material BillboardMaterial
				{
					.VertexShader = VertexShaderType::SubUV,
					.PixelShader = PixelShaderType::SubUV,
					.Layout = VertexLayout::PositionColorUVNormal,
					.Texture = Billboard->GetTextureName(),
					.Sampler = Sampler::PointClamp,
					.Blend = BlendMode::AlphaBlend
				};
				ObjectData.Add(FMeshRenderData{
					.Mesh = *RESOURCES.GetMesh(Billboard->GetRenderMeshName()),
					.Material = BillboardMaterial,
					.World = Billboard->GetTransform().GetWorldMatrix(),
					.Color = Billboard->GetColor(),
					.bSelected = Billboard->IsSelected(),
					.bWireFrame = false,
				});
			}
		}
	}
}

void Scene::RenderOutline()
{
	if (AGizmo::MainGizmo) {
		if (AActor* Selected = AGizmo::MainGizmo->GetTargetActor())
		{
			OutlineRenderer.Render(
				FrameBuffer,
				FOutlineRenderData{
					.Mesh = RESOURCES.GetMesh(Selected->GetRenderMeshName()),
					.World = Selected->GetTransform().GetWorldMatrix(),
					.PixelWidth = 4.0f,
					.ViewportWidth = RENDER.GetViewport().Width,
					.ViewportHeight = RENDER.GetViewport().Height
				}
			);
		}
	}
}

void Scene::RenderGizmo()
{
	if (AGizmo::MainGizmo && AGizmo::MainGizmo->GetTargetActor()) {
		TArray<FGizmoRenderData> GizmoRenderData;
		for (auto* Axis : AGizmo::MainGizmo->GetAxes())
		{
			GizmoRenderData.Add(FGizmoRenderData{
				.Mesh = *RESOURCES.GetMesh(Axis->GetRenderMeshName()),
				.World = Axis->GetTransform().GetWorldMatrix(),
				.Color = Axis->GetDisplayColor() });
		}
		GizmoRenderer.Render(FrameBuffer, GizmoRenderData);
	}
}
