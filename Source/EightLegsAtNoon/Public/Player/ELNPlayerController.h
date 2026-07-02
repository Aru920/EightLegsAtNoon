#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ELNPlayerController.generated.h"

UCLASS()
class EIGHTLEGSATNOON_API AELNPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
