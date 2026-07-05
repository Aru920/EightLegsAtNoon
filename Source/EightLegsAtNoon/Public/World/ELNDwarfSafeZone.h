#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ELNDwarfSafeZone.generated.h"

class UBoxComponent;

UCLASS()
class EIGHTLEGSATNOON_API AELNDwarfSafeZone : public AActor
{
	GENERATED_BODY()

public:
	AELNDwarfSafeZone();

	UFUNCTION(BlueprintPure, Category = "Safe Zone")
	UBoxComponent* GetZoneBox() const { return ZoneBox; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Safe Zone")
	TObjectPtr<UBoxComponent> ZoneBox;
};
