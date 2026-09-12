#include "pch.h"
#include "FNamePool.h"

FNamePool::FNamePool()
{
	ComparisonList.Add("");
	ComparisonMap.Add("", 0);
	DisplayList.Add("");
	DisplayMap.Add("", 0);
}

FNamePool& FNamePool::Instance()
{
	static FNamePool instance;
	return instance;
}

pair<int32, int32> FNamePool::AddName(const FString& ComparisonName, const FString& DisplayName)
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

int32 FNamePool::AddDisplayName(const FString& DisplayName)
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

int32 FNamePool::GetComparisonIndex(const FString& ComparisonName) // O(1)
{
	if (int32* ComparisonIndex = ComparisonMap.Find(ComparisonName))
	{
		return *ComparisonIndex;
	}
	return -1;
}

const FString& FNamePool::GetComparisonName(int32 index) const // O(1)
{
	return ComparisonList[index];
}

int32 FNamePool::GetDisplayIndex(const FString& DisplayName) // O(1)
{
	if (int32* DisplayIndex = DisplayMap.Find(DisplayName))
	{
		return *DisplayIndex;
	}
	return -1;
}

const FString& FNamePool::GetDisplayName(int32 index) const // O(1)
{
	return DisplayList[index];
}