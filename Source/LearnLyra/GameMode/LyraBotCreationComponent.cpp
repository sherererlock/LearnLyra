// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LyraBotCreationComponent.h"
#include "LyraGameMode.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "GameMode/LyraExperienceDefinition.h"
#include "GameMode/LyraExperienceManagerComponent.h"
//#include "Development/LyraDeveloperSettings.h"
#include "Player/LyraPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Components/LyraPawnExtensionComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/LyraHealthComponent.h"

ULyraBotCreationComponent::ULyraBotCreationComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void ULyraBotCreationComponent::BeginPlay()
{
    Super::BeginPlay();

    AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
    ULyraExperienceManagerComponent* ManagerComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
    check(ManagerComponent);

    ManagerComponent->CallOrRegister_OnExperienceLoaded_LowPriority(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ULyraBotCreationComponent::OnExperienceLoaded(const ULyraExperienceDefinition* Definition)
{
#if WITH_SERVER_CODE
    if (HasAuthority())
    {
        ServerCreateBots();
    }
#endif
}

#if WITH_SERVER_CODE

void ULyraBotCreationComponent::ServerCreateBots()
{
    if (BotControllerClass == nullptr)
        return;

    int32 EffectiveBotCount = NumBotsToCreate;
   
    for (int32 i = 0; i < EffectiveBotCount; i++)
    {
        SpawnOneBot();
    }
}

void ULyraBotCreationComponent::SpawnOneBot()
{
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnInfo.OverrideLevel = GetComponentLevel();
    SpawnInfo.ObjectFlags |= RF_Transient;

    AAIController* NewController = GetWorld()->SpawnActor<AAIController>(BotControllerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
    if (NewController != nullptr)
    {
        ALyraGameMode* GameMode = GetGameMode<ALyraGameMode>();
        check(GameMode);
        if (NewController->PlayerState != nullptr)
        {
            const int32 NameIndex = FMath::RandRange(0, RandomBotNames.Num() - 1);
            NewController->PlayerState->SetPlayerName(RandomBotNames[NameIndex]);
        }

        GameMode->DispatchPostLogin(NewController);
        GameMode->RestartPlayer(NewController);

        if (NewController->GetPawn() != nullptr)
        {
            if (ULyraPawnExtensionComponent* PawnExtComponent = NewController->GetPawn()->FindComponentByClass<ULyraPawnExtensionComponent>())
            {
                PawnExtComponent->CheckPawnReadyToInitialize();
            }
        }

        SpawnBotList.Add(NewController);
    }
}

void ULyraBotCreationComponent::RemoveOneBot()
{
    if (SpawnBotList.Num() > 0)
    {
        const int32 Index = FMath::RandRange(0, SpawnBotList.Num());
        AAIController* BotToRemove = SpawnBotList[Index];
        SpawnBotList.RemoveAtSwap(Index);

        if (BotToRemove)
        {
            if (APawn* Pawn = BotToRemove->GetPawn())
            {
                if (ULyraHealthComponent* Component = Pawn->FindComponentByClass<ULyraHealthComponent>())
                {
                    Component->DamageSelfDestruct();
                }
                else
                {
                    Pawn->Destroy();
                }
            }

            BotToRemove->Destroy();
        }
    }
}

FString ULyraBotCreationComponent::CreateBotName(int32 Index)
{
    return FString();
}

#endif