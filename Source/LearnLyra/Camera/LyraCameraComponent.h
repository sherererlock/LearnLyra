// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "LyraCameraComponent.generated.h"

class ULyraCameraMode;
class ULyraCameraModeStack;
class UCanvas;
struct FGameplayTag;

DECLARE_DELEGATE_RetVal(TSubclassOf<ULyraCameraMode>, FLyraCameraModeDelegate)

/**
 * 
 */
UCLASS()
class LEARNLYRA_API ULyraCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	ULyraCameraComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "Lyra|Camera")
	static ULyraCameraComponent* FindCameraComponent(const AActor* Actor) { return Actor->FindComponentByClass<ULyraCameraComponent>(); }
	
	virtual AActor* GetTargetActor() const { return GetOwner(); }

	FLyraCameraModeDelegate DetermineCameraModeDelegate;

	void AddFieldOfViewOffset(float FovOffset) { FieldOfViewOffset += FovOffset; }
	
	void GetBlendInfo(float& OutWeightofTopLayer, FGameplayTag& OutTagOfTopLayer) const;

protected:

	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;
	virtual void UpdateCameraMode();

protected:

	UPROPERTY()
	ULyraCameraModeStack* CameraModeStack;

	float FieldOfViewOffset;
};
