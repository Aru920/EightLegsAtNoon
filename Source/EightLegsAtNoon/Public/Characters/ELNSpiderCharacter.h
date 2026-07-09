#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ELNSpiderCharacter.generated.h"

class USkeletalMesh;

UCLASS()
class EIGHTLEGSATNOON_API AELNSpiderCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AELNSpiderCharacter();

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UFUNCTION(BlueprintPure, Category = "Spider")
	bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintPure, Category = "Spider")
	bool IsAttacking() const { return bIsAttacking; }

	UFUNCTION(BlueprintCallable, Category = "Spider")
	void StartAttack(AActor* AttackTarget, float DamageAmount, AController* EventInstigator);

	UFUNCTION(BlueprintCallable, Category = "Spider")
	bool ApplyAttackDamage();

	UFUNCTION(BlueprintCallable, Category = "Spider")
	void FinishAttack(bool bApplyDamage = true);

	UFUNCTION(BlueprintCallable, Category = "Spider")
	void KillSpider(AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetWalkSpeed(float NewWalkSpeed);

	UFUNCTION(BlueprintCallable, Category = "Spider")
	void SetVisualMesh(USkeletalMesh* NewMesh);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Spider")
	void OnSpiderDied(AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintNativeEvent, Category = "Spider")
	void OnSpiderAttackStarted(AActor* AttackTarget);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spider", meta = (ClampMin = "1.0"))
	float MaxHealth = 1.f;

	UPROPERTY(BlueprintReadOnly, Category = "Spider")
	float Health = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float WalkSpeed = 360.f;

private:
	UPROPERTY()
	TObjectPtr<AActor> CurrentAttackTarget;

	UPROPERTY()
	TObjectPtr<AController> PendingAttackInstigator;

	float PendingAttackDamage = 0.f;
	bool bHasAppliedAttackDamage = false;
	bool bIsAttacking = false;
	bool bIsDead = false;
};
