// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "LyraAbilityTargetData_SingleHit.generated.h"

/**
 * 
 */
USTRUCT()
struct LEARNLYRA_API FLyraAbilityTargetData_SingleHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()
	
public:
	FLyraAbilityTargetData_SingleHit()
		: CartridgeID(-1)
	{ }

	virtual void AddTargetDataToContext(FGameplayEffectContextHandle& Context, bool bIncludeActorArray) const override;

	/** ID to allow the identification of multiple bullets that were part of the same cartridge */
	UPROPERTY()
	int32 CartridgeID;

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FLyraAbilityTargetData_SingleHit::StaticStruct();
	}
};

template<>
struct TStructOpsTypeTraits<FLyraAbilityTargetData_SingleHit> : public TStructOpsTypeTraitsBase2<FLyraAbilityTargetData_SingleHit>
{
	enum
	{
		WithNetSerializer = true	// For now this is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};
