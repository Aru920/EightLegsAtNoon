#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ELNSpiderAIController.generated.h"

class AELNDwarfCharacter;
class AELNSpiderCharacter;

UCLASS()
class EIGHTLEGSATNOON_API AELNSpiderAIController : public AAIController
{git 
	GENERATED_BODY()

public:
	AELNSpiderAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;

	void FindDwarfTarget();
	void ChaseDwarf();
	bool TryAttackDwarf(AELNSpiderCharacter* Spider);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spider AI", meta = (ClampMin = "0.0"))
	float AcceptanceRadius = 75.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spider AI", meta = (ClampMin = "0.0"))
	float AttackRange = 110.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spider AI", meta = (ClampMin = "0.0"))
	float AttackDamage = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spider AI", meta = (ClampMin = "0.01"))
	float ChaseRefreshTime = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spider AI|Debug")
	bool bShowAttackDebug = true;

	UPROPERTY(BlueprintReadOnly, Category = "Spider AI")
	TObjectPtr<AELNDwarfCharacter> DwarfTarget;

private:
	float ChaseRefreshTimer = 0.f;
};
