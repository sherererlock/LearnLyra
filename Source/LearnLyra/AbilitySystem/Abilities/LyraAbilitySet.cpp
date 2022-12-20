// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/LyraAbilitySet.h"
#include "AbiltiyStstem/LyraAbilitySystemComponent.h"
#include "LyraGameplayAbility.h"

void FLyraAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

ULyraAbilitySet::ULyraAbilitySet(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
}

void ULyraAbilitySet::GiveToAbilitySystem(ULyraAbilitySystemComponent* ASC, FLyraAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(ASC);

	if (!ASC->IsOwnerActorAuthoritative())
		return;

	for (int32 i = 0; i < GrantedGameplayAbilities.Num(); i++)
	{
		const FLyraAbilitySet_GameplayAbility& Ability = GrantedGameplayAbilities[i];
		if (!IsValid(Ability.Ability))
			continue;

		ULyraGameplayAbility* AbilityCDO = Ability.Ability->GetDefaultObject<ULyraGameplayAbility>();

		FGameplayAbilitySpec ASPec(AbilityCDO, Ability.AbilityLevel);
		ASPec.SourceObject = SourceObject;
		ASPec.DynamicAbilityTags.AddTag(Ability.InputTag);

		const FGameplayAbilitySpecHandle ASHandle = ASC->GiveAbility(ASPec);
		if(OutGrantedHandles)
			OutGrantedHandles->AddAbilitySpecHandle(ASHandle);
	}
}
