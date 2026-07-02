// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ELNPlayerCharacter.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;
class USpringArmComponent;

UCLASS()
class EIGHTLEGSATNOON_API AELNPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AELNPlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category = "Aiming")
	FVector GetCursorWorldLocation() const { return CursorWorldLocation; }

protected:
	void UpdateCursorAim(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> TopDownCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "View Model")
	TObjectPtr<USkeletalMeshComponent> ShotgunArms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming", meta = (ClampMin = "1000.0"))
	float CursorTraceDistance = 50000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "View Model|Aiming", meta = (ClampMin = "0.0"))
	float ViewModelAimYawLimit = 35.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "View Model|Aiming", meta = (ClampMin = "0.0"))
	float ViewModelAimPitchLimit = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "View Model|Aiming", meta = (ClampMin = "0.0"))
	float ViewModelAimInterpSpeed = 60.f;

	UPROPERTY(BlueprintReadOnly, Category = "Aiming")
	FVector CursorWorldLocation = FVector::ZeroVector;

	FRotator ShotgunArmsBaseRotation = FRotator::ZeroRotator;
};
