#pragma once
#include "Containers.h"










#include <iostream>

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

	bool IsContainsComparison(const FString& ComparisonName) const
	{
		if (ComparisonMap.Contains(ComparisonName))
		{
			return true;
		}
		return false;
	}

	bool IsContainsDisplay(const FString& DisplayName) const
	{
		if (DisplayMap.Contains(DisplayName))
		{
			return true;
		}
		return false;
	}

	pair<int32, int32> AddName(const FString& ComparisonName, const FString& DisplayName)
	{
		int32 ComparisonIndex;
		int32 DisplayIndex;

		if (int32* FoundComparisonIndex = ComparisonMap.Find(ComparisonName))
		{ // name이 이미 있는 경우
			ComparisonIndex = *FoundComparisonIndex;
			DisplayIndex = AddDisplayName(DisplayName);
		}
		else
		{ // 새로 pool에 등록.
			ComparisonIndex = ComparisonList.Num();
			DisplayIndex = DisplayList.Num();

			ComparisonList.Add(ComparisonName);
			ComparisonMap.Add(ComparisonName, ComparisonIndex);

			DisplayList.Add(DisplayName);
			DisplayMap.Add(DisplayName, DisplayIndex);
		}
		return { ComparisonIndex, DisplayIndex };
	}

	int32 AddDisplayName(const FString& DisplayName)
	{
		if (int32* DisplayIndex = DisplayMap.Find(DisplayName))
		{
			return *DisplayIndex;
		}
		else
		{
			DisplayList.Add(DisplayName);
			DisplayMap.Add(DisplayName, DisplayList.Num() - 1);
			return DisplayList.Num() - 1;
		}
	}

	int32 GetComparisonIndex(const FString& ComparisonName) // O(1)
	{
		if (int32* ComparisonIndex = ComparisonMap.Find(ComparisonName))
		{
			return *ComparisonIndex;
		}
		return -1;
	}

	FString GetComparisonName(const int32 index) const // O(1)
	{
		return ComparisonList[index];
	}

	int32 GetDisplayIndex(const FString& DisplayName) // O(1)
	{
		if (int32* DisplayIndex = DisplayMap.Find(DisplayName))
		{
			return *DisplayIndex;
		}
		return -1;
	}

	FString GetDisplayName(const int32 index) const // O(1)
	{
		return DisplayList[index];
	}

	void Debug()
	{
		for (int32 i = 0; i < DisplayList.Num(); i++)
		{
			UE_LOG("%d", DisplayList[i]);
		}
		cout << endl;
		for (int32 i = 0; i < ComparisonList.Num(); i++)
		{
			UE_LOG("%s", ComparisonList[i]);
		}
	}
};