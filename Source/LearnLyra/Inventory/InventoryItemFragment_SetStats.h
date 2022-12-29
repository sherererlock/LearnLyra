// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "GameplayTagContainer.h"

#include "InventoryItemFragment_SetStats.generated.h"

/**
 * 
 */
UCLASS()
class LEARNLYRA_API UInventoryItemFragment_SetStats : public ULyraInventoryItemFragment
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TMap<FGameplayTag, int32> InitialItemStats;

public:
	virtual void OnInstanceCreated(ULyraInventoryItemInstance* Instance) const override;

	int32 GetItemStatByTag(FGameplayTag Tag) const;
};
