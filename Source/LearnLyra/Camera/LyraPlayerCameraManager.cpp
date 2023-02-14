// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/LyraPlayerCameraManager.h"

ALyraPlayerCameraManager::ALyraPlayerCameraManager(FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	DefaultFOV = LYRA_CAMERA_DEFAULT_FOV;
	ViewPitchMin = LYRA_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = LYRA_CAMERA_DEFAULT_PITCH_MAX;
}

void ALyraPlayerCameraManager::UpdateViewTarget(FTViewTarget& outVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);
}
