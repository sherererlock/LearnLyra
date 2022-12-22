// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LyraGameMode.generated.h"

class ULyraExperienceDefinition;
/**
 * 
 */
UCLASS()
class LEARNLYRA_API ALyraGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ALyraGameMode(const FObjectInitializer& ObjectInitialize = FObjectInitializer::Get());

	const ULyraPawnData* GetPawnDataForController(const AController* InController) const;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrotMessage) override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;

	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;
	virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
	virtual void InitGameState() override;

protected:
	void OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience);
	bool IsExperienceLoaded() const;

	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource);

	void HandleMatchAssignmentIfNotExpectingOne();
};
