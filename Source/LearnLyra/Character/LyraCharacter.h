// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "LyraCharacter.generated.h"

class ULyraPawnExtensionComponent;
class ULyraCameraComponent;
class ULyraHealthComponent;
class ULyraInventoryItemDefinition;
class ULyraAbilitySystemComponent;

UCLASS()
class LEARNLYRA_API ALyraCharacter : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALyraCharacter(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemUninitialized();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	void InitializeGameplayTags();

	// Begins the death sequence for the character (disables collision, disables movement, etc...)
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	// Ends the death sequence for the character (detaches controller, destroys pawn, etc...)
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	// Called when the death sequence for the character has completed
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDeathFinished"))
	void K2_OnDeathFinished();

	void DisableMovementAndCollision();
	void DestroyDueToDeath();
	void UninitAndDestroy();


	// Temp
	UFUNCTION(BlueprintCallable)
	void Equip(TSubclassOf<ULyraInventoryItemDefinition> ItemDefinition);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	ULyraAbilitySystemComponent* GetLyraAbilitySystemComponent() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Character", Meta = (AllowPrivateAccess = "true"))
	ULyraPawnExtensionComponent* PawnExtComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Character", Meta = (AllowPrivateAccess = "true"))
	ULyraCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Character", Meta = (AllowPrivateAccess = "true"))
	ULyraHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Character", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// Í¨¹ý IAbilitySystemInterface ¼Ì³Ð
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


};
