#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ELNDwarfAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class EIGHTLEGSATNOON_API AELNDwarfAIController : public AAIController
{
	GENERATED_BODY()

public:
	AELNDwarfAIController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> DwarfPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
};
