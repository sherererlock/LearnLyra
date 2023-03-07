// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/LyraAbilitySet.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "LyraGameplayAbility.h"

void FLyraAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FLyraAbilitySet_GrantedHandles::AddGamplayEffectSpecHandle(const FActiveGameplayEffectHandle& handle)
{
	if (handle.IsValid())
	{
		GameplayEffectHandles.Add(handle);
	}
}

void FLyraAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	if (Set != nullptr)
	{
		GrandtedAttributeSets.Add(Set);
	}
}

void FLyraAbilitySet_GrantedHandles::TakeFromAbilitySystem(ULyraAbilitySystemComponent* LyraASC)
{
	check(LyraASC);

	if (!LyraASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			LyraASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			LyraASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : GrandtedAttributeSets)
	{
		LyraASC->GetSpawnedAttributes_Mutable().Remove(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrandtedAttributeSets.Reset();
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

	for (int32 i = 0; i < GrantedGameplayEffects.Num(); i++)
	{
		const FLyraAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[i];

		if(!IsValid(EffectToGrant.GameplayEffect))
			continue;

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();

		const FActiveGameplayEffectHandle GameplayEffectHandle = ASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, ASC->MakeEffectContext());

		if(OutGrantedHandles)
			OutGrantedHandles->AddGamplayEffectSpecHandle(GameplayEffectHandle);
	}

	// Grant the attribute sets.
	for (int32 SetIndex = 0; SetIndex < GrantedAttributeSets.Num(); ++SetIndex)
	{
		const FLyraAbilitySet_AttributeSet& SetToGrant = GrantedAttributeSets[SetIndex];

		if (!IsValid(SetToGrant.AttributeSet))
			continue;

		UAttributeSet* NewSet = NewObject<UAttributeSet>(ASC->GetOwner(), SetToGrant.AttributeSet);
		ASC->AddAttributeSetSubobject(NewSet);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}
}
