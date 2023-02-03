// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ULyraActivatableWidget.h"
#include "Blueprint/UserWidget.h"
#include "CommonActivatableWidget.h"

#include "LyraHUDLayout.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "Lyra HUD Layout", Category = "Lyra|HUD"))
class ULyraHUDLayout : public ULyraActivatableWidget
{
	GENERATED_BODY()
public:

	ULyraHUDLayout(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

protected:
	void HandleEscapeAction();

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;
};
