#pragma once

#include "Containers.h"
#include "IUIPanel.h"

#include <string>

// 디버그 정보
class UIPanel_Debug : public IUIPanel
{
public:
    void Render() override;

	FString GetName() const override { return "Engine Debug Info"; }
};

// 씬 카메라 창
class UIPanel_SceneCamera : public IUIPanel
{
public:
    void Render() override;

	FString GetName() const override { return "Scene & Camera"; }

private:
    FString OpenSceneFileDialog();
    FString SaveSceneFileDialog();
};

// 스폰 창
class UIPanel_Spawn : public IUIPanel
{
public:
    void Render() override;

	FString GetName() const override { return "Spawn Primitives"; }
};

// 씬 매니저 창 
class UIPanel_SceneManager : public IUIPanel
{
public:
    void Render() override;

	FString GetName() const override { return "Scene Manager"; }
};

// picking actor 속성창
class UIPanel_Picking : public IUIPanel
{
public:
    void Render() override;

	FString GetName() const override { return "Inspector"; }

private:
    float s_euler[3] = { 0.0f, 0.0f, 0.0f };    // euler 회전각 x, y, z
    string s_lastActorID;                       // 마지막으로 선택된 actor ID
    bool s_idEditing = false;                   // Imgui에서 수정되었는지?
};