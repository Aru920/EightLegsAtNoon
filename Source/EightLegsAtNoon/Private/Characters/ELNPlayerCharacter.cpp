// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ELNPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AELNPlayerCharacter::AELNPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->bOrientRotationToMovement = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1800.f;
	CameraBoom->SetRelativeRotation(FRotator(-70.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bUsePawnControlRotation = false;

	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCamera->bUsePawnControlRotation = false;

	ShotgunArms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShotgunArms"));
	ShotgunArms->SetupAttachment(TopDownCamera);
	ShotgunArms->SetRelativeLocation(FVector(95.f, 25.f, -45.f));
	ShotgunArms->SetRelativeRotation(FRotator::ZeroRotator);
	ShotgunArms->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShotgunArms->SetOnlyOwnerSee(true);
	ShotgunArms->bCastDynamicShadow = false;
	ShotgunArms->CastShadow = false;
}

void AELNPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AELNPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AELNPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

