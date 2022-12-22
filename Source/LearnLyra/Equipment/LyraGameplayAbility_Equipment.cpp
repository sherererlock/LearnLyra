// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraGameplayAbility_Equipment.h"
#include "LyraEquipmentInstance.h"
//#include "Inventory/LyraInventoryItemInstance.h"

ULyraGameplayAbility_Equipment::ULyraGameplayAbility_Equipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ULyraEquipmentInstance* ULyraGameplayAbility_Equipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<ULyraEquipmentInstance>(Spec->SourceObject);
	}

	return nullptr;
}

//ULyraInventoryItemInstance* ULyraGameplayAbility_Equipment::GetAssociatedItem() const
//{
//	if (ULyraEquipmentInstance* Equipment = GetAssociatedEquipment())
//	{
//		return Cast<ULyraInventoryItemInstance>(Equipment->GetInstigator());
//	}
//	return nullptr;
//}


#if WITH_EDITOR
EDataValidationResult ULyraGameplayAbility_Equipment::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = Super::IsDataValid(ValidationErrors);

	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		ValidationErrors.Add(NSLOCTEXT("Lyra", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}

#endif