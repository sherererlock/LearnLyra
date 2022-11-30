// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LyraGameMode.h"
#include "GameMode/LyraGameState.h"
#include "Player/LyraPlayerController.h"
#include "Character/LyraCharacter.h"
#include "LyraPlayerState.h"

ALyraGameMode::ALyraGameMode(const FObjectInitializer& ObjectInitialize)
	: Super(ObjectInitialize)
{
	GameStateClass = ALyraGameState::StaticClass();
	PlayerControllerClass = ALyraPlayerController::StaticClass();
	DefaultPawnClass = ALyraCharacter::StaticClass();
	PlayerStateClass = ALyraPlayerState::StaticClass();
}

void ALyraGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrotMessage)
{
	Super::InitGame(MapName, Options, ErrotMessage);

	//GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMat);
}

UClass* ALyraGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* ALyraGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

bool ALyraGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	return true;
}

void ALyraGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

AActor* ALyraGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ALyraGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	Super::FinishRestartPlayer(NewPlayer, StartRotation);
}

bool ALyraGameMode::PlayerCanRestart_Implementation(APlayerController* Player)
{
	return true;
}

void ALyraGameMode::InitGameState()
{
	Super::InitGameState();
}
