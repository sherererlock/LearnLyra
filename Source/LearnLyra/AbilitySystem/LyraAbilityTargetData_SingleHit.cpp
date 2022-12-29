// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/LyraAbilityTargetData_SingleHit.h"

void FLyraAbilityTargetData_SingleHit::AddTargetDataToContext(FGameplayEffectContextHandle& Context, bool bIncludeActorArray) const
{
	FGameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(Context, bIncludeActorArray);

	//// Add game-specific data
	//if (FLyraGameplayEffectContext* TypedContext = FLyraGameplayEffectContext::ExtractEffectContext(Context))
	//{
	//	TypedContext->CartridgeID = CartridgeID;
	//}
}

bool FLyraAbilityTargetData_SingleHit::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayAbilityTargetData_SingleTargetHit::NetSerialize(Ar, Map, bOutSuccess);

	Ar << CartridgeID;

	return true;
}
