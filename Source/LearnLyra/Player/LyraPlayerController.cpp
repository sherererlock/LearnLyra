// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LyraPlayerController.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "Player/LyraPlayerState.h"

ALyraPlayerController::ALyraPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// PlayerCameraManagerClass = ALyraPlayerCameraManager::StaticClass();
}

void ALyraPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ALyraPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ALyraPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ALyraPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
}

void ALyraPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ALyraPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ALyraPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
}

void ALyraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

ALyraPlayerState* ALyraPlayerController::GetLyraPlayerState() const
{
	return CastChecked<ALyraPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

ULyraAbilitySystemComponent* ALyraPlayerController::GetLyraAbilitySystemComponent() const
{
	const ALyraPlayerState* LyraPS = GetLyraPlayerState();
	return (LyraPS ? LyraPS->GetLyraAbilitySystemComponent() : nullptr);
}

void ALyraPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
}

void ALyraPlayerController::CleanupPlayerState()
{
	Super::CleanupPlayerState();
}

void ALyraPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void ALyraPlayerController::SetPlayer(UPlayer* InPlayer)
{
	Super::SetPlayer(InPlayer);
}

void ALyraPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void ALyraPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (ULyraAbilitySystemComponent* LyraASC = GetLyraAbilitySystemComponent())
	{
		LyraASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}
