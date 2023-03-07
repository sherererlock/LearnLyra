// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attribute/LyraAttributeSet.h"
#include "AbilitySystem/LyraAbilitySystemComponent.h"

ULyraAttributeSet::ULyraAttributeSet()
{
}

UWorld* ULyraAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

ULyraAbilitySystemComponent* ULyraAttributeSet::GetLyraGameplayAbilitySystemComponent() const
{
	return Cast<ULyraAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
