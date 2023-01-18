// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatures/GameFeatureAction_WorldAction.h"

#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "UI/LyraHUD.h"
#include "UIExtensionSystem.h"

#include "GameFeatureAction_AddWidgets.generated.h"

struct FWorldContext;
struct FComponentRequestHandle;

USTRUCT()
struct FLyraHUDLayoutRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, Category = UI, meta = (AssetBundles = "Client"))
	TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

	UPROPERTY(EditAnywhere, Category = UI, meta = (Categories = "UI.Layer"))
	FGameplayTag LayerID;
};

USTRUCT()
struct FLyraHUDElementEntry
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, Category = UI, meta = (AssetBundles = "Client"))
	TSoftClassPtr<UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere, Category = UI)
	FGameplayTag SlotID;
};

/**
 * 
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Add Widget"))
class UGameFeatureAction_AddWidgets final : public UGameFeatureAction_WorldAction
{
	GENERATED_BODY()
	
public:
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

#if WITH_EDITORONLY_DATA
	virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
#endif

private:
	UPROPERTY(EditAnywhere, Category = UI, meta = (TitleProperty = "{LayerID}->{LayoutClass}"))
	TArray<FLyraHUDLayoutRequest> Layout;

	UPROPERTY(EditAnywhere, Category = UI, meta = (TitleProperty = "{SlotID}->{WidgetClass}"))
	TArray<FLyraHUDElementEntry> Widgets;

private:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
		TArray<TWeakObjectPtr<UCommonActivatableWidget>> LayoutsAdded;
		TArray<FUIExtensionHandle> ExtensionHandles;
	};

	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

	void Reset(FPerContextData& ActiveData);
	void HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

	void AddWidgets(AActor* Actor, FPerContextData& ActiveData);
	void RemoveWidgets(AActor* Actor, FPerContextData& ActiveData);
};
