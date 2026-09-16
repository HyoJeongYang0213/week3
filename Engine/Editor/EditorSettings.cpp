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
	cam.SetFOVX(ReadFloat("Camera", "FOV", "60.0"));
	cam.SetSpeed(ReadFloat("Camera", "MoveSpeed", "2.0"));
	cam.SetRotationSpeed(ReadFloat("Camera", "MouseSensitivity", "0.2"));
	cam.SetWheelSpeed(ReadFloat("Camera", "WheelSpeed", "1.0"));

	FVector location(
		ReadFloat("Camera", "PositionX", std::to_string(CAMERA.DefaultLocation.X).c_str()),
		ReadFloat("Camera", "PositionY", std::to_string(CAMERA.DefaultLocation.Y).c_str()),
		ReadFloat("Camera", "PositionZ", std::to_string(CAMERA.DefaultLocation.Z).c_str())
	);
	cam.Location = location;

	cam.Yaw = ReadFloat("Camera", "Yaw", std::to_string(CAMERA.DefaultYaw).c_str());
	cam.Pitch = ReadFloat("Camera", "Pitch", std::to_string(CAMERA.DefaultPitch).c_str());

	cam.SetProjectionMode(EProjectionMode(ReadInt("Camera", "ProjectionMode", "0")));
	cam.ViewMode = static_cast<EViewMode>(ReadInt("Camera", "ViewMode", "0"));
	cam.SetOrthoWidth(ReadFloat("Camera", "OrthoWidth", "25.0"));

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
	WriteFloat("Camera", "FOV", cam.GetFOVX());
	WriteFloat("Camera", "MoveSpeed", cam.GetSpeed());
	WriteFloat("Camera", "MouseSensitivity", cam.GetRotationSpeed());
	WriteFloat("Camera", "WheelSpeed", cam.GetWheelSpeed());

	FVector location = cam.Location;
	WriteFloat("Camera", "PositionX", location.X);
	WriteFloat("Camera", "PositionY", location.Y);
	WriteFloat("Camera", "PositionZ", location.Z);

	WriteFloat("Camera", "Yaw", cam.Yaw);
	WriteFloat("Camera", "Pitch", cam.Pitch);

	WriteInt("Camera", "ProjectionMode", int32(cam.GetProjectionMode()));
	WriteInt("Camera", "ViewMode", static_cast<int>(cam.ViewMode));
	WriteFloat("Camera", "OrthoWidth", cam.GetOrthoWidth());

	// Grid
	Scene* defaultScene = SCENE.GetCurrentScene();
	if (defaultScene)
	{
		WriteFloat("Grid", "Interval", defaultScene->GetGrid().CellSize);
	}
}