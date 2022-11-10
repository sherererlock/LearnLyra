// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraHeroComponent.h"

ULyraHeroComponent::ULyraHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool ULyraHeroComponent::HasPawnInitialized() const
{
	return false;
}

bool ULyraHeroComponent::IsReadyToBindInputs() const
{
	return false;
}

void ULyraHeroComponent::OnRegister()
{
}

bool ULyraHeroComponent::IsPawnComponentReadyToInitialize() const
{
	return false;
}

void ULyraHeroComponent::OnPawnReadyToInitialize()
{
}

void ULyraHeroComponent::BeginPlay()
{
}

void ULyraHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

void ULyraHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
}

void ULyraHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
}

void ULyraHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
}

void ULyraHeroComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
}

void ULyraHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
}

void ULyraHeroComponent::Input_AutoRun(const FInputActionValue& InputActionValue)
{
}
