#pragma once

#include <DirectXMath.h>

struct FVector
{
	float x, y, z;

	FVector(float _x = 0, float _y = 0, float _z = 0);

	// XMVECTOR -> FVector 변환 생성자
	FVector(DirectX::FXMVECTOR v)
	{
		DirectX::XMFLOAT3 f3;
		DirectX::XMStoreFloat3(&f3, v);
		x = f3.x;
		y = f3.y;
		z = f3.z;
	}

	// FVector -> XMVECTOR 자동 형변환 연산자
	operator DirectX::XMVECTOR() const
	{
		return DirectX::XMVectorSet(x, y, z, 0.0f);
	}

	// 명시적 변환 헬퍼 함수
	DirectX::XMVECTOR ToXMVECTOR(float w = 0.0f) const
	{
		return DirectX::XMVectorSet(x, y, z, w);
	}

	float LengthSquared() const;
	float Length() const;
	void Normalize();			// 자기 자신 정규화
	FVector Normalized() const; 	// 정규화 후 행렬 반환
	float Dot(const FVector& other) const;

	static float Cross(FVector a, FVector b) // Z-성분
	{
		return a.x * b.y - a.y * b.x;
	}

	// 3D 외적
	FVector Cross(const FVector& Other)
	{
		FVector Result(
			this->y * Other.z - this->z * Other.y,
			this->z * Other.x - this->x * Other.z,
			this->x * Other.y - this->y * Other.x
		);
		return Result;
	}

	// 3D 외적
	static FVector Cross3D(const FVector& a, const FVector& b)
	{
		return FVector(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		);
	}

	static FVector Cross(float w, FVector r) // w = (0, 0, w), r = 충돌지점 - 중심 거리
	{
		return FVector(-w * r.y, w * r.x, 0.0f);
	}

	static FVector Cross(FVector r, float w) // w = (0, 0, w)
	{
		return FVector(w * r.y, -w * r.x, 0.0f);
	}

	FVector operator+(const FVector& other) const;
	FVector operator-(const FVector& other) const;
	FVector operator*(float scalar) const;
	FVector operator/(float scalar) const;
	FVector& operator+=(const FVector& other);
	FVector& operator-=(const FVector& other);
};

struct FVector2D
{
	float x, y;

	FVector2D(float _x = 0, float _y = 0)
		: x(_x), y(_y)
	{}

	float Length() const { return sqrtf(x * x + y * y); }
	FVector2D Normalize() const { return FVector2D(x / Length(), y / Length()); }
	void Normalize()
	{
		float Len = Length();
		x /= Len;
		y /= Len;
	}
};