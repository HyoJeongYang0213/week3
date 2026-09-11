#pragma once
#include "FNamePool.h"
#include "Containers.h"
#include <stdexcept>

constexpr int32 NAME_None = 0;

struct FName
{
private:
	int32 DisplayIndex = -1; // 대소문자를 보존한 id
	int32 ComparisonIndex = -1; // 소문자로 정규화한 id. 비교/해시에 사용
	int32 Number = NAME_None; // 숫자 접미사. this->Number = 실제 숫자 + 1

public:
	FName() = default;

	FName(const char* pStr);
	FName(const FString str);

	FName* GetInstance();

	bool operator== (const FName& Other) const;

	int32 Compare(const FName& Other) const;
	FString NormalizeToSmall(FString str);
	pair<FString, int32> ParceNumber(FString name);
	FString ToString();
	FString itos(int32 number);
};