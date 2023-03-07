// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/LyraGameplayCueManager.h"
#include "AbilitySystemGlobals.h"

ULyraGameplayCueManager::ULyraGameplayCueManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool ULyraGameplayCueManager::ShouldAsyncLoadRuntimeObjectLibraries() const
{
	return false;
}

void ULyraGameplayCueManager::DumpGameplayCues(const TArray<FString>& Args)
{/*
	ULyraGameplayCueManager* GCM = Cast<ULyraGameplayCueManager>(UAbilitySystemGlobals::Get().GetGameplayCueManager());
	if (!GCM)
	{
		UE_LOG(LogTemp, Error, TEXT("DumpGameplayCues failed. No ULyraGameplayCueManager found."));
		return;
	}

	const bool bIncludeRefs = Args.Contains(TEXT("Refs"));

	UE_LOG(LogTemp, Log, TEXT("=========== Dumping Always Loaded Gameplay Cue Notifies ==========="));
	for (UClass* CueClass : GCM->AlwaysLoadedCues)
	{
		UE_LOG(LogTemp, Log, TEXT("  %s"), *GetPathNameSafe(CueClass));
	}

	UE_LOG(LogTemp, Log, TEXT("=========== Dumping Preloaded Gameplay Cue Notifies ==========="));
	for (UClass* CueClass : GCM->PreloadedCues)
	{
		TSet<FObjectKey>* ReferencerSet = GCM->PreloadedCueReferencers.Find(CueClass);
		int32 NumRefs = ReferencerSet ? ReferencerSet->Num() : 0;
		UE_LOG(LogTemp, Log, TEXT("  %s (%d refs)"), *GetPathNameSafe(CueClass), NumRefs);
		if (bIncludeRefs && ReferencerSet)
		{
			for (const FObjectKey& Ref : *ReferencerSet)
			{
				UObject* RefObject = Ref.ResolveObjectPtr();
				UE_LOG(LogTemp, Log, TEXT("    ^- %s"), *GetPathNameSafe(RefObject));
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("=========== Dumping Gameplay Cue Notifies loaded on demand ==========="));
	int32 NumMissingCuesLoaded = 0;
	if (GCM->RuntimeGameplayCueObjectLibrary.CueSet)
	{
		for (const FGameplayCueNotifyData& CueData : GCM->RuntimeGameplayCueObjectLibrary.CueSet->GameplayCueData)
		{
			if (CueData.LoadedGameplayCueClass && !GCM->AlwaysLoadedCues.Contains(CueData.LoadedGameplayCueClass) && !GCM->PreloadedCues.Contains(CueData.LoadedGameplayCueClass))
			{
				NumMissingCuesLoaded++;
				UE_LOG(LogTemp, Log, TEXT("  %s"), *CueData.LoadedGameplayCueClass->GetPathName());
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("=========== Gameplay Cue Notify summary ==========="));
	UE_LOG(LogTemp, Log, TEXT("  ... %d cues in always loaded list"), GCM->AlwaysLoadedCues.Num());
	UE_LOG(LogTemp, Log, TEXT("  ... %d cues in preloaded list"), GCM->PreloadedCues.Num());
	UE_LOG(LogTemp, Log, TEXT("  ... %d cues loaded on demand"), NumMissingCuesLoaded);
	UE_LOG(LogTemp, Log, TEXT("  ... %d cues in total"), GCM->AlwaysLoadedCues.Num() + GCM->PreloadedCues.Num() + NumMissingCuesLoaded);*/

}
