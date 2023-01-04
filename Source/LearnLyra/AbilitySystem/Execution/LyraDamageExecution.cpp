// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraDamageExecution.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/Attribute/LyraHealthSet.h"
//#include "AbilitySystem/Attribute/LyraCombatSet.h"
//#include "AbilitySystem/LyraGameplayEffectContext.h"
#include "AbilitySystem/LyraAbilitySourceInterface.h"
//#include "Teams/LyraTeamSubsystem.h"

struct FDamageStatics
{

};

static FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}


ULyraDamageExecution::ULyraDamageExecution()
{

}

void ULyraDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
}
