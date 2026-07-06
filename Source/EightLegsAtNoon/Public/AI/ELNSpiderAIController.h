#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ELNSpiderAIController.generated.h"

class AELNDwarfCharacter;

UCLASS()
class EIGHTLEGSATNOON_API AELNSpiderAIController : public AAIController
{
	GENERATED_BODY()

public:
	AELNSpiderAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;

	void FindDwarfTarget();
	void ChaseDwarf();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spider AI", meta = (ClampMin = "0.0"))
	float AcceptanceRadius = 75.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spider AI", meta = (ClampMin = "0.01"))
	float ChaseRefreshTime = 0.25f;

	UPROPERTY(BlueprintReadOnly, Category = "Spider AI")
	TObjectPtr<AELNDwarfCharacter> DwarfTarget;

private:
	float ChaseRefreshTimer = 0.f;
};
