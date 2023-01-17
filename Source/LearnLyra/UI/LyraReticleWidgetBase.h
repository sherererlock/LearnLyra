// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "LyraReticleWidgetBase.generated.h"

class ULyraWeaponInstance;
class ULyraInventoryItemInstance;

/**
 * 
 */
UCLASS(Abstract)
class LEARNLYRA_API ULyraReticleWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	ULyraReticleWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponInitialized();

	UFUNCTION(BlueprintCallable)
	void InitializeFromWeapon(ULyraWeaponInstance* InWeapon);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float ComputeSpreadAngle();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float ComputeMaxScreenSpaceSpreadRadius();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasFirstShotAccuracy() const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULyraWeaponInstance> WeaponInstance;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULyraInventoryItemInstance> InventoryInstance;
};
