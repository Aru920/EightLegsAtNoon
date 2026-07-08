#include "Characters/ELNDwarfCharacter.h"

#include "AI/ELNDwarfAIController.h"
#include "Characters/ELNSpiderCharacter.h"
#include "Engine/Engine.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"

AELNDwarfCharacter::AELNDwarfCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	AIControllerClass = AELNDwarfAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AELNDwarfCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentLives = MaxLives;
	bIsDead = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	if (PanicCheckInterval > 0.f)
	{
		GetWorldTimerManager().SetTimer(PanicTimerHandle, this, &AELNDwarfCharacter::UpdatePanicState, PanicCheckInterval, true);
	}
}

float AELNDwarfCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser
)
{
	if (bIsDead || DamageAmount <= 0.f)
	{
		return 0.f;
	}

	const int32 DamageToApply = FMath::Max(1, FMath::RoundToInt(DamageAmount));
	const int32 OldLives = CurrentLives;
	CurrentLives = FMath::Clamp(CurrentLives - DamageToApply, 0, MaxLives);

	const int32 DamageTaken = OldLives - CurrentLives;
	if (DamageTaken <= 0)
	{
		return 0.f;
	}

	UE_LOG(LogTemp, Log, TEXT("Dwarfy took %d damage. Lives: %d/%d"), DamageTaken, CurrentLives, MaxLives);
	if (bShowDwarfDebug && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(TEXT("Dwarfy hit: %d/%d lives"), CurrentLives, MaxLives));
	}

	OnDwarfDamaged(CurrentLives, MaxLives, DamageTaken, DamageCauser);

	if (CurrentLives <= 0)
	{
		bIsDead = true;
		SetPanicState(false, nullptr);
		GetWorldTimerManager().ClearTimer(PanicTimerHandle);
		GetCharacterMovement()->DisableMovement();
		UE_LOG(LogTemp, Warning, TEXT("Dwarfy died."));
		if (bShowDwarfDebug && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Dwarfy died"));
		}
		OnDwarfDied(DamageCauser);
	}

	return static_cast<float>(DamageTaken);
}

void AELNDwarfCharacter::UpdatePanicState()
{
	if (bIsDead)
	{
		SetPanicState(false, nullptr);
		return;
	}

	AELNSpiderCharacter* ThreatActor = FindNearestLivingSpiderInRange();
	SetPanicState(ThreatActor != nullptr, ThreatActor);
}

void AELNDwarfCharacter::SetPanicState(bool bNewPanicking, AELNSpiderCharacter* ThreatActor)
{
	if (bIsPanicking == bNewPanicking)
	{
		PanicThreat = ThreatActor;
		return;
	}

	bIsPanicking = bNewPanicking;
	PanicThreat = ThreatActor;
	GetCharacterMovement()->MaxWalkSpeed = bIsPanicking ? PanicSpeed : WalkSpeed;

	if (bIsPanicking)
	{
		UE_LOG(LogTemp, Log, TEXT("Dwarfy panic started."));
		if (bShowDwarfDebug && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Orange, TEXT("Dwarfy panic"));
		}
		OnDwarfPanicStarted(ThreatActor);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Dwarfy panic ended."));
		OnDwarfPanicEnded();
	}
}

AELNSpiderCharacter* AELNDwarfCharacter::FindNearestLivingSpiderInRange() const
{
	UWorld* World = GetWorld();
	if (!World || PanicDetectionRadius <= 0.f)
	{
		return nullptr;
	}

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ELNDwarfPanicCheck), false, this);

	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByObjectType(
		Overlaps,
		GetActorLocation(),
		FQuat::Identity,
		ObjectQueryParams,
		FCollisionShape::MakeSphere(PanicDetectionRadius),
		QueryParams
	);

	AELNSpiderCharacter* NearestSpider = nullptr;
	float BestDistanceSquared = TNumericLimits<float>::Max();

	for (const FOverlapResult& Overlap : Overlaps)
	{
		AELNSpiderCharacter* Spider = Cast<AELNSpiderCharacter>(Overlap.GetActor());
		if (!Spider || Spider->IsDead())
		{
			continue;
		}

		const float DistanceSquared = FVector::DistSquared(GetActorLocation(), Spider->GetActorLocation());
		if (DistanceSquared < BestDistanceSquared)
		{
			BestDistanceSquared = DistanceSquared;
			NearestSpider = Spider;
		}
	}

	return NearestSpider;
}
