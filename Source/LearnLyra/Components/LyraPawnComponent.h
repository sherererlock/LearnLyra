// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "LyraPawnComponent.generated.h"

UINTERFACE(BlueprintType)
class ULyraReadyInterface : public UInterface
{
	GENERATED_BODY()
};


class ILyraReadyInterface
{
	GENERATED_BODY()

public:
	virtual bool IsPawnComponentReadyToInitialize() const = 0;
};


/**
 * 
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class LEARNLYRA_API ULyraPawnComponent : public UPawnComponent, public ILyraReadyInterface
{
	GENERATED_BODY()
	
public:
	ULyraPawnComponent(const FObjectInitializer& ObjectInitializer);

	virtual bool IsPawnComponentReadyToInitialize() const override { return true; }
};
