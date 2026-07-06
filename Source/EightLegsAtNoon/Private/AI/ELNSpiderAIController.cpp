#include "AI/ELNSpiderAIController.h"

#include "Characters/ELNDwarfCharacter.h"
#include "Characters/ELNSpiderCharacter.h"
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

	const AELNSpiderCharacter* Spider = Cast<AELNSpiderCharacter>(GetPawn());
	if (!Spider || Spider->IsDead())
	{
		return;
	}

	if (!DwarfTarget)
	{
		FindDwarfTarget();
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
