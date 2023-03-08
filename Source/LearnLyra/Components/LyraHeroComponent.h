// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraPawnComponent.h"
#include "Input/LyraMappableConfigPair.h"
#include "GameplayAbilitySpec.h"
#include "LyraHeroComponent.generated.h"

class UInputComponent;
struct FInputActionValue;
class ULyraInputConfig;
class ULyraCameraMode;

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

	void AddAdditionalInputConfig(const ULyraInputConfig* InputConfig);
	void RemoveAdditionalInputConfig(const ULyraInputConfig* InputConfig);

	void SetAbilityCameraMode(TSubclassOf<ULyraCameraMode> CameraMode, const FGameplayAbilitySpecHandle& AbilityCameraModeOwningSpecHandle);
	void ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& AbilityCameraModeOwningSpecHandle);

protected:
	virtual void OnRegister() override;

	virtual bool IsPawnComponentReadyToInitialize() const override;
	void OnPawnReadyToInitialize();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);
	void Input_AutoRun(const FInputActionValue& InputActionValue);


	TSubclassOf<ULyraCameraMode> DetermineCameraMode();

protected:

	UPROPERTY(EditAnywhere)
	TArray<FMappableConfigPair> DefaultInputConfigs;

	// True when the pawn has fully finished initialization
	bool bPawnHasInitialized;

	// True when player input bindings have been applyed, will never be true for non-players
	bool bReadyToBindInputs;

	TSubclassOf<ULyraCameraMode> AbilityCameraMode;
	FGameplayAbilitySpecHandle OwningSpecHandle;

public:
	static const FName NAME_BindInputsNow;
};
