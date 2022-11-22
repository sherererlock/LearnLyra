// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraPawnComponent.h"
#include "LyraPawnExtensionComponent.generated.h"

class ULyraPawnData;
/**
 * 
 */
UCLASS()
class LEARNLYRA_API ULyraPawnExtensionComponent : public ULyraPawnComponent
{
	GENERATED_BODY()
	
public:

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const ULyraPawnData* InPawnData);

	ULyraPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "Lyra|Pawn")
	static ULyraPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULyraPawnExtensionComponent>() : nullptr); }

	// Should be called by the owning pawn when the pawn's controller changes.
	void HandleControllerChanged();

	// Should be called by the owning pawn when the input component is setup.
	void SetupPlayerInputComponent();

	// Call this anytime the pawn needs to check if it's ready to be initialized (pawn data assigned, possessed, etc..). 
	bool CheckPawnReadyToInitialize();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Lyra|Pawn", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsPawnReadyToInitialize() const { return bPawnReadyToInitialize; }

	void OnPawnReadyToInitialize_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);

protected:

	// Delegate fired when pawn has everything needed for initialization.
	FSimpleMulticastDelegate OnPawnReadyToInitialize;

protected:

	UPROPERTY(EditInstanceOnly, Category = "Lyra|Pawn")
	const ULyraPawnData* PawnData;

	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TObjectPtr<const ULyraPawnData> DefaultPawnData;

	// True when the pawn has everything needed for initialization.
	int32 bPawnReadyToInitialize : 1;
};
