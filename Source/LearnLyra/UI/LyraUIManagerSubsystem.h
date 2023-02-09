// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "LyraUIManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class LEARNLYRA_API ULyraUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()
	
public:
	ULyraUIManagerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	bool Tick(float DeltaTime);
	void SyncRootLayoutVisibilityToShowHUD();

	FTSTicker::FDelegateHandle TickHandle;
};
