// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LyraGameMode.h"
#include "GameMode/LyraGameState.h"
#include "Player/LyraPlayerController.h"
#include "GameMode/LyraPlayerBotController.h"
#include "Character/LyraCharacter.h"
#include "Character/LyraPawnData.h"
#include "Player/LyraPlayerState.h"
#include "LyraWorldSettings.h"
#include "LyraExperienceManagerComponent.h"
#include "Components/LyraPawnExtensionComponent.h"
#include "LyraExperienceDefinition.h"
#include "System/LyraAssetManager.h"


ALyraGameMode::ALyraGameMode(const FObjectInitializer& ObjectInitialize)
	: Super(ObjectInitialize)
{
	GameStateClass = ALyraGameState::StaticClass();
	PlayerControllerClass = ALyraPlayerController::StaticClass();
	DefaultPawnClass = ALyraCharacter::StaticClass();
	PlayerStateClass = ALyraPlayerState::StaticClass();
}

const ULyraPawnData* ALyraGameMode::GetPawnDataForController(const AController* InController) const
{
	if (InController != nullptr)
	{
		if (const ALyraPlayerState* PS = InController->GetPlayerState<ALyraPlayerState>())
		{
			if (const ULyraPawnData* PawnData = PS->GetPawnData<ULyraPawnData>())
			{
				return PawnData;
			}
		}
	}

	check(GameState);
	ULyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceComponent);

	if (ExperienceComponent->IsExperienceLoaded())
	{
		const ULyraExperienceDefinition* Experience = ExperienceComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData != nullptr)
		{
			return Experience->DefaultPawnData;
		}

		//// Experience is loaded and there's still no pawn data, fall back to the default for now
		//return ULyraAssetManager::Get().GetDefaultPawnData();
	}

	return nullptr;
}

void ALyraGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrotMessage)
{
	Super::InitGame(MapName, Options, ErrotMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

UClass* ALyraGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const ULyraPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* ALyraGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	const ULyraPawnData* PawnData = GetPawnDataForController(NewPlayer);
	if (!PawnData)
		return nullptr;

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	if (UClass* Class = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* Pawn = GetWorld()->SpawnActor<APawn>(Class, SpawnTransform, SpawnInfo))
		{
			if (ULyraPawnExtensionComponent* PawnExt = Pawn->FindComponentByClass<ULyraPawnExtensionComponent>())
			{
				PawnExt->SetPawnData(PawnData);
			}

			Pawn->FinishSpawning(SpawnTransform);

			return Pawn;
		}
	}

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

	ULyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceComponent);
	ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ALyraGameMode::RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset)
{
	if (bForceReset && (Controller != nullptr))
	{
		Controller->Reset();
	}

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(PC, &APlayerController::ServerRestartPlayer_Implementation);
	}
	else if (ALyraPlayerBotController* BotController = Cast<ALyraPlayerBotController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(BotController, &ALyraPlayerBotController::ServerRestartController);
	}
}

void ALyraGameMode::OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

bool ALyraGameMode::IsExperienceLoaded() const
{
	check(GameState);
	ULyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceComponent);

	return ExperienceComponent->IsExperienceLoaded();
}

void ALyraGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource)
{
#if WITH_SERVER_CODE
	if (ExperienceId.IsValid())
	{
		ULyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->ServerSetCurrentExperience(ExperienceId);
	}
#endif
}

void ALyraGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;
	FString ExperienceIdSource;

	UWorld* World = GetWorld();

	// see if the world settings has a default experience
	if (!ExperienceId.IsValid())
	{
		if (ALyraWorldSettings* TypedWorldSettings = Cast<ALyraWorldSettings>(GetWorldSettings()))
		{
			ExperienceId = TypedWorldSettings->GetDefaultGameplayExperience();
			ExperienceIdSource = TEXT("WorldSettings");
		}
	}

	OnMatchAssignmentGiven(ExperienceId, ExperienceIdSource);
}
