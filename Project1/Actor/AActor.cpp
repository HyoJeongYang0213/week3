#include "AActor.h"
#include "AGizmo.h"
#include "Intersection.h"
#include "PickingManager.h"
#include "Renderer.h"
#include "pch.h"
#include "ULineBatch.h"


AActor::AActor(const FLinearColor &inColor) : Color(inColor) {
  worldBuffer = new MatrixBuffer();
}

AActor::~AActor() {
  if (worldBuffer) {
    delete worldBuffer;
    worldBuffer = nullptr;
  }

  if (bOwnsMesh && mesh) {
    delete mesh;
    mesh = nullptr;
  }
}

void AActor::Destroy() {
  // 기즈모 선택 대상 해제
  if (AGizmo::MainGizmo && AGizmo::MainGizmo->GetTargetActor() == this) {
    AGizmo::MainGizmo->SetTargetActor(nullptr);
  }

  Super::Destroy();
}

bool AActor::bIsPicked(const FRay &worldRay, float &outDistance) {
  if (mesh) {
    return mesh->bIsPicked(worldRay, transform, outDistance);
  }
  return false;
}

void AActor::SetWorldBuffer() {
  worldBuffer->SetMat(transform.WorldMat);
  worldBuffer->SetVSBuffer(0);
}

bool AActor::IsSelected() const {
  return AGizmo::MainGizmo && this == AGizmo::MainGizmo->GetTargetActor();
}

void AActor::DrawingBox()
{
    FBoundingBox box = mesh->GetBoundingBox();
    
    FVector localedges[8];
    for (int i = 0; i < 8; i++) {
        localedges[i] = FVector(
            (i & 1) ? box.minX : box.maxX,
            (i & 2) ? box.minY : box.maxY,
            (i & 4) ? box.minZ : box.maxZ
        );
    }

    // 로컬 -> 월드 좌표
    const FMatrix& world = transform.GetWorldMatrix();
    FVector wmin = TransformPoint(localedges[0], world);
    FVector wmax = wmin;

    // 월드좌표 내 [min, max] 범위 구하기 
    for (int i = 1; i < 8; i++) {
        FVector p = TransformPoint(localedges[i], world);
        wmin.x = min(wmin.x, p.x);
        wmax.x = max(wmax.x, p.x);
        wmin.y = min(wmin.y, p.y);
        wmax.y = max(wmax.y, p.y);
        wmin.z = min(wmin.z, p.z);
        wmax.z = max(wmax.z, p.z);
    }
    
    // 해당 범위들로 꼭짓점 구성하기 
    FVector edges[8];
    for (int i = 0; i < 8; i++) {
        edges[i] = FVector(
            (i & 1) ? wmin.x : wmax.x,
            (i & 2) ? wmin.y : wmax.y,
            (i & 4) ? wmin.z : wmax.z
        );
    }


    FLinearColor color = FLinearColor::White;
    LINEBATCH.AddLine(edges[0], edges[1], color);
    LINEBATCH.AddLine(edges[0], edges[2], color);
    LINEBATCH.AddLine(edges[0], edges[4], color);
    LINEBATCH.AddLine(edges[1], edges[3], color);
    LINEBATCH.AddLine(edges[1], edges[5], color);
    LINEBATCH.AddLine(edges[3], edges[7], color);
    LINEBATCH.AddLine(edges[4], edges[5], color);
    LINEBATCH.AddLine(edges[4], edges[6], color);
    LINEBATCH.AddLine(edges[5], edges[7], color);
    LINEBATCH.AddLine(edges[6], edges[7], color);
    LINEBATCH.AddLine(edges[2], edges[3], color);
    LINEBATCH.AddLine(edges[2], edges[6], color);
}

void AActor::Render() {
  UObject::Render();

  SetWorldBuffer();

  if (mesh) {
    const bool bSelected = IsSelected();
    if (bSelected) {
      RENDERER.SetSelectedState();
    }

    mesh->SetColor(Color);
    mesh->Render();

    if (bSelected) {
      RENDERER.SetDefaultDepthState();
      DrawingBox();
    }
  }
}

void AActor::Update(float Deltatime) { UObject::Update(Deltatime); }
