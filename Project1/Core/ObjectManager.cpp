#include "ObjectManager.h"
#include "AGizmo.h"
#include "TemplateLibrary.h"
#include "pch.h"


// 객체 유효성 검사
bool ObjectManager::IsValidObject(const UObject *Target, uint32 UUID) const {
  if (!Target || UUID == 0)
    return false;

  // 등록 객체 검사
  for (const UObject *Obj : AllObjects) {
    if (Obj == Target && Obj->GetID() == UUID)
      return true;
  }

  // 기즈모 객체 검사
  if (AGizmo::MainGizmo) {
    if (AGizmo::MainGizmo == Target && AGizmo::MainGizmo->GetID() == UUID)
      return true;
    for (const AGizmoAxis *Axis : AGizmo::MainGizmo->GetAxes()) {
      if (Axis == Target && Axis->GetID() == UUID)
        return true;
    }
  }

  return false;
}

void ObjectManager::Destroy(UObject *Target) {
  for (int32 i = static_cast<int32>(AllObjects.size()) - 1; i >= 0; --i) {
    if (AllObjects[i] == Target) {
      if (ACollider *Collider = Cast<ACollider>(Target)) {
        DestroyCollider(Collider->GetID());
      }

      UObject *temp = AllObjects[i];
      AllObjects.RemoveAt(i);
      delete temp;
      break;
    }
  }
}

void ObjectManager::DestroyAllObjects() {
  for (int32 i = static_cast<int32>(AllObjects.size()) - 1; i >= 0; --i) {
    delete AllObjects[i];
  }
  AllObjects.clear();
  ColliderMap.clear();
}

void ObjectManager::DestroyAllActors() {
  for (int32 i = static_cast<int32>(AllObjects.size()) - 1; i >= 0; --i) {
    // 커스텀 캐스트 사용
    if (AActor *Actor = Cast<AActor>(AllObjects[i])) {
      Actor->Destroy();
    }
  }
}

void ObjectManager::DestroyAllColliders() {
  for (int32 i = static_cast<int32>(AllObjects.size()) - 1; i >= 0; --i) {
    if (ACollider *col = Cast<ACollider>(AllObjects[i])) {
      col->Destroy();
    }
  }
  ColliderMap.clear();
}

void ObjectManager::ReserveDestroy(UObject* target)
{
    // 1. target이 nullptr이면 반환
    if (target == nullptr) return;

    // 2. target의 UUID를 지역변수에 저장
    uint32 tmpID = target->GetID();

    // 3. PendingDestroyObjects를 순회:
    //    포인터와 UUID가 모두 같은 항목이 있으면 반환
    for (const PendingDestroyEntry& entry : PendingDestroyObjects)
    {
        if (entry.Target == target && entry.UUID == tmpID) return;
    }

    // 4. 중복이 없으면 { target, UUID }를 push_back
    PendingDestroyObjects.push_back( {target, tmpID});
}

void ObjectManager::ProcessPendingDestroy()
{
    TArray<PendingDestroyEntry> pending;
    pending.Swap(PendingDestroyObjects);

    // pending을 순회
    // IsValidObject(entry.Target, entry.UUID)가 참이면
    // entry.Target->Destroy() 호출
    for (const PendingDestroyEntry& entry : pending)
    {
        if (IsValidObject(entry.Target, entry.UUID))
        {
            entry.Target->Destroy();
        }
    }
}