#include "pch.h"
#include "AActor.h"
#include "AGizmo.h"
#include "FLineBatchRenderer.h"


AActor::AActor(const FLinearColor &inColor) : Color(inColor) {}

AActor::~AActor() {
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
	return mesh->IsPicked(worldRay, transform, outDistance);
  }
  return false;
}

bool AActor::IsSelected() const {
  return AGizmo::MainGizmo && this == AGizmo::MainGizmo->GetTargetActor();
}

FBoundingBox AActor::GetWorldBoundingBox() const
{
	if (mesh == nullptr) return FBoundingBox{};

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
        wmin.X = min(wmin.X, p.X);
        wmax.X = max(wmax.X, p.X);
        wmin.Y = min(wmin.Y, p.Y);
        wmax.Y = max(wmax.Y, p.Y);
        wmin.Z = min(wmin.Z, p.Z);
        wmax.Z = max(wmax.Z, p.Z);
    }

    return FBoundingBox{
    wmin.X, wmin.Y, wmin.Z,
    wmax.X, wmax.Y, wmax.Z
    };
}

void AActor::DrawingBox()
{
    FBoundingBox box = GetWorldBoundingBox();
    
    // 해당 범위들로 꼭짓점 구성하기 
    FVector edges[8];
    for (int i = 0; i < 8; i++) {
        edges[i] = FVector(
            (i & 1) ? box.minX : box.maxX,
            (i & 2) ? box.minY : box.maxY,
            (i & 4) ? box.minZ : box.maxZ
        );
    }

	FLinearColor color = FLinearColor::White;
	LINEBATCH.AddBox(edges, color, true);
}

void AActor::Update(float Deltatime) { UObject::Update(Deltatime); }
