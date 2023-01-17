// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LyraWeaponUserInterface.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/LyraWeaponInstance.h"
#include "Equipment/LyraEquipManagerComponent.h"
#include "GameFramework/Pawn.h"

ULyraWeaponUserInterface::ULyraWeaponUserInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULyraWeaponUserInterface::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULyraWeaponUserInterface::NativeDestruct()
{
	Super::NativeDestruct();
}

void ULyraWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (ULyraEquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<ULyraEquipmentManagerComponent>())
		{
			if (ULyraWeaponInstance* NewInstance = EquipmentManager->GetFirstInstanceOfType<ULyraWeaponInstance>())
			{
				if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
				{
					ULyraWeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					RebuildWidgetFromWeapon();
					OnWeaponChanged(OldWeapon, CurrentInstance);
				}
			}
		}
	}

}

void ULyraWeaponUserInterface::RebuildWidgetFromWeapon()
{
}
