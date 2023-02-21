// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraPlayerState.h"
#include "AbiltiyStstem/LyraAbilitySystemComponent.h"
#include "GameMode/LyraExperienceManagerComponent.h"
#include "GameMode/LyraExperienceDefinition.h"
#include "Character/LyraPawnData.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/Abilities/LyraAbilitySet.h"
#include "AbilitySystem/Attribute/LyraHealthSet.h"

ALyraPlayerState::ALyraPlayerState(const FObjectInitializer& ObjectInitilize)
	: Super(ObjectInitilize)
{
	AbilitySystemComponent = ObjectInitilize.CreateDefaultSubobject<ULyraAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	CreateDefaultSubobject<ULyraHealthSet>(TEXT("HealthSet"));

}

void ALyraPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ALyraPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	if (GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);

		ULyraExperienceManagerComponent* Com = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
		check(Com);

		Com->CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
}

void ALyraPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ALyraPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ALyraPlayerState::Reset()
{
	Super::Reset();
}

void ALyraPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);
}

void ALyraPlayerState::OnDeactivated()
{
	Super::OnDeactivated();
}

void ALyraPlayerState::OnReactivated()
{
	Super::OnReactivated();
}

void ALyraPlayerState::SetPawnData(const ULyraPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	PawnData = InPawnData;
	for(ULyraAbilitySet* AbilitySet : PawnData->AbilitySet )
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}

void ALyraPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

UAbilitySystemComponent* ALyraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ALyraPlayerState::OnRep_PawnData()
{
}

void ALyraPlayerState::OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience)
{
	if (CurrentExperience)
	{
		SetPawnData(CurrentExperience->DefaultPawnData);
	}
}

void ALyraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
}
