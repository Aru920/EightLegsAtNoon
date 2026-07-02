// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ELNPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

AELNPlayerCharacter::AELNPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

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

	SetActorTickEnabled(true);

	if (ShotgunArms)
	{
		ShotgunArmsBaseRotation = ShotgunArms->GetRelativeRotation();
	}
}

void AELNPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCursorAim(DeltaTime);
}

void AELNPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AELNPlayerCharacter::UpdateCursorAim(float DeltaTime)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController || !PlayerController->IsLocalController() || !ShotgunArms)
	{
		return;
	}

	FVector MouseWorldLocation;
	FVector MouseWorldDirection;
	if (PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection))
	{
		const FVector TraceEnd = MouseWorldLocation + MouseWorldDirection * CursorTraceDistance;

		FHitResult Hit;
		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ELNCursorAimTrace), false, this);
		if (GetWorld()->LineTraceSingleByChannel(Hit, MouseWorldLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			CursorWorldLocation = Hit.ImpactPoint;
		}
		else
		{
			CursorWorldLocation = TraceEnd;
		}
	}

	float MouseX = 0.f;
	float MouseY = 0.f;
	int32 ViewportX = 0;
	int32 ViewportY = 0;
	if (!PlayerController->GetMousePosition(MouseX, MouseY))
	{
		return;
	}

	PlayerController->GetViewportSize(ViewportX, ViewportY);
	if (ViewportX <= 0 || ViewportY <= 0)
	{
		return;
	}

	const float NormalizedX = FMath::Clamp((MouseX - ViewportX * 0.5f) / (ViewportX * 0.5f), -1.f, 1.f);
	const float NormalizedY = FMath::Clamp((MouseY - ViewportY * 0.5f) / (ViewportY * 0.5f), -1.f, 1.f);

	const FRotator TargetRotation(
		ShotgunArmsBaseRotation.Pitch - NormalizedY * ViewModelAimPitchLimit,
		ShotgunArmsBaseRotation.Yaw + NormalizedX * ViewModelAimYawLimit,
		ShotgunArmsBaseRotation.Roll
	);

	const FRotator NewRotation = FMath::RInterpTo(
		ShotgunArms->GetRelativeRotation(),
		TargetRotation,
		DeltaTime,
		ViewModelAimInterpSpeed
	);

	ShotgunArms->SetRelativeRotation(NewRotation);
}

