// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "Character/LyraCharacter.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "Components/LyraHeroComponent.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_ABILITY_SIMPLE_FAILURE_MESSAGE, "Ability.UserFacingSimpleActivateFail.Message");
UE_DEFINE_GAMEPLAY_TAG(TAG_ABILITY_PLAY_MONTAGE_FAILURE_MESSAGE, "Ability.PlayMontageOnActivateFail.Message");

ULyraGameplayAbility::ULyraGameplayAbility(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}

ULyraAbilitySystemComponent* ULyraGameplayAbility::GetLyraAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ULyraAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}

ALyraPlayerController* ULyraGameplayAbility::GetLyraPlayerControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ALyraPlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr);
}

ALyraCharacter* ULyraGameplayAbility::GetLyraCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ALyraCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

AController* ULyraGameplayAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		// Look for a player controller or pawn in the owner chain.
		AActor* TestActor = CurrentActorInfo->OwnerActor.Get();
		while (TestActor)
		{
			if (AController* C = Cast<AController>(TestActor))
			{
				return C;
			}

			if (APawn* Pawn = Cast<APawn>(TestActor))
			{
				return Pawn->GetController();
			}

			TestActor = TestActor->GetOwner();
		}
	}

	return nullptr;
}

ULyraHeroComponent* ULyraGameplayAbility::GetHeroComponentFromActorInfo() const
{
	if(CurrentActorInfo)
	{
		ALyraCharacter* LyraCharacter = Cast<ALyraCharacter>(CurrentActorInfo->AvatarActor.Get());
		if (LyraCharacter)
		{
			return LyraCharacter->FindComponentByClass<ULyraHeroComponent>();
		}

	}

	return nullptr;
}

bool ULyraGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Hanlde, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Hanlde, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void ULyraGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULyraGameplayAbility::NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const
{
}
