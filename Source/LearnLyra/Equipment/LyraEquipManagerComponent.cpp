// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LyraEquipManagerComponent.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/LyraAbilitySet.h"
#include "LyraEquipmentInstance.h"
#include "LyraEquipmentDefinition.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "Character/LyraCharacter.h"

FString FLyraAppliedEquipmentEntry::GetDebugString()
{
	return FString::Printf(TEXT("%s of %s"), *GetNameSafe(Instance), *GetNameSafe(EquipmentDefinition.Get()));
}

void FLyraEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemoveIndecies, int32 FinalSize)
{
	for (int32 Index : RemoveIndecies)
	{
		const FLyraAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnUnequipped();
		}
	}
}

void FLyraEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddIndecies, int32 FinalSize)
{
	for (int32 Index : AddIndecies)
	{
		const FLyraAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnEquipped();
		}
	}
}

void FLyraEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndecies, int32 FinalSize)
{
}

ULyraEquipmentInstance* FLyraEquipmentList::AddEntry(TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition)
{
	ULyraEquipmentInstance* Result = nullptr;
	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	const ULyraEquipmentDefinition* EquipmentCDO = GetDefault<ULyraEquipmentDefinition>(EquipmentDefinition);
	TSubclassOf<ULyraEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = ULyraEquipmentInstance::StaticClass();
	}

	FLyraAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<ULyraEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);  //@TODO: Using the actor instead of component as the outer due to UE-127172
	Result = NewEntry.Instance;

	if (ULyraAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (TObjectPtr<const ULyraAbilitySet> AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, /*inout*/ &NewEntry.GrantedHandles, Result);
		}
	}
	else
	{
		//@TODO: Warning logging?
	}

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

	MarkItemDirty(NewEntry);

	return Result;
}

void FLyraEquipmentList::RemoveEntry(ULyraEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FLyraAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			if (ULyraAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyEquipmentActors();

			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

ULyraAbilitySystemComponent* FLyraEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	ALyraCharacter* LyraCharacter = Cast<ALyraCharacter>(OwningActor);
	return Cast<ULyraAbilitySystemComponent>(LyraCharacter->GetLyraAbilitySystemComponent());
}

ULyraEquipmentManagerComponent::ULyraEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	EquipmentList(this)

{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void ULyraEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

ULyraEquipmentInstance* ULyraEquipmentManagerComponent::EquipItem(TSubclassOf<ULyraEquipmentDefinition> EquipmentDefintion)
{
	ULyraEquipmentInstance* Result = nullptr;
	if (EquipmentDefintion != nullptr)
	{
		Result = EquipmentList.AddEntry(EquipmentDefintion);
		if (Result != nullptr)
		{
			Result->OnEquipped();
		}
	}

	return Result;
}

void ULyraEquipmentManagerComponent::UnEquipItem(ULyraEquipmentInstance* Instance)
{
	if (Instance != nullptr)
	{
		Instance->OnUnequipped();
		EquipmentList.RemoveEntry(Instance);
	}
}

bool ULyraEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FLyraAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		ULyraEquipmentInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void ULyraEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void ULyraEquipmentManagerComponent::UninitializeComponent()
{
	TArray<ULyraEquipmentInstance*> AllEquipmentInstances;

	// gathering all instances before removal to avoid side effects affecting the equipment list iterator
	for (const FLyraAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		AllEquipmentInstances.Add(Entry.Instance);
	}

	for (ULyraEquipmentInstance* EquipInstance : AllEquipmentInstances)
	{
		UnEquipItem(EquipInstance);
	}

	Super::UninitializeComponent();
}

ULyraEquipmentInstance* ULyraEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<ULyraEquipmentInstance> InstanceType)
{
	for (FLyraAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (ULyraEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

TArray<ULyraEquipmentInstance*> ULyraEquipmentManagerComponent::GetEquipmentInstanceOfType(TSubclassOf<ULyraEquipmentInstance> InstanceType)
{
	TArray<ULyraEquipmentInstance*> Results;
	for (const FLyraAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (ULyraEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}
