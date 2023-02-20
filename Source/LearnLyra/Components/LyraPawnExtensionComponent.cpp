// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraPawnExtensionComponent.h"

ULyraPawnExtensionComponent::ULyraPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	PawnData = nullptr;
	bPawnReadyToInitialize = false;
	PawnData = DefaultPawnData;
}

void ULyraPawnExtensionComponent::HandleControllerChanged()
{
	CheckPawnReadyToInitialize();
}

void ULyraPawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckPawnReadyToInitialize();
}

bool ULyraPawnExtensionComponent::CheckPawnReadyToInitialize()
{
	if (bPawnReadyToInitialize)
	{
		return true;
	}

	//PawnData = DefaultPawnData;
	// Pawn data is required.
	if (!PawnData)
	{
		return false;
	}

	APawn* Pawn = GetPawnChecked<APawn>();

	const bool bHasAuthority = Pawn->HasAuthority();
	const bool bIsLocallyControlled = Pawn->IsLocallyControlled();

	if (bHasAuthority || bIsLocallyControlled)
	{
		// Check for being possessed by a controller.
		if (!GetController<AController>())
		{
			return false;
		}
	}

	// Allow pawn components to have requirements.
	TArray<UActorComponent*> InteractableComponents = Pawn->GetComponentsByInterface(ULyraReadyInterface::StaticClass());
	for (UActorComponent* InteractableComponent : InteractableComponents)
	{
		const ILyraReadyInterface* Ready = CastChecked<ILyraReadyInterface>(InteractableComponent);
		if (!Ready->IsPawnComponentReadyToInitialize())
		{
			return false;
		}
	}

	bPawnReadyToInitialize = true;
	OnPawnReadyToInitialize.Broadcast();

	return false;
}

void ULyraPawnExtensionComponent::InitAbilitySystemComponent(ULyraAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	check(InASC);
	check(InOwnerActor);
	if (InASC == AbilitySystemComponent)
		return;

	if (AbilitySystemComponent)
	{
		UninitializeAbilitySystem();
	}

	AbilitySystemComponent = InASC;
	APawn* Pawn = GetPawnChecked<APawn>();
	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);

	OnAbilitySystemInitialize.Broadcast();
}

void ULyraPawnExtensionComponent::UninitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		AbilitySystemComponent->CancelAbilities(nullptr, nullptr);
		AbilitySystemComponent->ClearAbilityInput();
		AbilitySystemComponent->RemoveAllGameplayCues();

		if (AbilitySystemComponent->GetOwnerActor() != nullptr)
		{
			AbilitySystemComponent->SetAvatarActor(nullptr);
		}
		else
		{
			// If the ASC doesn't have a valid owner, we need to clear *all* actor info, not just the avatar pairing
			AbilitySystemComponent->ClearActorInfo();
		}

		OnAbilitySystemUninitialize.Broadcast();
	}

	AbilitySystemComponent = nullptr;
}

void ULyraPawnExtensionComponent::OnPawnReadyToInitialize_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnPawnReadyToInitialize.IsBoundToObject(Delegate.GetUObject()))
	{
		OnPawnReadyToInitialize.Add(Delegate);
	}

	if (bPawnReadyToInitialize)
	{
		Delegate.Execute();
	}
}

void ULyraPawnExtensionComponent::OnAbilitySystemInitialize_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemInitialize.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemInitialize.Add(Delegate);
	}

	if (AbilitySystemComponent)
	{
		Delegate.Execute();
	}
}

void ULyraPawnExtensionComponent::OnAbilitySystemUninitialize_Register(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemUninitialize.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemUninitialize.Add(Delegate);
	}
}

void ULyraPawnExtensionComponent::SetPawnData(const ULyraPawnData* InPawnData)
{
	check(InPawnData);

	bPawnReadyToInitialize = false;

	APawn* Pawn = GetPawnChecked<APawn>();

	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		// UE_LOG(LogLyra, Error, TEXT("Trying to set PawnData [%s] on pawn [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(Pawn), *GetNameSafe(PawnData));
		return;
	}

	PawnData = InPawnData;

	CheckPawnReadyToInitialize();
}