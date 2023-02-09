// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "CommonPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "LyraPlayerController.generated.h"

class ULyraAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class LEARNLYRA_API ALyraPlayerController : public ACommonPlayerController
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

	UFUNCTION(BlueprintCallable, Category = "Lyra|PlayerController")
	ALyraPlayerState* GetLyraPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "Lyra|PlayerController")
	ULyraAbilitySystemComponent* GetLyraAbilitySystemComponent() const;

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
