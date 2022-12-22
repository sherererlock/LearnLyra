// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraHeroComponent.h"
#include "Character/LyraPawnData.h"
#include "LyraPawnExtensionComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LyraGame/LyraGameplayTags.h"
#include "Input/LyraInputComponent.h"
#include "Settings/LyraSettingsLocal.h"
#include "LyraPlayerState.h"
#include "AbilitySystem/Abilities/LyraAbilitySet.h"
#include "Components/GameFrameworkComponentManager.h"

namespace LyraHero
{
	static const float LookYawRate = 300.0f;
	static const float LookPitchRate = 165.0f;
};

const FName ULyraHeroComponent::NAME_BindInputsNow("BindInputsNow");

ULyraHeroComponent::ULyraHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bPawnHasInitialized = false;
	bReadyToBindInputs = false;
}

bool ULyraHeroComponent::HasPawnInitialized() const
{
	return bPawnHasInitialized;
}

bool ULyraHeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void ULyraHeroComponent::AddAdditionalInputConfig(const ULyraInputConfig* InputConfig)
{
	TArray<uint32> BindHandles;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	ULyraInputComponent* LyraIC = Pawn->FindComponentByClass<ULyraInputComponent>();
	check(LyraIC);

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (const ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		LyraIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
	}
}

void ULyraHeroComponent::RemoveAdditionalInputConfig(const ULyraInputConfig* InputConfig)
{
}

void ULyraHeroComponent::OnRegister()
{
	Super::OnRegister();
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnExtComp->OnPawnReadyToInitialize_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnPawnReadyToInitialize));
		}
	}
	else
	{
		// UE_LOG(LogLyra, Error, TEXT("[ULyraHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));
	}

}

bool ULyraHeroComponent::IsPawnComponentReadyToInitialize() const
{
	if (!GetPlayerState<APlayerState>())
		return false;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
		return false;

	// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
	if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
	{
		AController* Controller = GetController<AController>();

		const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
			(Controller->PlayerState != nullptr) && \
			(Controller->PlayerState->GetOwner() == Controller);

		if (!bHasControllerPairedWithPS)
		{
			return false;
		}
	}

	const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
	const bool bIsBot = Pawn->IsBotControlled();

	if (bIsLocallyControlled && !bIsBot)
	{
		// The input component is required when locally controlled.
		if (!Pawn->InputComponent)
		{
			return false;
		}
	}

	return true;
}

void ULyraHeroComponent::OnPawnReadyToInitialize()
{
	if (bPawnHasInitialized)
		return;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const bool bIsLocallyControlled = Pawn->IsLocallyControlled();

	const ULyraPawnData* PawnData = nullptr;

	ALyraPlayerState* LyraPS = GetPlayerState<ALyraPlayerState>();
	check(LyraPS);

	if (ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		PawnData = PawnExtComp->GetPawnData<ULyraPawnData>();

		PawnExtComp->InitAbilitySystemComponent(LyraPS->GetLyraAbilitySystemComponent(), LyraPS);

		{
			const TArray<ULyraAbilitySet*>& ASet = PawnData->AbilitySet;
			for (int i = 0; i < ASet.Num(); i++)
				ASet[i]->GiveToAbilitySystem(LyraPS->GetLyraAbilitySystemComponent(), nullptr);
		}
	}

	if (APlayerController* PC = GetController<APlayerController>())
	{
		if (Pawn->InputComponent != nullptr)
			InitializePlayerInput(Pawn->InputComponent);
	}

	bPawnHasInitialized = true;
}

void ULyraHeroComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULyraHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ULyraHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (const ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const ULyraPawnData* PawnData = PawnExtComp->GetPawnData<ULyraPawnData>())
		{
			if (const ULyraInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FLyraGameplayTags& GameplayTags = FLyraGameplayTags::Get();
				for (const FMappableConfigPair& Pair : DefaultInputConfigs)
				{
					FMappableConfigPair::ActivatePair(Pair);
				}

				ULyraInputComponent* LyraIC = CastChecked<ULyraInputComponent>(PlayerInputComponent);
				LyraIC->AddInputMappings(InputConfig, Subsystem);

				if (ULyraSettingsLocal* LocalSettings = ULyraSettingsLocal::Get())
				{
					// LocalSettings->OnInputConfigActivated.AddUObject(this, &ULyraHeroComponent::OnInputConfigActivated);
					// LocalSettings->OnInputConfigDeactivated.AddUObject(this, &ULyraHeroComponent::OnInputConfigDeactivated);
				}

				TArray<uint32> BindHandles;
				LyraIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

				LyraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
				LyraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
				LyraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
				LyraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
				LyraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, /*bLogIfNotFound=*/ false);
			}
		}
	}

	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void ULyraHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (ULyraAbilitySystemComponent* LyraASC = PawnExtComp->GetLyraAbilitySystemComponent())
			{
				LyraASC->AbilityInputTagPressed(InputTag);
			}
		}
	}
}

void ULyraHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (ULyraAbilitySystemComponent* LyraASC = PawnExtComp->GetLyraAbilitySystemComponent())
			{
				LyraASC->AbilityInputTagReleased(InputTag);
			}
		}
	}
}

void ULyraHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void ULyraHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X * 50.0f * GetWorld()->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y * 50.0f * GetWorld()->GetDeltaSeconds());
	}
}

void ULyraHeroComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	const UWorld* World = GetWorld();
	check(World);

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X * LyraHero::LookYawRate * World->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y * LyraHero::LookPitchRate * World->GetDeltaSeconds());
	}
}

void ULyraHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
}

void ULyraHeroComponent::Input_AutoRun(const FInputActionValue& InputActionValue)
{
}
