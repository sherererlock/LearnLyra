// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "LyraBotCreationComponent.generated.h"

class ULyraExperienceDefinition;

class AAIController;

/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class LEARNLYRA_API ULyraBotCreationComponent : public UGameStateComponent
{
	GENERATED_BODY()
	
public:
	ULyraBotCreationComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

private:
	void OnExperienceLoaded(const ULyraExperienceDefinition* Definition);

protected:
	UPROPERTY(EditDefaultsOnly, Category = Teams)
	int32 NumBotsToCreate = 5;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TSubclassOf<AAIController> BotControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> RandomBotNames;

protected:
	UPROPERTY(Transient)
	TArray<TObjectPtr<AAIController>> SpawnBotList;

#if WITH_SERVER_CODE
public :
	void Cheat_AddBot() { SpawnOneBot(); }
	void Cheat_RemoveBot() { RemoveOneBot(); }

protected:
	virtual void ServerCreateBots();

	virtual void SpawnOneBot();
	virtual void RemoveOneBot();

	FString CreateBotName(int32 Index);
#endif


};
