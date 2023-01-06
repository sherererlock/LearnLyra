// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LyraPlayerBotController.h"
#include "LyraGameMode.h"
#include "Engine/World.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"

ALyraPlayerBotController::ALyraPlayerBotController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsPlayerState = true;
	bStopAILogicOnUnposses = false;
}

void ALyraPlayerBotController::ServerRestartController()
{
	if (GetNetMode() == NM_Client)
	{
		return;
	}

	ensure((GetPawn() == nullptr) && IsInState(NAME_Inactive));

	if (IsInState(NAME_Inactive) || (IsInState(NAME_Spectating)))
	{
		ALyraGameMode* const GameMode = GetWorld()->GetAuthGameMode<ALyraGameMode>();

		if ((GameMode == nullptr))
		{
			return;
		}

		// If we're still attached to a Pawn, leave it
		if (GetPawn() != nullptr)
		{
			UnPossess();
		}

		// Re-enable input, similar to code in ClientRestart
		ResetIgnoreInputFlags();

		GameMode->RestartPlayer(this);
	}
}

void ALyraPlayerBotController::OnUnPossess()
{
	// Make sure the pawn that is being unpossessed doesn't remain our ASC's avatar actor
	if (APawn* PawnBeingUnpossessed = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerState))
		{
			if (ASC->GetAvatarActor() == PawnBeingUnpossessed)
			{
				ASC->SetAvatarActor(nullptr);
			}
		}
	}

	Super::OnUnPossess();
}

void ALyraPlayerBotController::OnPlayerStateChanged()
{
}

void ALyraPlayerBotController::BroadcastOnPlayerStateChanged()
{
	OnPlayerStateChanged();
}

void ALyraPlayerBotController::InitPlayerState()
{
	Super::InitPlayerState();
	BroadcastOnPlayerStateChanged();
}

void ALyraPlayerBotController::CleanupPlayerState()
{
	Super::CleanupPlayerState();
	BroadcastOnPlayerStateChanged();
}

void ALyraPlayerBotController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	BroadcastOnPlayerStateChanged();
}
