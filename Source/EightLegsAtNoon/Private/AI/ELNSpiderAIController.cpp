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

	const float DistanceSquared = FVector::DistSquared(Spider->GetActorLocation(), DwarfTarget->GetActorLocation());
	if (DistanceSquared > FMath::Square(AttackRange))
	{
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("%s attacked Dwarfy and will die."), *Spider->GetName());
	if (bShowAttackDebug && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Orange, TEXT("Spider hit Dwarfy"));
	}

	UGameplayStatics::ApplyDamage(DwarfTarget, AttackDamage, this, Spider, nullptr);
	Spider->KillSpider(this, DwarfTarget);
	return true;
}
