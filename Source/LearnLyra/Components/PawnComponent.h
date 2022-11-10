// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFrameworkComponent.h"
#include "PawnComponent.generated.h"

/**
 * 
 */
UCLASS()
class LEARNLYRA_API UPawnComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()
	
public :
	UPawnComponent(const FObjectInitializer& ObjectInitializer);

	template<class T>
	T* GetPawn()const
	{
		return Cast<T>(GetOwner());
	}

	template<class T>
	T* GetPawnChecked()const
	{
		return CastChecked<T>(GetOwner());
	}

	template<class T>
	T* GetPlayerState() const
	{
		return GetPawnChecked<APawn>()->GetPlayerState<T>();
	}

	template<class T>
	T* GetController() const
	{
		return GetPawnChecked<APawn>()->GetController<T>();
	}
};
