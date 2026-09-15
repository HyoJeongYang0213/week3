#include "pch.h"
#include "UIPanel.h"
#include "Global.h"
#include "ATextActor.h"
#include "AActor.h"

#include "APointLight.h"
#include "ASpotLight.h"
#include "ADirectionalLight.h"

#include <random>
#include <DefaultScene.h>

void UIPanel_Memory::Render()
{
	// ImGui::SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond_FirstUseEver);

	//////////////////////////////
	/////// 메모리&객체 Stat ////// 
	/////////////////////////////
	ImGui::Begin("Memory & Object Status", &bIsOpen, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("[ Used Memory ] %u bytes", OBJECT.TotalAllocationBytes);
	ImGui::Text("[ No. of Obj. ] %u", OBJECT.TotalAllocationCount);
	
	ImGui::End();
}

void UIPanel_Camera::Render()
{
	ImGui::Begin("Scene & Camera Debug", &bIsOpen, ImGuiWindowFlags_AlwaysAutoResize);

	// 카메라 디버그 섹션
	ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.4f, 1.0f), "[ Camera Controls ]");
	Camera& cam = CAMERA;

    // 카메라 직교투영 여부 선택 체크박스
    bool isOrtho = (cam.GetProjectionMode() == Orthographic);
    if (ImGui::Checkbox("Orthgraphic", &isOrtho)) {
        cam.SetProjectionMode(isOrtho ? Orthographic : Perspective);
    }

    // 에디터 뷰 모드 선택 콤보박스
	int ViewMode = static_cast<int>(cam.ViewMode);
	if (ImGui::Combo("View Mode", &ViewMode, "Unlit\0Wireframe\0"))
	{
		cam.ViewMode = static_cast<EViewMode>(ViewMode);
	}

    // 카메라 시야각 조절 슬라이더
    float fov = cam.GetFOV();
    if (ImGui::SliderFloat("FOV", &fov, 10.0f, 150.0f))
        cam.SetFOV(fov);
    ImGui::Text("FOV: %.3f", cam.GetFOV());
    
    // 카메라 위치 조절 슬라이더
    FVector camLoc = cam.GetLocation();
	if (ImGui::DragFloat3("Cam Pos", &camLoc.x, 0.05f, -20.0f, 20.0f))
	{
		cam.SetLocation(camLoc);
	}

    // 카메라 각도 조절 슬라이더
    FVector camEuler = FQuaternion::ToEuler(cam.GetRotation());
	if (ImGui::DragFloat3("Cam Rot", &camEuler.x, 0.01f, -3.14f, 3.14f))
	{
		cam.SetRotation(FQuaternion::FromEuler(camEuler.x, camEuler.y, camEuler.z));
	}

	//카메라 이동 속도 및 회전 마우스 감도 조절 슬라이더
	ImGui::SliderFloat("Move Speed", &cam.GetSpeedRef(), 0.5f, 20.0f, "%.1f");
	ImGui::SliderFloat("Mouse Sensitivity", &cam.GetRotationSpeedRef(), 0.01f, 0.5f, "%.3f");

    // 카메라 설정 리셋 버튼
	if (ImGui::Button("Reset Camera"))
	{
		cam.SetLocation(FVector(3.336f, 3.282f, -4.715f));
		cam.SetRotation(FQuaternion::FromEuler(0.391f, -0.468f, 0.0f));
	}
	
    // 카메라 앞(Forward)벡터 출력 텍스트
	FVector camFwd = cam.GetForward();
	ImGui::Text("Forward: (%.2f, %.2f, %.2f)", camFwd.x, camFwd.y, camFwd.z);

    ImGui::End();
}

void UIPanel_Spawn::Render()
{
    ImGui::Begin("Spawn Primitives", &bIsOpen, ImGuiWindowFlags_AlwaysAutoResize);

	// Spawn 버튼
	// ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.8f, 1.0f), "[ Spawn Primitives ]");
	
	// Select number
	static int spawnCount = 1;
	ImGui::InputInt("No. of Prim", &spawnCount);
	if (spawnCount < 0) spawnCount = 0;

	// Select Primitives
	static int selected_item = 0;
	const char* items[] = { "Sphere", "Cube", "Circle", "Rectangle", "Triangle", "PointLight", "SpotLight", "DirectionalLight"};
	ImGui::Combo("##Primitives", &selected_item, items, IM_ARRAYSIZE(items));

	// 난수 생성 및 범위 설정 -> spawn 위치 지정을 위해
	// 화면 안에 spawn 되도록 수정
	FVector camLocation = CAMERA.GetLocation();
	FVector camForward = CAMERA.GetForward();

	static std::mt19937 rng(std::random_device{}());
	static std::uniform_real_distribution<float> distSide(-3.0f, 3.0f);   // 좌우 범위
	static std::uniform_real_distribution<float> distUp(0.0f, 2.0f);      // 상하 범위 (Grid 위로 한정)

	float spawnDistance = 8.0f; // 카메라 앞으로 얼마나 떨어뜨릴지

	// 카메라의 오른쪽 벡터 (forward와 up의 외적)
	FVector worldUp(0.0f, 1.0f, 0.0f);
	FVector camRight = FVector::Cross3D(camForward, worldUp).Normalized();

	FVector spawnCenter = camLocation + camForward * spawnDistance;

	FVector randomLoc = spawnCenter + camRight * distSide(rng) + worldUp * distUp(rng);

	if (ImGui::Button("Spawn"))
	{
		for (int i=0; i<spawnCount; i++)
		{	
            AActor* spawnedActor = nullptr;
			switch(selected_item)
			{
				case 0 :
                    spawnedActor = FObjectFactory::SpawnColider<ASphere>(randomLoc, { 1.0f, 1.0f, 1.0f });
					break;
				case 1 :
                    spawnedActor = FObjectFactory::SpawnColider<ACube>(randomLoc, { 1.0f, 1.0f, 1.0f });
					break;
				case 2 :
                    spawnedActor = FObjectFactory::SpawnColider<ACircle>(randomLoc, { 1.0f, 1.0f, 1.0f });
					break;
				case 3 : 
                    spawnedActor = FObjectFactory::SpawnColider<ARectangle>(randomLoc, { 1.0f, 1.0f, 1.0f });
					break;
				case 4 :
                    spawnedActor = FObjectFactory::SpawnColider<ATriangle>(randomLoc, { 1.0f, 1.0f, 1.0f });
					break;
                case 5 :
                    spawnedActor = FObjectFactory::SpawnActor<APointLight>(randomLoc, { 0.2f, 0.2f, 0.2f });
                    break;
                case 6:
                    spawnedActor = FObjectFactory::SpawnActor<ASpotLight>(randomLoc, { 0.2f, 0.2f, 0.2f });
                    break;
                case 7:
                    spawnedActor = FObjectFactory::SpawnActor<ADirectionalLight>(randomLoc, { 0.2f, 0.2f, 0.2f });
                    break;

				default :
					break;
			}
            if (spawnedActor)
            {
                // 스폰된 액터 1칸 위에 UUID 라벨 흰색으로 표시
                ATextActor* label = FObjectFactory::SpawnActor<ATextActor>();
                label->SetScale(FVector(0.25f, 0.25f, 0.25f));
                label->SetTarget(spawnedActor);
                label->SetText(std::to_wstring(spawnedActor->GetID()));
            }
		}
			
	}

    bool bShowUUID = RENDERER.IsShowFlagEnabled(EEngineShowFlags::SF_BillboardText);

    if (ImGui::Checkbox("Show UUID", &bShowUUID))
    {
        RENDERER.SetShowFlag(EEngineShowFlags::SF_BillboardText, bShowUUID);
    }

    ImGui::End();
}

void UIPanel_SaveLoad::Render()
{
    ImGui::Begin("Picking Primitive Property", &bIsOpen, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.8f, 1.0f), "[ Save & Load Scene ]");
	
    // NEW SCENE
    if (ImGui::Button("New Scene"))
	{
        // Collider만 삭제 (Grid, World Axis Gizmo 삭제되지 않도록)
		OBJECT.DestroyAllColliders();
		PICK.pickedObjcect = nullptr;
		if (AGizmo::MainGizmo)
		{
			AGizmo::MainGizmo->SetTargetActor(nullptr);
		}
	}
	
	// SAVE
	if (ImGui::Button("Save Scene"))
	{
		// "./SceneData/MyScene.Scene" 으로 저장됨
		SAVELOAD.SaveScene("./SceneData/MyScene"); 
	}
	
	// LOAD
	if (ImGui::Button("Load Scene"))
	{
		// "./SceneData/MyScene.Scene" 에서 로드됨
		TArray<UObject*> loadedObj = SAVELOAD.LoadScene("./SceneData/MyScene.Scene");

	}
	
	ImGui::End();
}

void UIPanel_Picking::Render()
{
    ImGui::Begin("Picking Primitive Property", &bIsOpen, ImGuiWindowFlags_AlwaysAutoResize);
    
    // Picked Primitive Editor
    ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.0f), "[ Picking Controls ]");
    AActor* pickedActor = PICK.pickedObjcect;
    
    if (pickedActor)
    {
    	AGizmoAxis* gizmo = dynamic_cast<AGizmoAxis*>(pickedActor); //기즈모는 uid 없어서 해야됨
    	if (gizmo)
    	{
    		pickedActor = gizmo->GetTargetActor();
    	}
    
    	if (pickedActor)
    	{
    		string uid = std::to_string(pickedActor->GetID());
    		//string cid = string(pickedActor->GetClass()->Name);
    		// 디버그 정보 표시
    		ImGui::Text("UUID: %s", uid.c_str());
    		//ImGui::Text("ClassName: %s", cid.c_str());
    
    		// 선형 색상 편집
    		FLinearColor color = pickedActor->GetColor();
    		if (color.a <= 0.0f)
    		{
    			color = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
    		}
    
    		if (ImGui::ColorEdit4(("Linear Color##" + uid).c_str(), &color.r, ImGuiColorEditFlags_Float))
    		{
    			if (color.a <= 0.0f)
    			{
    				color.a = 1.0f;
    			}
    			pickedActor->SetColor(color);
    			if (pickedActor->GetMesh())
    			{
    				pickedActor->GetMesh()->SetColor(color);
    			}
    			if (PICK.pickedObjcect && !dynamic_cast<AGizmoAxis*>(PICK.pickedObjcect.Get()))
    			{
    				PICK.pickedObjcect->SetColor(color);
    			}
    		}
    
    		// 위치 편집
    		FVector loc = pickedActor->GetLocation();
    		if (ImGui::DragFloat3(("Pos##" + uid).c_str(), &loc.x, 0.01f, -10.0f, 10.0f))
    		{
    			pickedActor->SetLocation(loc);
    		}
    
    		// Scale Editor
    		FVector scale = pickedActor->GetScale();
    		if (ImGui::DragFloat3(("Scale##" + uid).c_str(), &scale.x, 0.01f, 0.01f, 5.0f))
    		{
    			pickedActor->SetScale(scale);
    		}
    
    		// 회전 편집 및 실시간 동기화
    		static float s_euler[3] = { 0.0f, 0.0f, 0.0f };
    		static string s_lastActorID;
    		static bool s_isEditingInImGui = false;
    
    		// 대상 변경 또는 위젯 미조작 시 기즈모 변환값 실시간 반영
    		if (uid != s_lastActorID || !s_isEditingInImGui)
    		{
    			FVector eulerRad = FQuaternion::ToEuler(pickedActor->GetRotation());
    			s_euler[0] = eulerRad.x * (180.0f / Global::PI);
    			s_euler[1] = eulerRad.y * (180.0f / Global::PI);
    			s_euler[2] = eulerRad.z * (180.0f / Global::PI);
    			s_lastActorID = uid;
    		}
    
    		bool bPrimChanged = false;
    		if (ImGui::DragFloat(("Rotation X##" + uid).c_str(), &s_euler[0], 1.0f, -180.0f, 180.0f))
    		{
    			bPrimChanged = true;
    		}
            
    		if (ImGui::DragFloat(("Rotation Y##" + uid).c_str(), &s_euler[1], 1.0f, -180.0f, 180.0f))
    		{
    			bPrimChanged = true;
    		}
    
    		if (ImGui::DragFloat(("Rotation Z##" + uid).c_str(), &s_euler[2], 1.0f, -180.0f, 180.0f))
    		{
    			bPrimChanged = true;
    		}
    
    		s_isEditingInImGui = bPrimChanged;
    
    		// 위젯 조작 시에만 각도를 라디안으로 변환하여 적용
    		if (bPrimChanged)
    		{
    			FQuaternion newRot = FQuaternion::FromEuler(
    				s_euler[0] * (Global::PI / 180.0f),
    				s_euler[1] * (Global::PI / 180.0f),
    				s_euler[2] * (Global::PI / 180.0f)
    			);
    			pickedActor->SetRotation(newRot);
    		}

            // Light 관련 코드라면
            if (pickedActor->GetClass()->Name == "ASpotLight") {
                ASpotLight *light = Cast<ASpotLight>(pickedActor);
                float angle = light->GetAngle();
                if (ImGui::SliderFloat("Angle", &angle, 1.0f, 60.0f))
                    light->SetAngle(angle);
                float length = light->GetLength();
                if (ImGui::SliderFloat("Length", &length, 0.1f, 30.0f))
                    light->SetLength(length);
            }

            if (pickedActor->GetClass()->Name == "APointLight") {
                APointLight* light = Cast<APointLight>(pickedActor);
                float radius = light->GetRadius();
                if (ImGui::SliderFloat("Radius", &radius, 1.0f, 40.0f))
                    light->SetRadius(radius);
            }

            // 삭제버튼
            if (ImGui::Button("Delete"))
            {
                pickedActor->Destroy();

                PICK.pickedObjcect = nullptr;
                if (AGizmo::MainGizmo)
                {
                    AGizmo::MainGizmo->SetTargetActor(nullptr);
                }
            }
    	}
    }
    
    ImGui::End();
}
	
void UIPanel_FPS::Render()
{
    
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);

    ImGui::Begin("Engine Main Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("DirectX 11 & ImGui Active");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
}

void UIPanel_Grid::Render()
{
    ImGui::Begin("Grid", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    Scene* scene = SCENE.GetCurrentScene();
    DefaultScene* defaultScene = dynamic_cast<DefaultScene*>(scene);

    float cellSize = defaultScene->Ugrid.GetCellSize();
    if (ImGui::SliderFloat("Grid Interval", &cellSize, 0.15f, 2.0f))
    {
        defaultScene->Ugrid.SetCellSize(cellSize);
        // ----------------------------
        // editor.ini 저장 추가!!
        // ----------------------------
    }
    ImGui::End();
}
void UIPanel_SceneManager::Render()
{
    
    ImGui::Begin("Scene Manager");
    if (ImGui::TreeNode("Primitives"))
    {
        int32 Selected = -1;
        int32 i = 0;
        bool On = false;
        for (UObject* Object : OBJECT.GUObjectArray)
        {
            AActor* Actor = Cast<AActor, UObject>(Object);
            if (Actor->Primitive != EPrimitive::None && Actor->Primitive != EPrimitive::Gizmo)
            {
                if (ImGui::Selectable(Object->GetName().c_str(), Selected == i))
                {
                    if (!On)
                    {
                        Selected = i;
                        PICK.pickedObjcect = Cast<AActor, UObject>(Object);
                        AGizmo::MainGizmo->SetTargetActor(PICK.pickedObjcect);
                        On = true;
                    }
                    else
                    {
                        Selected = -1;
                        PICK.pickedObjcect = nullptr;
                        AGizmo::MainGizmo->SetTargetActor(nullptr);
                        On = false;
                    }
                }
                i++;
            }
        }
        ImGui::TreePop();
    }
    ImGui::End();
}
