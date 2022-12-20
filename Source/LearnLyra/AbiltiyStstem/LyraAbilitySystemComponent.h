// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "LyraAbilitySystemComponent.generated.h"

struct FGameplayTag;
struct FGameplayAbilitySpec;
/**
 * 
 */
UCLASS()
class LEARNLYRA_API ULyraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	ULyraAbilitySystemComponent(const FObjectInitializer& ObjectInitialzer = FObjectInitializer::Get());

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

protected:

	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

protected:

	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
