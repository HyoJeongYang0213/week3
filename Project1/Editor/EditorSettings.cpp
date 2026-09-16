#include "pch.h"
#include "EditorSettings.h"

const char* EditorIniPath = "./editor.ini";

static float ReadFloat(const char* Section, const char* Key, const char* DefaultValue)
{
	char buffer[64];
	GetPrivateProfileStringA(Section, Key, DefaultValue, buffer, sizeof(buffer), EditorIniPath);
	return std::stof(buffer);
}

static int ReadInt(const char* Section, const char* Key, const char* DefaultValue)
{
	char buffer[64];
	GetPrivateProfileStringA(Section, Key, DefaultValue, buffer, sizeof(buffer), EditorIniPath);
	return std::stoi(buffer);
}

static void WriteFloat(const char* Section, const char* Key, float Value)
{
	std::string value = std::to_string(Value);
	WritePrivateProfileStringA(Section, Key, value.c_str(), EditorIniPath);
}

static void WriteInt(const char* Section, const char* Key, int Value)
{
	std::string value = std::to_string(Value);
	WritePrivateProfileStringA(Section, Key, value.c_str(), EditorIniPath);
}


void EditorSettings::Load()
{
	Camera& cam = CAMERA;

	// Camera
	cam.SetFOV(ReadFloat("Camera", "FOV", "60.0"));
	cam.SetSpeed(ReadFloat("Camera", "MoveSpeed", "2.0"));
	cam.SetRotationSpeed(ReadFloat("Camera", "MouseSensitivity", "0.2"));
	cam.SetWheelSpeed(ReadFloat("Camera", "WheelSpeed", "1.0"));

	FVector location(
		ReadFloat("Camera", "PositionX", "3.336"),
		ReadFloat("Camera", "PositionY", "3.282"),
		ReadFloat("Camera", "PositionZ", "-4.715")
	);
	cam.SetLocation(location);

	FVector rotation(
		ReadFloat("Camera", "Pitch", "0.391"),
		ReadFloat("Camera", "Yaw", "-0.468"),
		ReadFloat("Camera", "Roll", "0.0")
	);
	cam.SetRotation(FQuaternion::FromEuler(rotation.x, rotation.y, rotation.z));

	cam.SetProjectionMode(ReadInt("Camera", "ProjectionMode", "0"));
	cam.ViewMode = static_cast<EViewMode>(ReadInt("Camera", "ViewMode", "0"));

	// Grid
	Scene* defaultScene = SCENE.GetCurrentScene();
	if (defaultScene)
	{
		defaultScene->GetGrid().CellSize = ReadFloat("Grid", "Interval", "1.0");
	}
}


void EditorSettings::Save()
{
	Camera& cam = CAMERA;

	// Camera
	WriteFloat("Camera", "FOV", cam.GetFOV());
	WriteFloat("Camera", "MoveSpeed", cam.GetSpeed());
	WriteFloat("Camera", "MouseSensitivity", cam.GetRotationSpeed());
	WriteFloat("Camera", "WheelSpeed", cam.GetWheelSpeed());

	FVector location = cam.GetLocation();
	WriteFloat("Camera", "PositionX", location.x);
	WriteFloat("Camera", "PositionY", location.y);
	WriteFloat("Camera", "PositionZ", location.z);

	FVector rotation = FQuaternion::ToEuler(cam.GetRotation());
	WriteFloat("Camera", "Pitch", rotation.x);
	WriteFloat("Camera", "Yaw", rotation.y);
	WriteFloat("Camera", "Roll", rotation.z);

	WriteInt("Camera", "ProjectionMode", cam.GetProjectionMode());
	WriteInt("Camera", "ViewMode", static_cast<int>(cam.ViewMode));

	// Grid
	Scene* defaultScene = SCENE.GetCurrentScene();
	if (defaultScene)
	{
		WriteFloat("Grid", "Interval", defaultScene->GetGrid().CellSize);
	}
}