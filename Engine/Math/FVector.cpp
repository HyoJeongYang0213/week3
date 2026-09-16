#include "pch.h"
#include "FVector.h"

float FVector::LengthSquared() const
{
	return X * X + Y * Y + Z * Z;
}

float FVector::Length() const
{
	return sqrt(LengthSquared());
}

void FVector::Normalize()
{
	float length = Length();
	if (length > 0)
	{
		X /= length;
		Y /= length;
		Z /= length;
	}
}

FVector FVector::Normalized() const
{
	float length = Length();
	if (length > 0.0f)
	{
		return FVector(X / length, Y / length, Z / length);
	}
	return FVector(0.0f, 0.0f, 0.0f);
}



float FVector::Dot(const FVector& other) const
{
	return X * other.X + Y * other.Y + Z * other.Z;
}

FVector FVector::operator+(const FVector& other) const
{
	return FVector(X + other.X, Y + other.Y, Z + other.Z);
}

FVector FVector::operator-(const FVector& other) const
{
	return FVector(X - other.X, Y - other.Y, Z - other.Z);
}

FVector FVector::operator*(float scalar) const
{
	return FVector(X * scalar, Y * scalar, Z * scalar);
}

FVector FVector::operator/(float scalar) const
{
	return *this * (1 / scalar);
}

FVector& FVector::operator+=(const FVector& other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;
	return *this;
}

FVector& FVector::operator-=(const FVector& other)
{
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;
	return *this;
}
