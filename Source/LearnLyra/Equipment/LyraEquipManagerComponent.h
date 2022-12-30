// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Components/PawnComponent.h"
#include "AbilitySystem/Abilities/LyraAbilitySet.h"

#include "LyraEquipManagerComponent.generated.h"

class ULyraEquipmentDefinition;
class ULyraEquipmentInstance;
class ULyraAbilitySystemComponent;
struct FLyraEquipmentList;
class ULyraEquipmentManagerComponent;

USTRUCT(BlueprintType)
struct FLyraAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FLyraAppliedEquipmentEntry()
	{}

	FString GetDebugString();

private:
	friend FLyraEquipmentList;
	friend ULyraEquipmentManagerComponent;

	UPROPERTY()
	TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	ULyraEquipmentInstance* Instance = nullptr;

	// Authority-only list of granted handles
	UPROPERTY(NotReplicated)
	FLyraAbilitySet_GrantedHandles GrantedHandles;
};

USTRUCT(BlueprintType)
struct FLyraEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY();

	FLyraEquipmentList() : OwnerComponent(nullptr) {}

	FLyraEquipmentList(UActorComponent* Component) : OwnerComponent(Component) {}

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemoveIndecies, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddIndecies, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndecies, int32 FinalSize);
	//~FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FLyraAppliedEquipmentEntry, FLyraEquipmentList>(Entries, DeltaParams, *this);
	}

	ULyraEquipmentInstance* AddEntry(TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(ULyraEquipmentInstance* Instance);

private:
	ULyraAbilitySystemComponent* GetAbilitySystemComponent() const;
	friend ULyraEquipmentManagerComponent;

private:

	UPROPERTY()
	UActorComponent* OwnerComponent;

	UPROPERTY()
	TArray<FLyraAppliedEquipmentEntry> Entries;
};

template<>
struct TStructOpsTypeTraits<FLyraEquipmentList> : public TStructOpsTypeTraitsBase2<FLyraEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};

/**
 * 
 */
UCLASS(BlueprintType, Const)
class LEARNLYRA_API ULyraEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
	
public:
	ULyraEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	ULyraEquipmentInstance* EquipItem(TSubclassOf<ULyraEquipmentDefinition> EquipmentDefintion);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnEquipItem(ULyraEquipmentInstance* Instance);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ULyraEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<ULyraEquipmentInstance> InstanceType);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<ULyraEquipmentInstance*> GetEquipmentInstanceOfType(TSubclassOf<ULyraEquipmentInstance> InstanceType);

	template<typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

private:
	UPROPERTY(Replicated)
	FLyraEquipmentList EquipmentList;
};
