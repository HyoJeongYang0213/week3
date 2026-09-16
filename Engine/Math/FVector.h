#pragma once

#include <DirectXMath.h>

struct FMatrix;

struct FVector
{
	float X, Y, Z;

	static const FVector Zero;
	static const FVector One;
	static const FVector Forward;
	static const FVector Right;
	static const FVector Up;

	constexpr FVector(float InX = 0, float InY = 0, float InZ = 0)
		: X(InX), Y(InY), Z(InZ)
	{}

	// XMVECTOR -> FVector 변환 생성자
	FVector(DirectX::FXMVECTOR v)
	{
		DirectX::XMFLOAT3 f3;
		DirectX::XMStoreFloat3(&f3, v);
		X = f3.x;
		Y = f3.y;
		Z = f3.z;
	}

	// FVector -> XMVECTOR 자동 형변환 연산자
	operator DirectX::XMVECTOR() const
	{
		return DirectX::XMVectorSet(X, Y, Z, 0.0f);
	}

	// 명시적 변환 헬퍼 함수
	DirectX::XMVECTOR ToXMVECTOR(float w = 0.0f) const
	{
		return DirectX::XMVectorSet(X, Y, Z, w);
	}

	float LengthSquared() const;
	float Length() const;
	void Normalize();			// 자기 자신 정규화
	FVector Normalized() const; 	// 정규화 후 행렬 반환
	float Dot(const FVector& other) const;

	static float Cross(FVector a, FVector b) // Z-성분
	{
		return a.X * b.Y - a.Y * b.X;
	}

	// 3D 외적
	FVector Cross(const FVector& Other)
	{
		FVector Result(
			this->Y * Other.Z - this->Z * Other.Y,
			this->Z * Other.X - this->X * Other.Z,
			this->X * Other.Y - this->Y * Other.X
		);
		return Result;
	}

	// 3D 외적
	static FVector Cross3D(const FVector& a, const FVector& b)
	{
		return FVector(
			a.Y * b.Z - a.Z * b.Y,
			a.Z * b.X - a.X * b.Z,
			a.X * b.Y - a.Y * b.X
		);
	}

	static FVector Cross(float w, FVector r) // w = (0, 0, w), r = 충돌지점 - 중심 거리
	{
		return FVector(-w * r.Y, w * r.X, 0.0f);
	}

	static FVector Cross(FVector r, float w) // w = (0, 0, w)
	{
		return FVector(w * r.Y, -w * r.X, 0.0f);
	}

	FVector operator+(const FVector& other) const;
	FVector operator-(const FVector& other) const;
	FVector operator*(float scalar) const;
	FVector operator/(float scalar) const;
	FVector& operator+=(const FVector& other);
	FVector& operator-=(const FVector& other);
};

inline const FVector FVector::Zero{ 0.0f, 0.0f, 0.0f };
inline const FVector FVector::One{ 1.0f, 1.0f, 1.0f };
inline const FVector FVector::Right{ 0.0f, 1.0f, 0.0f };
inline const FVector FVector::Forward{ 1.0f, 0.0f, 0.0f };
inline const FVector FVector::Up{ 0.0f, 0.0f, 1.0f };

struct FVector2
{
	float X, Y;

	FVector2(float InX = 0, float InY = 0)
		: X(InX), Y(InY)
	{}

	float Length() const { return sqrtf(X * X + Y * Y); }
	FVector2 Normalize() const { return FVector2(X / Length(), Y / Length()); }
	void Normalize()
	{
		float Len = Length();
		X /= Len;
		Y /= Len;
	}
};

struct FVector4
{
	float X, Y, Z, W;

	FVector4(float InX = 0, float InY = 0, float InZ = 0, float InW = 0)
		: X(InX), Y(InY), Z(InZ), W(InW)
	{}

	FVector4 operator*(const FMatrix& M) const;
};
