// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LyraGameplayAbility.generated.h"

/**
 * ELyraAbilityActivationPolicy
 *
 *	Defines how an ability is meant to activate.
 */
UENUM(BlueprintType)
enum class ELyraAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};


class ALyraCharacter;
/**
 * 
 */
UCLASS()
class LEARNLYRA_API ULyraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	ULyraGameplayAbility(const FObjectInitializer& objectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
	ALyraCharacter* GetLyraCharacterFromActorInfo() const;

	ELyraAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

protected:
	//~UGameplayAbility interface

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Hanlde, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Ability Activation")
	ELyraAbilityActivationPolicy ActivationPolicy;
};
