#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

SceneManager& SceneManager::GetInstance()
{
	static SceneManager instance;
	return instance;
}

void SceneManager::AddScene(const FString& name, Scene* scene)
{
	if (m_scenes.Contains(name))
		return;

	m_scenes[name] = scene;
}

void SceneManager::RequestChangeScene(const FString& name)
{
	if (m_hasPendingSceneChange) return;
	m_hasPendingSceneChange = true;
	m_pendingSceneName = name;
}

void  SceneManager::ChangeScene(const FString& name) 
{
	m_currentScene = m_scenes[name];
	m_currentSceneName = name;
}

void SceneManager::Update(float deltaTime)
{
	if (m_currentScene)
	{
		m_currentScene->Update(deltaTime);
	}
}

void SceneManager::Render()
{
	if (m_currentScene) m_currentScene->Render();
}

void SceneManager::Release()
{
	m_currentScene = nullptr;
	// 모든 씬 해제
	for (auto& pair : m_scenes)
	{
		delete pair.second;
	}
	m_scenes.clear();
}
