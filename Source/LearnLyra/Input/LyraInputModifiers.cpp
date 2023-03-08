// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/LyraInputModifiers.h"
#include "Player/LyraLocalPlayer.h"
#include "EnhancedPlayerInput.h"
#include "GameFramework/PlayerController.h"

FInputActionValue ULyraInputModifiers::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayInput, FInputActionValue CurrentValue, float DeltaTime)
{
	if (ensureMsgf(CurrentValue.GetValueType() != EInputActionValueType::Boolean, TEXT("Setting Based Scalar modifier doesn't support boolean Values")))
	{
		FVector ScalarToUse = FVector(1.0, 1.0, 1.0);
		ScalarToUse.X = FMath::Clamp(ScalarToUse.X, MinValueClamp.X, MaxValueClamp.X);
		ScalarToUse.X = FMath::Clamp(ScalarToUse.X, MinValueClamp.X, MaxValueClamp.X);
		ScalarToUse.X = FMath::Clamp(ScalarToUse.X, MinValueClamp.X, MaxValueClamp.X);

		return CurrentValue.Get<FVector>() * ScalarToUse;
	}

	return CurrentValue;
}
