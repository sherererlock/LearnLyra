// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonLocalPlayer.h"
#include "Engine/LocalPlayer.h"
#include "LyraLocalPlayer.generated.h"

/**
 * 
 */
UCLASS()
class LEARNLYRA_API ULyraLocalPlayer : public UCommonLocalPlayer
{
	GENERATED_BODY()
	
public:

	ULyraLocalPlayer();

	// UObject
	virtual void PostInitProperties() override;
	// UObject

	// UPlayer
	virtual void SwitchController(class APlayerController* PC) override;

	// ULocalPlayer
	virtual bool SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld) override;
	virtual void InitOnlineSession() override;

};
