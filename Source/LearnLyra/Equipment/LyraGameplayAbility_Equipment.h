// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/LyraGameplayAbility.h"

#include "LyraGameplayAbility_Equipment.generated.h"

class ULyraEquipmentInstance;
class ULyraInventoryItemInstance;

/**
 * LyraGameplayAbility_Equipment
 *
 * An ability granted by and associated with an equipment instance
 */
UCLASS()
class ULyraGameplayAbility_Equipment : public ULyraGameplayAbility
{
	GENERATED_BODY()

public:

	ULyraGameplayAbility_Equipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Lyra|Ability")
	ULyraEquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
	ULyraInventoryItemInstance* GetAssociatedItem() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif

};
