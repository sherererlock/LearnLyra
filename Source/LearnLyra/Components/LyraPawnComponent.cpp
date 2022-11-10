// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraPawnComponent.h"

ULyraPawnComponent::ULyraPawnComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}