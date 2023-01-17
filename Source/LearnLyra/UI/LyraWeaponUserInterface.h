// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "LyraWeaponUserInterface.generated.h"

class ULyraWeaponInstance;
/**
 * 
 */
UCLASS()
class LEARNLYRA_API ULyraWeaponUserInterface : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	ULyraWeaponUserInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged(ULyraWeaponInstance* OldWeapon, ULyraWeaponInstance* NewWeapon);

private:
	void RebuildWidgetFromWeapon();

private:
	UPROPERTY(Transient)
	TObjectPtr<ULyraWeaponInstance> CurrentInstance;
};
