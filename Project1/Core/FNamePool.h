#pragma once
#include "Containers.h"

struct FNamePool
{
private:
	TMap<FString, int32> ComparisonMap;
	TMap<FString, int32> DisplayMap; 
	TArray<FString> ComparisonList;
	TArray<FString> DisplayList;
	TMap<FString, int32> ClassCountMap;

public:
	FNamePool();
	static FNamePool& GetInstance();

	pair<int32, int32> AddName(const FString& ComparisonName, const FString& DisplayName);
	int32 AddDisplayName(const FString& DisplayName);

	int32 GetComparisonIndex(const FString& ComparisonName); // O(1)
	const FString& GetComparisonName(int32 index) const; // O(1)
	int32 GetDisplayIndex(const FString& DisplayName); // O(1)
	const FString& GetDisplayName(int32 index) const; // O(1)
	int32 ClaimNextIndex(const FString& ClassName);
};