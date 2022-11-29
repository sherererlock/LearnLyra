// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LyraPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class LEARNLYRA_API ALyraPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ALyraPlayerState(const FObjectInitializer& ObjectInitilize = FObjectInitializer::Get());
	// Actor
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	// Actor

	// APlayerState
	virtual void Reset() override;
	virtual void ClientInitialize(AController* C) override;
	virtual void OnDeactivated() override;
	virtual void OnReactivated() override;


protected:
	// APlayerState
	virtual void CopyProperties(APlayerState* PlayerState);
	// APlayerState

};
