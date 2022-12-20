// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/LyraGameplayAbility.h"
#include "Character/LyraCharacter.h"


ULyraGameplayAbility::ULyraGameplayAbility(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}

ALyraCharacter* ULyraGameplayAbility::GetLyraCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ALyraCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

bool ULyraGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Hanlde, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Hanlde, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void ULyraGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
