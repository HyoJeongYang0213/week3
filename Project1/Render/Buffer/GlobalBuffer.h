#pragma once

#include "FMatrix.h"
#include "ConstBuffer.h"
#include "FLinearColor.h"
#include "FVector.h"

class MatrixBuffer : public ConstBuffer
{
private:
    struct Data
    {
        FMatrix matrix;
    };
    
    Data data;
public:
    MatrixBuffer() : ConstBuffer(&data, sizeof(Data))
    {
        data.matrix = FMatrix::Identity();
    }

    void SetMat(FMatrix mat)
    {
        data.matrix = mat.Transpose();
    }

};

class ColorBuffer : public ConstBuffer
{
private:
    struct Data
    {
        FLinearColor color = FLinearColor(0,0,0,0);
        int32 useTextrue = 0;
    };
    
    Data data;
public:
    ColorBuffer() : ConstBuffer(&data, sizeof(Data))
    {
        data.color = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
    }

    void SetColor(FLinearColor col)
    {
        data.color = col;
    }

    void SetUseTexture(int32 usetexture)
    {
        data.useTextrue = usetexture;
    }
};

class CameraBuffer : public ConstBuffer
{
public:
    struct Data
    {
        FMatrix matrix;
        FVector CameraPos;
        float pad;
        FVector CameraForward;
        float pad2;
    };

private:
    Data data;

public:
    CameraBuffer() : ConstBuffer(&data, sizeof(Data))
    {
        data.matrix = FMatrix::Identity();
        data.CameraPos = FVector(0.0f, 0.0f, 0.0f);
        data.pad = 0.0f;
    }

    void SetMat(FMatrix mat, const FVector& cameraPos, const FVector & cameraForward)
    {
        data.matrix = mat.Transpose();
        data.CameraPos = cameraPos;
        data.CameraForward = cameraForward;
    }

    void SetMat(FMatrix mat)
    {
        data.matrix = mat.Transpose();
    }

    void SetCameraPos(const FVector& cameraPos)
    {
        data.CameraPos = cameraPos;
    }
};


class SubUVBuffer : public ConstBuffer
{
private:
    struct Data
    {
        float ScaleU = 1.f, ScaleV = 1.f;
        float OffsetU = 0.f, OffsetV = 0.f;
    };
    Data data;
public:
    SubUVBuffer() : ConstBuffer(&data, sizeof(data))
    {
        data.ScaleU = 1.f;
        data.ScaleV = 1.f;
        data.OffsetU = 0.f;
        data.OffsetV = 0.f;
    }
    void SetUV(float ScaleU, float ScaleV, float OffsetU, float OffsetV)
    {
        data.ScaleU = ScaleU;
        data.ScaleV = ScaleV;
        data.OffsetU = OffsetU;
        data.OffsetV = OffsetV;
    }
    
};


