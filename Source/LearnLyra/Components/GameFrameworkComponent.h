// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFrameworkComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, HideCategories = (Trigger, PhysicsVolume))
class LEARNLYRA_API UGameFrameworkComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameFrameworkComponent(const FObjectInitializer& ObjectInitializer);

	template<class T>
	T* GetGameInstance() const
	{
		AActor* Owener = GetOwner();
		return Owener ? Owener->GetGameInstance<T>() : nullptr;
	}

	bool HasAuthority() const;
		
};
