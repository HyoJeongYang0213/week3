#include "pch.h"
#include "ACollider.h"
#include "Global.h"


ACollider::~ACollider() {
  // 소멸 시 콜라이더 맵에서 자동 제거
  ObjectManager::GetInstance().DestroyCollider(GetID());
}

void ACollider::Destroy() {
  // 콜라이더 맵에서 등록 해제
  ObjectManager::GetInstance().DestroyCollider(GetID());

  Super::Destroy();
}

void ACollider::Move(float deltaTime) {
  if (Mass <= 0.0f || bSleeping) {
    return;
  }

  if (bUseGravity) {
    Velocity += Global::G * deltaTime;
  }

  Velocity = Velocity * (1.0f / (1.0f + deltaTime * LinearDamping));
  AngularVelocity *= 1.0f / (1.0f + deltaTime * AngularDamping);

  transform.Location += Velocity * deltaTime;
  // transform.Rotation.z += AngularVelocity * deltaTime;

  FQuaternion deltaRot = FQuaternion::FromAxisAngle(
      FVector(0.0f, 0.0f, 1.0f), AngularVelocity * deltaTime);
  transform.SetRotation((deltaRot * transform.GetRotation()).Normalized());
}

void ACollider::Pressed() {}

void ACollider::Released() {}

#include "CircleGenerator.h"
#include "SphereVertex.h"
#include "PrimitiveVertex.h"


ACube::ACube(const FLinearColor &inColor) : ACollider(inColor) {
  SetMesh(OBJECT.GetOrCreateMesh("Cube", cube_vertices, cube_indices));
  SetPrimitive(EPrimitive::Cube);
}

ASphere::ASphere(const FLinearColor &inColor) : ACollider(inColor) {
  // 구체 정점 최초 계산 후 캐시
  //static const TArray<FVertexData> sphereVertices =
  //    CreateSphereVertices(0.5f, 20, 20, false);
  SetMesh(OBJECT.GetOrCreateMesh("Sphere", sphere_vertices, sphere_indices));
  SetPrimitive(EPrimitive::Sphere);
}

ATriangle::ATriangle(const FLinearColor &inColor) : ACollider(inColor) {
  SetMesh(OBJECT.GetOrCreateMesh("Triangle", triangle_vertices, trianlge_indices));
  SetPrimitive(EPrimitive::Triangle);
}

ARectangle::ARectangle(const FLinearColor &inColor) : ACollider(inColor) {
  SetMesh(OBJECT.GetOrCreateMesh("Rectangle", rectangle_vertices, rectangle_indices));
  SetPrimitive(EPrimitive::Rectangle);
}

ACircle::ACircle(const FLinearColor &inColor) : ACollider(inColor) {
  TArray<FVertexData> circle_vertices =
      CircleGenerator::MakeCircle(32, 1.0f, 1.0f, 0.0f, 1.0f);
  SetMesh(OBJECT.GetOrCreateMesh("Circle", circle_vertices));
  SetPrimitive(EPrimitive::Circle);
}
