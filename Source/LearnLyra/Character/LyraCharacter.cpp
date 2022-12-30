// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/LyraPawnExtensionComponent.h"
#include "LyraGame/LyraGameplayTags.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/LyraHealthComponent.h"

// TODO: Delete Those
#include "Inventory/LyraInventoryItemDefinition.h"
#include "Inventory/LyraInventoryManagerComponent.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include "Inventory/InventoryItemFragment_EquipItem.h"
#include "Equipment/LyraEquipManagerComponent.h"
#include "Equipment/LyraEquipmentInstance.h"

// Sets default values
ALyraCharacter::ALyraCharacter(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));  // Rotate mesh to be X forward since it is exported as Y forward.
	MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 

	PawnExtComponent = CreateDefaultSubobject<ULyraPawnExtensionComponent>(TEXT("PawnExtComponent"));

	HealthComponent = CreateDefaultSubobject<ULyraHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	HealthComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void ALyraCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALyraCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ALyraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	PawnExtComponent->HandleControllerChanged();
}

void ALyraCharacter::UnPossessed()
{
	Super::UnPossessed();

	PawnExtComponent->HandleControllerChanged();
}

void ALyraCharacter::InitializeGameplayTags()
{

}

void ALyraCharacter::Equip(TSubclassOf<ULyraInventoryItemDefinition> ItemDefinition)
{
	AController* PlayerController = GetController();
	check(PlayerController);

	ULyraInventoryManagerComponent* ManagerComponent = PlayerController->FindComponentByClass<ULyraInventoryManagerComponent>();
	if (ManagerComponent == nullptr)
		return;

	check(ManagerComponent);
	if (ULyraInventoryItemInstance* Instance = ManagerComponent->AddItemDefinition(ItemDefinition))
	{
		if (const UInventoryItemFragment_EquipItem* EquipInfo = Instance->FindFragmentByClass<UInventoryItemFragment_EquipItem>())
		{
			TSubclassOf<ULyraEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef)
			{
				if (ULyraEquipmentManagerComponent* EquipmentManager = FindComponentByClass<ULyraEquipmentManagerComponent>())
				{
					ULyraEquipmentInstance* EquippedItem = EquipmentManager->EquipItem(EquipDef);
					if (EquippedItem != nullptr)
					{
						EquippedItem->SetInstigator(Instance);
					}
				}
			}
		}
	}
}

// Called every frame
void ALyraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALyraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}

UAbilitySystemComponent* ALyraCharacter::GetAbilitySystemComponent() const
{
	return PawnExtComponent->GetLyraAbilitySystemComponent();
}

ULyraAbilitySystemComponent* ALyraCharacter::GetLyraAbilitySystemComponent() const
{
	return PawnExtComponent->GetLyraAbilitySystemComponent();
}

void ALyraCharacter::OnDeathStarted(AActor*)
{
	DisableMovementAndCollision();
}

void ALyraCharacter::DisableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	UCharacterMovementComponent* LyraMoveComp = CastChecked<UCharacterMovementComponent>(GetCharacterMovement());
	LyraMoveComp->StopMovementImmediately();
	LyraMoveComp->DisableMovement();
}

void ALyraCharacter::DestroyDueToDeath()
{
	K2_OnDeathFinished();

	UninitAndDestroy();
}


void ALyraCharacter::UninitAndDestroy()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}

	// Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
	if (ULyraAbilitySystemComponent* LyraASC = GetLyraAbilitySystemComponent())
	{
		if (LyraASC->GetAvatarActor() == this)
		{
			PawnExtComponent->UninitializeAbilitySystem();
		}
	}

	SetActorHiddenInGame(true);
}


void ALyraCharacter::OnDeathFinished(AActor*)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

