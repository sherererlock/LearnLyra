// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "Equipment/LyraEquipmentDefinition.h"
#include "InventoryItemFragment_EquipItem.generated.h"

/**
 * 
 */
UCLASS()
class LEARNLYRA_API UInventoryItemFragment_EquipItem : public ULyraInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Lyra)
	TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition;
};
