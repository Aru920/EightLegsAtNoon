#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "ELNBTTask_FindSafeZonePoint.generated.h"

class UBoxComponent;

UCLASS()
class EIGHTLEGSATNOON_API UELNBTTask_FindSafeZonePoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UELNBTTask_FindSafeZonePoint();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	bool IsPointInsideSafeZone2D(const UBoxComponent* ZoneBox, const FVector& WorldLocation) const;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MoveLocationKey;

	UPROPERTY(EditAnywhere, Category = "Safe Zone", meta = (ClampMin = "1"))
	int32 MaxAttempts = 12;

	UPROPERTY(EditAnywhere, Category = "Safe Zone", meta = (ClampMin = "0.1", ClampMax = "1.0"))
	float PointScale = 0.85f;

	UPROPERTY(EditAnywhere, Category = "Safe Zone")
	FVector QueryExtent = FVector(80.f, 80.f, 300.f);

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebug = false;
};
