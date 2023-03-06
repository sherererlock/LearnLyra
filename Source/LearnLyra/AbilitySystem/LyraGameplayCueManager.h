// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueManager.h"
#include "LyraGameplayCueManager.generated.h"

/**
 * 
 */
UCLASS()
class LEARNLYRA_API ULyraGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()

public:
	ULyraGameplayCueManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool ShouldAsyncLoadRuntimeObjectLibraries() const override;

	static void DumpGameplayCues(const TArray<FString>& Args);
};
