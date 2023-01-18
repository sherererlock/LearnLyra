// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LyraHUD.generated.h" 

/**
 * 
 */
UCLASS(Config = Game)
class LEARNLYRA_API ALyraHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ALyraHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// UObject
	virtual void PreInitializeComponents() override;

	// UActor
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// AHUD
	virtual void GetDebugActorList(TArray<AActor*>& InOutList) override;

};
