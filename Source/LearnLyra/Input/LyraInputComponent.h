// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/LyraMappableConfigPair.h"
#include "LyraInputConfig.h"
#include "EnhancedInputSubsystems.h"

#include "LyraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class LEARNLYRA_API ULyraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	ULyraInputComponent(const FObjectInitializer& ObjectInitializer);

	void AddInputMappings(const ULyraInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;
	void RemoveInputMappings(const ULyraInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;

	template<class UserClass, typename FuncType>
	void BindNativeAction(const ULyraInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const ULyraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);

	void RemoveBinds(TArray<uint32>& BindHandles);

	void AddInputConfig(const FLoadedMappableConfigPair& ConfigPair, UEnhancedInputLocalPlayerSubsystem* InputSubsystem);
	void RemoveInputConfig(const FLoadedMappableConfigPair& ConfigPair, UEnhancedInputLocalPlayerSubsystem* InputSubsystem);
};

template<class UserClass, typename FuncType>
void ULyraInputComponent::BindNativeAction(const ULyraInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void ULyraInputComponent::BindAbilityActions(const ULyraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);
	
	for (const FLyraInputAction& action : InputConfig->AbilityInputActions)
	{
		if (action.InputAction && action.InputTag.IsValid())
		{
			if (PressedFunc)
				BindHandles.Add(BindAction(action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, action.InputTag).GetHandle());

			if (ReleasedFunc)
				BindHandles.Add(BindAction(action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, action.InputTag).GetHandle());
		}
	}
}