// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraPawnExtensionComponent.h"

ULyraPawnExtensionComponent::ULyraPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULyraPawnExtensionComponent::HandleControllerChanged()
{
}

void ULyraPawnExtensionComponent::SetupPlayerInputComponent()
{
}

bool ULyraPawnExtensionComponent::CheckPawnReadyToInitialize()
{
	return false;
}

void ULyraPawnExtensionComponent::OnPawnReadyToInitialize_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate)
{
}
