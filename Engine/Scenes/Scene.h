#pragma once

#include "FMeshRenderer.h"
#include "FOutlineRenderer.h"
#include "ConstantBuffer.h"
#include "FGizmoRenderer.h"
#include "FGrid.h"

class ASkySphere;
class AWorldAxes;
class AGizmo;

class Scene
{   
public:
	Scene();

	void Update(float DeltaTime);
	void Render();
	FGrid& GetGrid() { return Grid; }

private:
	void CollectRenderData(TArray<FMeshRenderData>& ObjectData, TArray<FMeshRenderData>& TextData) const;
	void RenderOutline();
	void RenderGizmo();

	FMeshRenderer MeshRenderer;
	FOutlineRenderer OutlineRenderer;
	FGizmoRenderer GizmoRenderer;

	ConstantBuffer FrameBuffer;

	AGizmo* Gizmo = nullptr;

	AWorldAxes* WorldAxes;
	FGrid Grid;
	ASkySphere* SkySphere;
};

