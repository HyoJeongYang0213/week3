#include "pch.h"
#include "FName.h"

FName::FName(const char* pStr, bool bIsClassName)
	: FName::FName(FString(pStr), bIsClassName)
{}

FName::FName(const FString& Name, bool bIsClassName)
{
	FString DisplayName;
	if (!bIsClassName)
	{
		pair<FString, int32> ParceResult = ParceNumber(Name);
		DisplayName = ParceResult.first;
		Number = ParceResult.second + 1; // 1을 더해서 저장. _0 이면 1 저장
	}
	else
	{
		DisplayName = Name.substr(1, Name.size() - 1);
	}
	FString ComparisonName = NormalizeToSmall(DisplayName);

	pair<int32, int32> Indecies = NAMEPOOL.AddName(ComparisonName, DisplayName);
	ComparisonIndex = Indecies.first;
	DisplayIndex = Indecies.second;

	if (bIsClassName)
	{
		Number = NAMEPOOL.ClaimNextIndex(ComparisonName) + 1;
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
		FString Name = NAMEPOOL.GetComparisonName(ComparisonIndex);
		FString OtherName = NAMEPOOL.GetComparisonName(Other.ComparisonIndex);
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
	FString Name = NAMEPOOL.GetDisplayName(DisplayIndex);
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

pair<FString, int32> FName::ParceNumber(const FString& Name) const
{
	constexpr int32 NO_NUMBER = -1;
	const int32 Len = Name.size();

	auto IsDigit = [](char c) { return '0' <= c && c <= '9'; };

	if (Len == 0 || !IsDigit(Name[Len - 1]))
	{
		return { Name, NO_NUMBER };
	}

	int32 Seperator = Len - 1;

	while (Seperator >= 0 && IsDigit(Name[Seperator]))
	{
		--Seperator;
	}

	if (Seperator <= 0 || Name[Seperator] != '_')
	{
		return { Name, NO_NUMBER };
	}

	const int32 DigitStart = Seperator + 1;
	const int32 DigitLen = Len - DigitStart;

	if (DigitLen >= 2 && Name[DigitStart] == '0')
	{
		return { Name, NO_NUMBER };
	}

	if (DigitLen > 10)
	{
		return { Name, NO_NUMBER };
	}

	int32 Value = 0;
	for (int32 i = DigitStart; i < Len; ++i)
	{
		Value = Value * 10 + (Name[i] - '0');
	}

	return { Name.substr(0, Seperator) , Value};
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