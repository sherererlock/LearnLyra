// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilitySystem/Abilities/LyraAbilityCost.h"
#include "LyraAbilityCost_InventoryItem.generated.h"

class ULyraInventoryItemDefinition;

/**
 * 
 */
UCLASS(meta = (DisplayName = "Inventory Item"))
class LEARNLYRA_API ULyraAbilityCost_InventoryItem : public ULyraAbilityCost
{
	GENERATED_BODY()
	
public:
	ULyraAbilityCost_InventoryItem();

	virtual bool CheckCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const ULyraGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	/** How much of the item to spend (keyed on ability level) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AbilityCost)
	FScalableFloat Quantity;

	/** Which item to consume */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AbilityCost)
	TSubclassOf<ULyraInventoryItemDefinition> ItemDefinition;
};
