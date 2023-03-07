// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "InventoryItemFragment_Reticle.generated.h"

class ULyraReticleWidgetBase;
/**
 * 
 */
UCLASS()
class LEARNLYRA_API UInventoryItemFragment_Reticle : public ULyraInventoryItemFragment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Reticle)
	TArray<TSubclassOf<ULyraReticleWidgetBase>> ReticleWidgets;
};
