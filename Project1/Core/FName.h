#pragma once
#include "FNamePool.h"
#include "Containers.h"
#include <stdexcept>

struct FName
{
private:
	int32 DisplayIndex = 0; // 대소문자를 보존한 id
	int32 ComparisonIndex = 0; // 소문자로 정규화한 id. 비교/해시에 사용
	int32 Number = 0; // 숫자 접미사. this->Number = 실제 숫자 + 1

public:
	FName() = default;
	FName(const char* pStr);
	FName(const FString& str);

	bool operator== (const FName& Other) const;
	bool operator!= (const FName& Other) const;

	int32 Compare(const FName& Other) const;
	FString ToString() const;

	
private:
	FString NormalizeToSmall(const FString& str);
	pair<FString, int32> ParceNumber(const FString& name) const;
	FString itos(int32 number) const; 
};