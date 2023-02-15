// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "LyraPlayerCameraManager.generated.h"

#define LYRA_CAMERA_DEFAULT_FOV			(80.0f)
#define LYRA_CAMERA_DEFAULT_PITCH_MIN	(-89.0f)
#define LYRA_CAMERA_DEFAULT_PITCH_MAX	(89.0f)

/**
 * 
 */
UCLASS()
class LEARNLYRA_API ALyraPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	ALyraPlayerCameraManager(const FObjectInitializer& ObjectInitialier);

protected:
	virtual void UpdateViewTarget(FTViewTarget& outVT, float DeltaTime) override;

};
