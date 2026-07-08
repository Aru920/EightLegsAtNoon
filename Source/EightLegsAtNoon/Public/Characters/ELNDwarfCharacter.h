#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ELNDwarfCharacter.generated.h"

class AELNDwarfSafeZone;

UCLASS()
class EIGHTLEGSATNOON_API AELNDwarfCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AELNDwarfCharacter();

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UFUNCTION(BlueprintPure, Category = "Dwarfy")
	int32 GetCurrentLives() const { return CurrentLives; }

	UFUNCTION(BlueprintPure, Category = "Dwarfy")
	int32 GetMaxLives() const { return MaxLives; }

	UFUNCTION(BlueprintPure, Category = "Dwarfy")
	bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintPure, Category = "AI")
	AELNDwarfSafeZone* GetSafeZone() const { return SafeZone; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Dwarfy")
	void OnDwarfDamaged(int32 NewLives, int32 MaxLivesValue, int32 DamageTaken, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent, Category = "Dwarfy")
	void OnDwarfDied(AActor* DamageCauser);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float WalkSpeed = 220.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<AELNDwarfSafeZone> SafeZone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dwarfy", meta = (ClampMin = "1"))
	int32 MaxLives = 3;

	UPROPERTY(BlueprintReadOnly, Category = "Dwarfy")
	int32 CurrentLives = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dwarfy|Debug")
	bool bShowDwarfDebug = true;

private:
	bool bIsDead = false;
};
