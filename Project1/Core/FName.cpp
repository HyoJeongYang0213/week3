#include "pch.h"
#include "FName.h"

FName::FName(const char* pStr)
	: FName::FName(FString(pStr))
{}

FName::FName(const FString name)
{
	pair<FString, int32> parceResult = ParceNumber(name);
	FString DisplayName = parceResult.first;
	Number = parceResult.second + 1; // 1을 더해서 저장. _0 이면 1 저장

	FString ComparisonName = NormalizeToSmall(DisplayName);

	if (FNamePool::Instance().IsContainsComparison(ComparisonName)) 
	{ // name이 이미 있는 경우
		ComparisonIndex = FNamePool::Instance().GetComparisonIndex(ComparisonName);
		DisplayIndex = FNamePool::Instance().AddDisplayName(DisplayName);
	}
	else 
	{ // name이 없는 경우, 새로 등록
		pair<int32, int32> Indecies = FNamePool::Instance().AddName(ComparisonName, DisplayName);
		ComparisonIndex = Indecies.first;
		DisplayIndex = Indecies.second;
	}
}

bool FName::operator== (const FName& Other) const
{
	if (ComparisonIndex == Other.ComparisonIndex && Number == Other.Number)
	{
		return true;
	}
	return false;
}

int32 FName::Compare(const FName& Other) const
{
	if (ComparisonIndex == Other.ComparisonIndex)
	{
		return Number - Other.Number;
	}
	else
	{
		FString name = FNamePool::Instance().GetComparisonName(ComparisonIndex);
		FString otherName = FNamePool::Instance().GetComparisonName(Other.ComparisonIndex);
		for (int32 i = 0; i < name.size() && i < otherName.size(); i++)
		{
			if (name[i] - otherName[i] > 0)
			{
				return 1;
			}
			else if (name[i] - otherName[i] < 0)
			{
				return -1;
			}
		}
		return name.size() - otherName.size();
	}
}

FString FName::ToString()
{
	FString name = FNamePool::Instance().GetDisplayName(DisplayIndex);
	if (Number)
	{
		name = name + '_' + itos(Number - 1);
	}
	return name;
}

FString FName::NormalizeToSmall(FString name)
{
	FString normalizedName = name;
	for (char& c : normalizedName)
	{
		if ('A' <= c && c <= 'Z')
		{
			c = c + 'a' - 'A';
		}
	}
	return normalizedName;
}

pair<FString, int32> FName::ParceNumber(FString name)
{
	bool bIsNumber = false;
	bool bIsUnderbar = false;
	int32 stoi = 0;
	TArray<char> numbers = {};
	FString parcedName = {};
	for (char& c : name)
	{
		if (bIsNumber)
		{
			numbers.Add(c);
		}
		if (c == '_')
		{
			bIsUnderbar = true;
		}
		if (c != '_' && bIsUnderbar && '0' <= c && c <= '9')
		{
			numbers.Add(c);
			bIsUnderbar = false;
			bIsNumber = true;
		}
		if (!bIsNumber && !bIsUnderbar)
		{
			parcedName.push_back(c);
		}
	}
	if (bIsNumber) 
	{
		for (int32 i = 0; i < numbers.Num(); i++)
		{
			stoi = stoi * 10 + (numbers[i] - '0');
		}
	}
	pair<FString, int32> result(parcedName, stoi);
	return result;


}

FString FName::itos(int32 number)
{
	FString string;
	if (number == 0)
	{
		string.push_back('0');
		return string;
	}
	while (number >= 1)
	{
		string.push_back((number - '0' % 10) + '0');
		number /= 10;
	}
	reverse(string.begin(), string.end());

	return string;
}