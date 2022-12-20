// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraPlayerState.h"
#include "AbiltiyStstem/LyraAbilitySystemComponent.h"

ALyraPlayerState::ALyraPlayerState(const FObjectInitializer& ObjectInitilize)
	: Super(ObjectInitilize)
{
	AbilitySystemComponent = ObjectInitilize.CreateDefaultSubobject<ULyraAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
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

void ALyraPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

UAbilitySystemComponent* ALyraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
