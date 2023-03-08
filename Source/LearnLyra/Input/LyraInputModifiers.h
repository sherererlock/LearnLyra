// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "LyraInputModifiers.generated.h"

class UEnhancedPlayerInput;

/**
 * 
 */
UCLASS(NotBlueprintable, MinimalAPI, meta = (DisaplayName = "Setting Based Scalar"))
class ULyraInputModifiers : public UInputModifier
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Setting)
	FVector MaxValueClamp = FVector(10, 10, 10);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Setting)
	FVector MinValueClamp = FVector::ZeroVector;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Setting)
	float TargetingMultiplier = 0.5f;

protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayInput, FInputActionValue CurrentValue, float DeltaTime) override;
};
