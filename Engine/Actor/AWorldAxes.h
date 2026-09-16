#pragma once
#include "AActor.h"

class AWorldAxes : public AActor
{
	DECLARE_CLASS(AWorldAxes, AActor)

public:
	AWorldAxes();
	virtual void AddLines();

	virtual bool IsEditorOnly() const override { return true; }
};

