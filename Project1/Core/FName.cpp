#include "pch.h"
#include "FName.h"

FName::FName(const char* pStr)
	: FName::FName(FString(pStr))
{}

FName::FName(const FString& Name)
{
	pair<FString, int32> ParceResult = ParceNumber(Name);
	FString DisplayName = ParceResult.first;
	Number = ParceResult.second + 1; // 1을 더해서 저장. _0 이면 1 저장

	FString ComparisonName = NormalizeToSmall(DisplayName);

	pair<int32, int32> Indecies = FNamePool::Instance().AddName(ComparisonName, DisplayName);
	ComparisonIndex = Indecies.first;
	DisplayIndex = Indecies.second;
}

bool FName::operator== (const FName& Other) const
{
	if (ComparisonIndex == Other.ComparisonIndex && Number == Other.Number)
	{
		return true;
	}
	return false;
}

bool FName::operator!= (const FName& Other) const
{
	return !(*this == Other);
}

bool FName::IsNone() const
{
	return ComparisonIndex == 0 && Number == 0;
}

int32 FName::Compare(const FName& Other) const
{
	if (ComparisonIndex == Other.ComparisonIndex)
	{
		return Number - Other.Number;
	}
	else
	{
		FString Name = FNamePool::Instance().GetComparisonName(ComparisonIndex);
		FString OtherName = FNamePool::Instance().GetComparisonName(Other.ComparisonIndex);
		for (size_t i = 0; i < Name.size() && i < OtherName.size(); i++)
		{
			if (Name[i] - OtherName[i] > 0)
			{
				return 1;
			}
			else if (Name[i] - OtherName[i] < 0)
			{
				return -1;
			}
		}
		return (int32)Name.size() - (int32)OtherName.size();
	}
}

FString FName::ToString() const
{
	FString Name = FNamePool::Instance().GetDisplayName(DisplayIndex);
	if (Number)
	{
		Name += '_' + itos(Number - 1);
	}
	return Name;
}

int32 FName::GetComparisonIndex() const
{
	return ComparisonIndex;
}

int32 FName::GetDisplayIndex() const
{
	return DisplayIndex;
}

int32 FName::GetNumber() const
{
	return Number;
}

FString FName::NormalizeToSmall(const FString& Name)
{
	FString normalizedName = Name;
	for (char& c : normalizedName)
	{
		if ('A' <= c && c <= 'Z')
		{
			c = c + 'a' - 'A';
		}
	}
	return normalizedName;
}

pair<FString, int32> FName::ParceNumber(const FString& name) const
{
	bool bIsNumber = false;
	bool bIsUnderbar = false;
	int32 Number = 0;
	TArray<char> Numbers = {};
	FString ParcedName = {};
	for (char c : name)
	{
		if (bIsNumber)
		{
			Numbers.Add(c);
		}
		if (c == '_')
		{
			bIsUnderbar = true;
		}
		if (c != '_' && bIsUnderbar && '0' <= c && c <= '9')
		{
			Numbers.Add(c);
			bIsUnderbar = false;
			bIsNumber = true;
		}
		if (!bIsNumber && !bIsUnderbar)
		{
			ParcedName.push_back(c);
		}
	}
	if (bIsNumber) 
	{
		for (int32 i = 0; i < Numbers.Num(); i++)
		{
			Number = Number * 10 + (Numbers[i] - '0');
		}
	}
	else
	{
		Number = -1;
	}
	return { ParcedName, Number };
}

FString FName::itos(int32 Number) const
{
	FString result;
	if (Number == 0)
	{
		result.push_back('0');
		return result;
	}
	while (Number >= 1)
	{
		result.push_back((Number % 10) + '0');
		Number /= 10;
	}
	reverse(result.begin(), result.end());

	return result;
}