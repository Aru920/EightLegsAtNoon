#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "ELNDwarfCharacter.generated.h"

class AELNDwarfSafeZone;
class AELNSpiderCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FELNDwarfLivesChangedSignature, int32, CurrentLives, int32, MaxLives);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FELNDwarfDeathSignature, AActor*, DamageCauser);

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

	UFUNCTION(BlueprintPure, Category = "Dwarfy")
	bool IsPanicking() const { return bIsPanicking; }

	UFUNCTION(BlueprintPure, Category = "Dwarfy")
	AActor* GetPanicThreat() const { return PanicThreat; }

	UFUNCTION(BlueprintPure, Category = "AI")
	AELNDwarfSafeZone* GetSafeZone() const { return SafeZone; }

	UFUNCTION(BlueprintCallable, Category = "Dwarfy")
	void NotifyFriendlyShot(AActor* ShotBy);

	UPROPERTY(BlueprintAssignable, Category = "Dwarfy")
	FELNDwarfLivesChangedSignature OnLivesChanged;

	UPROPERTY(BlueprintAssignable, Category = "Dwarfy")
	FELNDwarfDeathSignature OnDwarfDeath;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Dwarfy")
	void OnDwarfDamaged(int32 NewLives, int32 MaxLivesValue, int32 DamageTaken, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent, Category = "Dwarfy")
	void OnDwarfDied(AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent, Category = "Dwarfy")
	void OnDwarfPanicStarted(AActor* ThreatActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Dwarfy")
	void OnDwarfPanicEnded();

	UFUNCTION(BlueprintImplementableEvent, Category = "Dwarfy")
	void OnDwarfFriendlyShot(AActor* ShotBy);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float WalkSpeed = 220.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float PanicSpeed = 360.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Hit Reaction", meta = (ClampMin = "0.0"))
	float HitSlowSpeed = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Hit Reaction", meta = (ClampMin = "0.0"))
	float HitSlowDuration = 0.75f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<AELNDwarfSafeZone> SafeZone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (ClampMin = "0.0"))
	float PanicDetectionRadius = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (ClampMin = "0.01"))
	float PanicCheckInterval = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dwarfy", meta = (ClampMin = "1"))
	int32 MaxLives = 3;

	UPROPERTY(BlueprintReadOnly, Category = "Dwarfy")
	int32 CurrentLives = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dwarfy|Debug")
	bool bShowDwarfDebug = true;

private:
	void UpdatePanicState();
	void SetPanicState(bool bNewPanicking, AELNSpiderCharacter* ThreatActor);
	AELNSpiderCharacter* FindNearestLivingSpiderInRange() const;
	void ApplyHitSlow();
	void ClearHitSlow();
	void RefreshMoveSpeed();

	UPROPERTY()
	TObjectPtr<AActor> PanicThreat;

	FTimerHandle PanicTimerHandle;
	FTimerHandle HitSlowTimerHandle;

	bool bIsPanicking = false;
	bool bIsHitSlowed = false;
	bool bIsDead = false;
};
