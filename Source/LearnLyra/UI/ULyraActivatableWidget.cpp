// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ULyraActivatableWidget.h"

#include "Editor/WidgetCompilerLog.h"
#include "Input/CommonInputMode.h"
#include "Input/UIActionBindingHandle.h"

#define LOCTEXT_NAMESPACE "Lyra"

ULyraActivatableWidget::ULyraActivatableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TOptional<FUIInputConfig> ULyraActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case ELyraWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case ELyraWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case ELyraWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
	case ELyraWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}
