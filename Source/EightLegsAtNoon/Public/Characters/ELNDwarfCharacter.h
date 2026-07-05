#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ELNDwarfCharacter.generated.h"

UCLASS()
class EIGHTLEGSATNOON_API AELNDwarfCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AELNDwarfCharacter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float WalkSpeed = 220.f;
};
