// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "LyraAbilitySourceInterface.generated.h"

class AActor;
struct FGameplayTagContainer;
class UPhysicalMaterial;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULyraAbilitySourceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEARNLYRA_API ILyraAbilitySourceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	virtual float GetDistanceAttenuation(float Distance, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const = 0;

	virtual float GetPhysicalMaterialAttenuation(const UPhysicalMaterial* PhysicalMaterial, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const = 0;
};
