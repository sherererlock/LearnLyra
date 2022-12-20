// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/LyraPawnExtensionComponent.h"
#include "LyraGame/LyraGameplayTags.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ALyraCharacter::ALyraCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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
	return nullptr;
}

