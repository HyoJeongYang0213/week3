#pragma once
#include "Containers.h"

struct FNamePool
{
private:
	TMap<FString, int32> ComparisonMap;
	TMap<FString, int32> DisplayMap; 
	TArray<FString> ComparisonList;
	TArray<FString> DisplayList;

public:
	static FNamePool& Instance()
	{
		static FNamePool instance;
		return instance;
	}

	bool IsContainsComparison(FString ComparisonName)
	{
		if (ComparisonMap.Contains(ComparisonName))
		{
			return true;
		}
		return false;
	}

	bool IsContainsDisplay(FString DisplayName)
	{
		if (DisplayMap.Contains(DisplayName))
		{
			return true;
		}
		return false;
	}

	pair<int32, int32> AddName(FString ComparisonName, FString DisplayName)
	{
		ComparisonList.Add(ComparisonName);
		ComparisonMap.Add(ComparisonName, ComparisonList.Num() - 1);

		DisplayList.Add(DisplayName);
		DisplayMap.Add(DisplayName, DisplayList.Num() - 1);

		pair<int32, int32> indices(ComparisonList.Num() - 1, DisplayList.Num() - 1);
		return indices;
	}

	int32 AddDisplayName(FString DisplayName)
	{
		if (IsContainsDisplay(DisplayName))
		{
			return GetDisplayIndex(DisplayName);
		}
		else
		{
			DisplayList.Add(DisplayName);
			DisplayMap.Add(DisplayName, DisplayList.Num() - 1);
			return GetDisplayIndex(DisplayName);
		}
	}

	int32 GetComparisonIndex(FString ComparisonName) // O(1)
	{
		if (IsContainsComparison(ComparisonName))
		{
			return ComparisonMap[ComparisonName];
		}
		return -1;
	}

	FString GetComparisonName(int32 index) // O(1)
	{
		return ComparisonList[index];
	}

	int32 GetDisplayIndex(FString DisplayName) // O(1)
	{
		if (IsContainsDisplay(DisplayName))
		{
			return DisplayMap[DisplayName];
		}
		return -1;
	}

	FString GetDisplayName(int32 index) // O(1)
	{
		return DisplayList[index];
	}
};