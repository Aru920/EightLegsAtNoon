#include "AI/ELNBTTask_FindSafeZonePoint.h"

#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Characters/ELNDwarfCharacter.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "World/ELNDwarfSafeZone.h"

UELNBTTask_FindSafeZonePoint::UELNBTTask_FindSafeZonePoint()
{
	NodeName = TEXT("Find Safe Zone Point");
	MoveLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UELNBTTask_FindSafeZonePoint, MoveLocationKey));
}

EBTNodeResult::Type UELNBTTask_FindSafeZonePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	const AELNDwarfCharacter* Dwarf = Cast<AELNDwarfCharacter>(AIController->GetPawn());
	if (!Dwarf)
	{
		return EBTNodeResult::Failed;
	}

	const AELNDwarfSafeZone* SafeZone = Dwarf->GetSafeZone();
	const UBoxComponent* ZoneBox = SafeZone ? SafeZone->GetZoneBox() : nullptr;
	if (!ZoneBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Safe Zone Point failed: Dwarfy has no SafeZone."));
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIController->GetWorld());
	if (!Blackboard || !NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	const FVector Center = ZoneBox->GetComponentLocation();
	const FVector Extent = ZoneBox->GetScaledBoxExtent();
	const float XRange = Extent.X * PointScale;
	const float YRange = Extent.Y * PointScale;

	for (int32 Attempt = 0; Attempt < MaxAttempts; ++Attempt)
	{
		const float LocalX = FMath::FRandRange(-XRange, XRange);
		const float LocalY = FMath::FRandRange(-YRange, YRange);

		FVector RandomPoint = Center;
		RandomPoint += ZoneBox->GetForwardVector() * LocalX;
		RandomPoint += ZoneBox->GetRightVector() * LocalY;

		FNavLocation ProjectedLocation;
		if (!NavSystem->ProjectPointToNavigation(RandomPoint, ProjectedLocation, QueryExtent))
		{
			continue;
		}

		if (!IsPointInsideSafeZone2D(ZoneBox, ProjectedLocation.Location))
		{
			continue;
		}

		Blackboard->SetValueAsVector(MoveLocationKey.SelectedKeyName, ProjectedLocation.Location);

		if (bDrawDebug)
		{
			DrawDebugSphere(AIController->GetWorld(), ProjectedLocation.Location, 45.f, 12, FColor::Green, false, 1.5f);
		}

		return EBTNodeResult::Succeeded;
	}

	UE_LOG(LogTemp, Warning, TEXT("Find Safe Zone Point failed after %d attempts."), MaxAttempts);
	return EBTNodeResult::Failed;
}

bool UELNBTTask_FindSafeZonePoint::IsPointInsideSafeZone2D(const UBoxComponent* ZoneBox, const FVector& WorldLocation) const
{
	if (!ZoneBox)
	{
		return false;
	}

	const FVector LocalLocation = ZoneBox->GetComponentTransform().InverseTransformPosition(WorldLocation);
	const FVector LocalExtent = ZoneBox->GetUnscaledBoxExtent();

	return FMath::Abs(LocalLocation.X) <= LocalExtent.X
		&& FMath::Abs(LocalLocation.Y) <= LocalExtent.Y;
}
