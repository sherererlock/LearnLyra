// Fill out your copyright notice in the Description page of Project Settings.


#include "System/LyraGameSession.h"


ALyraGameSession::ALyraGameSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ALyraGameSession::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void ALyraGameSession::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}

