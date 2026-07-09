#include "AI/ELNSpiderAIController.h"

#include "Characters/ELNDwarfCharacter.h"
#include "Characters/ELNSpiderCharacter.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

AELNSpiderAIController::AELNSpiderAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AELNSpiderAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	FindDwarfTarget();
	ChaseDwarf();
}

void AELNSpiderAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AttackCooldownRemaining = FMath::Max(0.f, AttackCooldownRemaining - DeltaSeconds);

	AELNSpiderCharacter* Spider = Cast<AELNSpiderCharacter>(GetPawn());
	if (!Spider || Spider->IsDead())
	{
		return;
	}

	if (!DwarfTarget)
	{
		FindDwarfTarget();
	}

	if (!DwarfTarget || DwarfTarget->IsDead())
	{
		StopMovement();
		return;
	}

	if (TryAttackDwarf(Spider))
	{
		return;
	}

	ChaseRefreshTimer -= DeltaSeconds;
	if (ChaseRefreshTimer <= 0.f)
	{
		ChaseDwarf();
		ChaseRefreshTimer = ChaseRefreshTime;
	}
}

void AELNSpiderAIController::FindDwarfTarget()
{
	DwarfTarget = Cast<AELNDwarfCharacter>(
		UGameplayStatics::GetActorOfClass(this, AELNDwarfCharacter::StaticClass())
	);
}

void AELNSpiderAIController::ChaseDwarf()
{
	if (!DwarfTarget)
	{
		return;
	}

	MoveToActor(DwarfTarget, AcceptanceRadius, true);
}

bool AELNSpiderAIController::TryAttackDwarf(AELNSpiderCharacter* Spider)
{
	if (!Spider || !DwarfTarget || DwarfTarget->IsDead())
	{
		return false;
	}

	if (Spider->IsAttacking())
	{
		return true;
	}

	if (AttackCooldownRemaining > 0.f)
	{
		return true;
	}

	const float DistanceSquared = FVector::DistSquared(Spider->GetActorLocation(), DwarfTarget->GetActorLocation());
	if (DistanceSquared > FMath::Square(AttackRange))
	{
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("%s attacked Dwarfy."), *Spider->GetName());
	if (bShowAttackDebug && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Orange, TEXT("Spider hit Dwarfy"));
	}

	Spider->StartAttack(DwarfTarget, AttackDamage, this);
	AttackCooldownRemaining = AttackCooldown;
	return true;
}
