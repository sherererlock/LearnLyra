// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraPawnComponent.h"
#include "Input/LyraMappableConfigPair.h"

#include "LyraHeroComponent.generated.h"

class UInputComponent;
struct FInputActionValue;
/**
 * 
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class LEARNLYRA_API ULyraHeroComponent : public ULyraPawnComponent
{
	GENERATED_BODY()

public:
	ULyraHeroComponent(const FObjectInitializer& ObjectInitializer);

	/** True if this has completed OnPawnReadyToInitialize so is prepared for late-added features */
	bool HasPawnInitialized() const;

	/** True if this player has sent the BindInputsNow event and is prepared for bindings */
	bool IsReadyToBindInputs() const;

protected:
	virtual void OnRegister() override;

	virtual bool IsPawnComponentReadyToInitialize() const override;
	void OnPawnReadyToInitialize();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);
	void Input_AutoRun(const FInputActionValue& InputActionValue);

protected:

	UPROPERTY(EditAnywhere)
	TArray<FMappableConfigPair> DefaultInputConfigs;

	// True when the pawn has fully finished initialization
	bool bPawnHasInitialized;

	// True when player input bindings have been applyed, will never be true for non-players
	bool bReadyToBindInputs;
};
