#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ELNDwarfAnimInstance.generated.h"

UCLASS()
class EIGHTLEGSATNOON_API UELNDwarfAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Direction = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsMoving = false;
};
