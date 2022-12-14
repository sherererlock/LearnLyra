// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "LyraPlayerBotController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class LEARNLYRA_API ALyraPlayerBotController : public AModularAIController
{
	GENERATED_BODY()
public:
	ALyraPlayerBotController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ILyraTeamAgentInterface interface
	//virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	//virtual FGenericTeamId GetGenericTeamId() const override;
	//virtual FOnLyraTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~End of ILyraTeamAgentInterface interface

	// Attempts to restart this controller (e.g., to respawn it)
	void ServerRestartController();

	////Update Team Attitude for the AI
	//UFUNCTION(BlueprintCallable, Category = "Lyra AI Player Controller")
	//void UpdateTeamAttitude(UAIPerceptionComponent* AIPerception);

	virtual void OnUnPossess() override;


private:
	//UFUNCTION()
	//void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

protected:
	// Called when the player state is set or cleared
	virtual void OnPlayerStateChanged();

private:
	void BroadcastOnPlayerStateChanged();

protected:
	//~AController interface
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	//~End of AController interface

private:
	//UPROPERTY()
	//FOnLyraTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	APlayerState* LastSeenPlayerState;
};
