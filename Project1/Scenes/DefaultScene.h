#pragma once
#include "Scene.h"
#include "ACollider.h"
#include "AGizmo.h"
#include "AWorldAxises.h"
#include "PickingManager.h"
#include "ASkySphere.h"
#include "FGrid.h"

class DefaultScene : public Scene
{
public:
	DefaultScene();
	~DefaultScene();

	virtual void Initialize() override;
	virtual void Update(float deltatime) override;
	virtual void Render() override;


	AGizmo* gizmo = nullptr;

	AWorldAxises* worldAxises;

	FGrid Grid;

	ASkySphere* skysphere;
};

