// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameState.h"

#include "GameFramework/GameStateBase.h"

#include "LyraGameState.generated.h"

class ULyraExperienceManagerComponent;
/**
 * 
 */
UCLASS(Config = Game)
class LEARNLYRA_API ALyraGameState : public AModularGameStateBase
{
	GENERATED_BODY()
	
public:

	ALyraGameState(const FObjectInitializer& ObjectInitialize = FObjectInitializer::Get());

	// ~AActor Interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~End of AActor interface

	// ~AGameStateBase interface
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	// ~End of AGameStateBase interface

protected:
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY()
	ULyraExperienceManagerComponent* ExperienceManagerComponent;
};
