// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "LyraGameSession.generated.h"

/**
 * 
 */
UCLASS()
class LEARNLYRA_API ALyraGameSession : public AGameSession
{
	GENERATED_BODY()

public:

	ALyraGameSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
};
