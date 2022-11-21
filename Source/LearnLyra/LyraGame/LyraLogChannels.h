// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"

LEARNLYRA_API DECLARE_LOG_CATEGORY_EXTERN(LogLyra, Log, All);
LEARNLYRA_API DECLARE_LOG_CATEGORY_EXTERN(LogLyraExperience, Log, All);
LEARNLYRA_API DECLARE_LOG_CATEGORY_EXTERN(LogLyraAbilitySystem, Log, All);
LEARNLYRA_API DECLARE_LOG_CATEGORY_EXTERN(LogLyraTeams, Log, All);

LEARNLYRA_API FString GetClientServerContextString(UObject* ContextObject = nullptr);