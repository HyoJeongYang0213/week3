#pragma once

#include "Containers.h"

#include <functional>

class UObject;
class AActor;

class SaveLoadManager
{
public:
	static SaveLoadManager& GetInstance();
	static void SaveScene(const FString& path);
	static TArray<UObject*> LoadScene(const FString& path);

private:

	// <location, rotation, scale> 을 인자로 받아서 AActor*를 반환하는 공통타입 함수
	using CreatorFunc = std::function<AActor*(FVector, FQuaternion, FVector)>;

	static TMap<string, CreatorFunc>& GetActorCreatorRegistry();
	static string EPrimitiveToStr(EPrimitive prim);

	SaveLoadManager() = default;
	~SaveLoadManager() = default;
};