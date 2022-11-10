// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworkComponent.h"

// Sets default values for this component's properties
UGameFrameworkComponent::UGameFrameworkComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoActivate = false;
}

bool UGameFrameworkComponent::HasAuthority() const
{
	AActor* Owner = GetOwner();
	check(Owner);
	return Owner->HasAuthority();
}

