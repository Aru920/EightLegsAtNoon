// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ELNPlayerCharacter.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"

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
	AddInputMappingContext();

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

	bool bBoundFireInput = false;
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (FireAction)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AELNPlayerCharacter::HandleFirePressed);
			bBoundFireInput = true;
		}
	}

	if (!bBoundFireInput)
	{
		PlayerInputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &AELNPlayerCharacter::HandleFirePressed);
	}
}

void AELNPlayerCharacter::UpdateCursorAim(float DeltaTime)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController || !PlayerController->IsLocalController() || !ShotgunArms)
	{
		return;
	}

	FHitResult CursorHit;
	FVector TraceStart;
	FVector TraceEnd;
	if (GetCursorTrace(CursorHit, TraceStart, TraceEnd))
	{
		CursorWorldLocation = CursorHit.bBlockingHit ? CursorHit.ImpactPoint : TraceEnd;
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

void AELNPlayerCharacter::HandleFirePressed()
{
	if (bIsFireLocked)
	{
		return;
	}

	const bool bMontageStarted = PlayFireMontage();
	if (!bMontageStarted || !bFireTraceFromAnimNotify)
	{
		FireTrace();
	}
}

void AELNPlayerCharacter::FireTrace()
{
	FHitResult FireHit;
	FVector TraceStart;
	FVector TraceEnd;
	if (!GetCursorTrace(FireHit, TraceStart, TraceEnd))
	{
		return;
	}

	if (bDrawFireTraceDebug)
	{
		if (FireHit.bBlockingHit)
		{
			DrawDebugSphere(GetWorld(), FireHit.ImpactPoint, ImpactDebugSphereRadius, 12, FColor::Yellow, false, FireTraceDebugDuration, 0, 1.5f);
		}
	}

	if (FireHit.bBlockingHit)
	{
		CheckPistolHitOverlap(FireHit.ImpactPoint);
	}
}

bool AELNPlayerCharacter::PlayFireMontage()
{
	if (!FireMontage || !ShotgunArms)
	{
		return false;
	}

	UAnimInstance* AnimInstance = ShotgunArms->GetAnimInstance();
	if (!AnimInstance)
	{
		return false;
	}

	const float MontageLength = AnimInstance->Montage_Play(FireMontage, FireMontagePlayRate);
	if (MontageLength <= 0.f)
	{
		return false;
	}

	bIsFireLocked = true;

	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindUObject(this, &AELNPlayerCharacter::HandleFireMontageEnded);
	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, FireMontage);

	return true;
}

void AELNPlayerCharacter::HandleFireMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == FireMontage)
	{
		bIsFireLocked = false;
	}
}

void AELNPlayerCharacter::CheckPistolHitOverlap(const FVector& HitCenter)
{
	if (bDrawPistolHitRadiusDebug)
	{
		DrawDebugSphere(GetWorld(), HitCenter, PistolHitRadius, 24, FColor::Green, false, FireTraceDebugDuration, 0, 2.f);
	}

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ELNPistolHitOverlap), false, this);

	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		HitCenter,
		FQuat::Identity,
		ObjectQueryParams,
		FCollisionShape::MakeSphere(PistolHitRadius),
		QueryParams
	);

	TSet<AActor*> DamagedActors;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.GetActor();
		if (!HitActor || HitActor == this || HitActor->GetOwner() == this || DamagedActors.Contains(HitActor))
		{
			continue;
		}

		DamagedActors.Add(HitActor);
		UGameplayStatics::ApplyDamage(HitActor, PistolDamage, GetController(), this, nullptr);

		UE_LOG(LogTemp, Log, TEXT("Pistol hit overlap: %s"), *HitActor->GetName());
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Hit: %s"), *HitActor->GetName()));
		}
	}
}

bool AELNPlayerCharacter::GetCursorTrace(FHitResult& OutHit, FVector& OutTraceStart, FVector& OutTraceEnd) const
{
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController || !PlayerController->IsLocalController())
	{
		return false;
	}

	FVector MouseWorldLocation;
	FVector MouseWorldDirection;
	if (!PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection))
	{
		return false;
	}

	OutTraceStart = MouseWorldLocation;
	OutTraceEnd = MouseWorldLocation + MouseWorldDirection * CursorTraceDistance;

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ELNCursorTrace), false, this);
	GetWorld()->LineTraceSingleByChannel(OutHit, OutTraceStart, OutTraceEnd, ECC_GameTraceChannel1, QueryParams);
	return true;
}

void AELNPlayerCharacter::AddInputMappingContext() const
{
	if (!PlayerMappingContext)
	{
		return;
	}

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController || !PlayerController->IsLocalController())
	{
		return;
	}

	if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSubsystem->AddMappingContext(PlayerMappingContext, PlayerMappingPriority);
		}
	}
}

