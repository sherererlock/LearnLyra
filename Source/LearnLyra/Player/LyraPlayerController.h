// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LyraPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LEARNLYRA_API ALyraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALyraPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Actor
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type  EndPlayReason) override;
	// Actor

	// AController
	virtual void SetPawn(class APawn* InPawn) override;
	virtual void OnPossess(class APawn* InPawn) override;
	virtual void OnUnPossess() override;
	// AController

	// APlayerController
	virtual void ReceivedPlayer() override;
	virtual void PlayerTick(float DeltaTime) override;
	// APlayerController

protected:
	// AController
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	// AController

	// APlayerController
	virtual void SetPlayer(UPlayer* InPlayer) override;

	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	// APlayerController
};