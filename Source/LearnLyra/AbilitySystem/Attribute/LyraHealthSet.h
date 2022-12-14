// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attribute/LyraAttributeSet.h"
#include "AbiltiyStstem/LyraAbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "LyraHealthSet.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageImmunity);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageSelfDestruct);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_FellOutOfWorld);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Lyra_Damage_Message);

struct FGameplayEffectModCallbackData;
/**
 * 
 */
UCLASS(BlueprintType)
class LEARNLYRA_API ULyraHealthSet : public ULyraAttributeSet
{
	GENERATED_BODY()
	
public:
	ULyraHealthSet();

	ATTRIBUTE_ACCESSORS(ULyraHealthSet, Health);
	ATTRIBUTE_ACCESSORS(ULyraHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(ULyraHealthSet, Healing);

	// Delegate to broadcast when the health attribute reaches zero.
	mutable FLyraAttributeEvent OnOutOfHealth;

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:
	// The current health attribute.  The health will be capped by the max health attribute.  Health is hidden from modifiers so only executions can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Lyra|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	// The current max health attribute.  Max health is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Lyra|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	bool bOutOfHealth;

private:
	// Incoming healing. This is mapped directly to +Health
	UPROPERTY(BlueprintReadOnly, Category = "Lyra|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Healing;

	// Incoming damage. This is mapped directly to -Health
	UPROPERTY(BlueprintReadOnly, Category = "Lyra|Health", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
};
