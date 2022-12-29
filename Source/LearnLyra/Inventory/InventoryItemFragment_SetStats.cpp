// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemFragment_SetStats.h"
#include "Inventory/LyraInventoryItemInstance.h"

void UInventoryItemFragment_SetStats::OnInstanceCreated(ULyraInventoryItemInstance* Instance) const
{
	for (const auto& KVP : InitialItemStats)
	{
		Instance->AddStatTagStack(KVP.Key, KVP.Value);
	}
}

int32 UInventoryItemFragment_SetStats::GetItemStatByTag(FGameplayTag Tag) const
{
	if (const int32* StatPtr = InitialItemStats.Find(Tag))
	{
		return *StatPtr;
	}

	return 0;
}
