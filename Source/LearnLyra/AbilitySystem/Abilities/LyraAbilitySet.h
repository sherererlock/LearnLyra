// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySet.h"
#include "GameplayTagContainer.h"
#include "LyraAbilitySet.generated.h"

class ULyraAbilitySystemComponent;
class ULyraGameplayAbility;

USTRUCT(BlueprintType)
struct FLyraAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULyraGameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FLyraAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);

protected:

	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;


};

/**
 * 
 */
UCLASS(BlueprintType, Const)
class LEARNLYRA_API ULyraAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	ULyraAbilitySet(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

	void GiveToAbilitySystem(ULyraAbilitySystemComponent* ASC, FLyraAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:

	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = Ability))
	TArray<FLyraAbilitySet_GameplayAbility> GrantedGameplayAbilities;
};
