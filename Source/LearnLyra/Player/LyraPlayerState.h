// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "LyraPlayerState.generated.h"

class UAbilitySystemComponent;
class ULyraAbilitySystemComponent;
class ULyraExperienceDefinition;
class ULyraPawnData;
/**
 * 
 */
UCLASS()
class LEARNLYRA_API ALyraPlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ALyraPlayerState(const FObjectInitializer& ObjectInitilize = FObjectInitializer::Get());
	// Actor
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	// Actor

	// APlayerState
	virtual void Reset() override;
	virtual void ClientInitialize(AController* C) override;
	virtual void OnDeactivated() override;
	virtual void OnReactivated() override;

	ULyraAbilitySystemComponent* GetLyraAbilitySystemComponent() const { return AbilitySystemComponent; }

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const ULyraPawnData* InPawnData);

protected:
	// APlayerState
	virtual void CopyProperties(APlayerState* PlayerState);
	// APlayerState

	// Í¨¹ý IAbilitySystemInterface ¼Ì³Ð
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION()
	void OnRep_PawnData();

private:
	void OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience);


protected:

	UPROPERTY(VisibleAnywhere, Category = "Lyra|PlayerState")
	ULyraAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	const ULyraPawnData* PawnData;

public:
	static const FName NAME_LyraAbilityReady;
};
