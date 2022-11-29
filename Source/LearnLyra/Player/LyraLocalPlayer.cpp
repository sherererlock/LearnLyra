// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LyraLocalPlayer.h"

ULyraLocalPlayer::ULyraLocalPlayer()
{
}

void ULyraLocalPlayer::PostInitProperties()
{
    Super::PostInitProperties();
}

void ULyraLocalPlayer::SwitchController(APlayerController* PC)
{
    Super::SwitchController(PC);
}

bool ULyraLocalPlayer::SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld)
{
    const bool bResult = Super::SpawnPlayActor(URL, OutError, InWorld);
    return bResult;
}

void ULyraLocalPlayer::InitOnlineSession()
{
    Super::InitOnlineSession();
}
